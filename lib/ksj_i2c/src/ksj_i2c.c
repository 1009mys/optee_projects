#include <stdint.h>
#include <stdio.h>
#include <stdlib.h> // Ensure this header is included for EXIT_FAILURE

#include "ksj_i2c.h"

volatile uint32_t *bsc1;

void setup_bsc1() {
    int mem_fd = open("/dev/mem", O_RDWR | O_SYNC);
    if (mem_fd < 0) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    void *bsc1_map = mmap(
        NULL,
        BLOCK_SIZE,
        PROT_READ | PROT_WRITE,
        MAP_SHARED,
        mem_fd,
        BSC1_BASE
    );

    if (bsc1_map == MAP_FAILED) {
        perror("mmap");
        close(mem_fd);
        exit(EXIT_FAILURE);
    }

    bsc1 = (volatile uint32_t *)bsc1_map;
    close(mem_fd);
}

void write_register(uint8_t device_addr, uint8_t reg_addr, uint8_t value) {
    bsc1[BSC_C / 4] = BSC_C_CLEAR;
    bsc1[BSC_A / 4] = device_addr;

    bsc1[BSC_FIFO / 4] = reg_addr;
    bsc1[BSC_FIFO / 4] = value;
    bsc1[BSC_DLEN / 4] = 2;
    bsc1[BSC_C / 4] = BSC_C_I2CEN | BSC_C_ST;

    while (!(bsc1[BSC_S / 4] & BSC_S_DONE));
    bsc1[BSC_S / 4] = BSC_S_DONE;
}

void read_block(uint8_t device_addr, uint8_t reg_addr, uint8_t* buffer, uint8_t length) {
    bsc1[BSC_C / 4] = BSC_C_CLEAR;
    bsc1[BSC_A / 4] = device_addr;

    bsc1[BSC_FIFO / 4] = reg_addr;
    bsc1[BSC_DLEN / 4] = 1;
    bsc1[BSC_C / 4] = BSC_C_I2CEN | BSC_C_ST;
    while (!(bsc1[BSC_S / 4] & BSC_S_DONE));
    bsc1[BSC_S / 4] = BSC_S_DONE;

    bsc1[BSC_DLEN / 4] = length;
    bsc1[BSC_C / 4] = BSC_C_I2CEN | BSC_C_ST | BSC_C_READ;
    while (!(bsc1[BSC_S / 4] & BSC_S_DONE));

    for (int i = 0; i < length; ++i) {
        buffer[i] = bsc1[BSC_FIFO / 4] & 0xFF;
    }
}

int16_t combine_bytes(uint8_t msb, uint8_t lsb) {
    return (int16_t)((msb << 8) | lsb);
}


uint8_t test_register(uint8_t device_addr, uint8_t reg_addr) {
    // Clear FIFO
    bsc1[BSC_C / 4] = BSC_C_CLEAR;

    // Set slave address
    bsc1[BSC_A / 4] = device_addr;

    // Write register address to FIFO
    bsc1[BSC_FIFO / 4] = reg_addr;

    // Set data length to 1
    bsc1[BSC_DLEN / 4] = 1;

    printf("BSC_S: 0x%08X\n", bsc1[BSC_S / 4]);

    // Start write transfer
    bsc1[BSC_C / 4] = BSC_C_I2CEN | BSC_C_ST;

    // Wait for transfer to complete
    while (!(bsc1[BSC_S / 4] & BSC_S_DONE));

    

    // Clear DONE bit
    bsc1[BSC_S / 4] = BSC_S_DONE;

    // Set data length to 1 for read
    bsc1[BSC_DLEN / 4] = 1;


    // Start read transfer
    bsc1[BSC_C / 4] = BSC_C_I2CEN | BSC_C_ST | BSC_C_READ;

    // Wait for transfer to complete
    while (!(bsc1[BSC_S / 4] & BSC_S_DONE));

    // 12. Check error
    if (bsc1[BSC_S / 4] & BSC_S_ERR) {
        fprintf(stderr, "I2C error!\n");
        return 0xFF;
    }

    // Read data from FIFO
    uint8_t data = bsc1[BSC_FIFO / 4] & 0xFF;

    return data;
}

void print_bsc_status() {
    uint32_t status = bsc1[BSC_S / 4];
    printf("BSC_S: 0x%08X\n", status);

    if (status & BSC_S_TA)   printf("  - Transfer Active\n");
    if (status & BSC_S_DONE) printf("  - Transfer Done\n");
    if (status & BSC_S_TXW)  printf("  - TX FIFO needs writing\n");
    if (status & BSC_S_RXR)  printf("  - RX FIFO needs reading\n");
    if (status & BSC_S_TXD)  printf("  - TX FIFO can accept data\n");
    if (status & BSC_S_RXD)  printf("  - RX FIFO contains data\n");
    if (status & BSC_S_TXE)  printf("  - TX FIFO is empty\n");
    if (status & BSC_S_RXF)  printf("  - RX FIFO is full\n");
    if (status & BSC_S_ERR)  printf("  - ACK Error (No ACK from slave!)\n");
    if (status & BSC_S_CLKT) printf("  - Clock Stretch Timeout\n");
}

void wake_mpu6050(uint8_t device_addr) {
    // Write 0x00 to register 0x6B
    bsc1[BSC_C / 4] = BSC_C_CLEAR;
    bsc1[BSC_A / 4] = device_addr;
    bsc1[BSC_FIFO / 4] = 0x6B;            // PWR_MGMT_1
    bsc1[BSC_FIFO / 4] = 0x00;            // Clear sleep
    bsc1[BSC_DLEN / 4] = 2;
    bsc1[BSC_C / 4] = BSC_C_I2CEN | BSC_C_ST;
    while (!(bsc1[BSC_S / 4] & BSC_S_DONE));
    bsc1[BSC_S / 4] = BSC_S_DONE;
}
