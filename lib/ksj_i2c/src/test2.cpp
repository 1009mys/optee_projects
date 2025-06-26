#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <cstdint>
#include <cstring>

#define BSC1_BASE 0x3F804000
#define BLOCK_SIZE 0x1000

// BSC 레지스터 오프셋
#define BSC_C     0x00
#define BSC_S     0x04
#define BSC_DLEN  0x08
#define BSC_A     0x0C
#define BSC_FIFO  0x10

// BSC 제어 비트
#define BSC_C_I2CEN (1 << 15)
#define BSC_C_ST    (1 << 7)
#define BSC_C_CLEAR (3 << 4)
#define BSC_C_READ  (1 << 0)

// BSC 상태 비트
#define BSC_S_DONE  (1 << 1)

#define CLOCK_BASE 0x3F101000
#define CM_I2C1_CTL (0x98 / 4)
#define CM_I2C1_DIV (0x9C / 4)

#define GPIO_BASE 0x3F200000

#define CLOCK_BASE 0x3F101000
#define CM_I2C1_CTL (0x98 / 4)
#define CM_I2C1_DIV (0x9C / 4)


volatile uint32_t* map_bsc() {
    int fd = open("/dev/mem", O_RDWR | O_SYNC);
    if (fd < 0) {
        perror("open /dev/mem");
        exit(1);
    }

    void* map = mmap(nullptr, BLOCK_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, BSC1_BASE);
    close(fd);

    if (map == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }

    return reinterpret_cast<volatile uint32_t*>(map);
}

void bsc_write(volatile uint32_t* bsc, uint8_t addr, uint8_t reg, uint8_t val) {
    bsc[BSC_C / 4] = BSC_C_CLEAR;
    bsc[BSC_A / 4] = addr;
    bsc[BSC_FIFO / 4] = reg;
    bsc[BSC_FIFO / 4] = val;
    bsc[BSC_DLEN / 4] = 2;
    bsc[BSC_C / 4] = BSC_C_I2CEN | BSC_C_ST;

    while (!(bsc[BSC_S / 4] & BSC_S_DONE));
}

uint8_t bsc_read(volatile uint32_t* bsc, uint8_t addr, uint8_t reg) {
    // Write register address first
    bsc[BSC_C / 4] = BSC_C_CLEAR;
    bsc[BSC_A / 4] = addr;
    bsc[BSC_FIFO / 4] = reg;
    bsc[BSC_DLEN / 4] = 1;
    bsc[BSC_C / 4] = BSC_C_I2CEN | BSC_C_ST;
    while (!(bsc[BSC_S / 4] & BSC_S_DONE));
    usleep(1000);

    // Read phase
    bsc[BSC_C / 4] = BSC_C_CLEAR;
    bsc[BSC_A / 4] = addr;
    bsc[BSC_DLEN / 4] = 1;
    bsc[BSC_C / 4] = BSC_C_I2CEN | BSC_C_ST | BSC_C_READ;
    while (!(bsc[BSC_S / 4] & BSC_S_DONE));

    return bsc[BSC_FIFO / 4] & 0xFF;
}


volatile uint32_t* map_gpio() {
    int fd = open("/dev/mem", O_RDWR | O_SYNC);
    if (fd < 0) {
        perror("open /dev/mem");
        exit(1);
    }

    void* map = mmap(nullptr, BLOCK_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, GPIO_BASE);
    close(fd);

    if (map == MAP_FAILED) {
        perror("mmap gpio");
        exit(1);
    }

    return reinterpret_cast<volatile uint32_t*>(map);
}

volatile uint32_t* map_clock() {
    int fd = open("/dev/mem", O_RDWR | O_SYNC);
    if (fd < 0) {
        perror("open /dev/mem clock");
        exit(1);
    }
    void* map = mmap(nullptr, BLOCK_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, CLOCK_BASE);
    close(fd);
    if (map == MAP_FAILED) {
        perror("mmap clock");
        exit(1);
    }
    return reinterpret_cast<volatile uint32_t*>(map);
}

void enable_bsc1_clock(volatile uint32_t* clk) {
    clk[CM_I2C1_CTL] = 0x5A000006;  // Password + ENAB + SRC = OSC
    usleep(100);
}

void set_gpio_alt0_for_i2c(volatile uint32_t* gpio) {
    uint32_t gpfsel0 = gpio[0];  // GPFSEL0
    gpfsel0 &= ~(0b111 << 6);    // Clear GPIO2
    gpfsel0 &= ~(0b111 << 9);    // Clear GPIO3
    gpfsel0 |=  (0b100 << 6);    // ALT0 for GPIO2
    gpfsel0 |=  (0b100 << 9);    // ALT0 for GPIO3
    gpio[0] = gpfsel0;
}

int main() {
    // 1. GPIO2, GPIO3 핀 기능을 ALT0로 설정 (I2C)
    volatile uint32_t* gpio = map_gpio();
    set_gpio_alt0_for_i2c(gpio);

    auto clk = map_clock();
    enable_bsc1_clock(clk);

    // 2. I2C 컨트롤러 맵핑
    volatile uint32_t* bsc = map_bsc();

    const uint8_t addr = 0x68;
    const uint8_t reg_who_am_i = 0x75;
    const uint8_t reg_pwr_mgmt_1 = 0x6B;

    // 3. 슬립 해제
    bsc_write(bsc, addr, reg_pwr_mgmt_1, 0x00);
    usleep(5000);

    // 4. WHO_AM_I 읽기
    uint8_t id = bsc_read(bsc, addr, reg_who_am_i);
    std::cout << "WHO_AM_I: 0x" << std::hex << static_cast<int>(id) << std::endl;

    return 0;
}