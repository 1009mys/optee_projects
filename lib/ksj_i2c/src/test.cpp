#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>

#define MPU6050_ADDR 0x68
#define WHO_AM_I_REG 0x75
#define PWR_MGMT_1   0x6B

int test() {
    const char* i2c_device = "/dev/i2c-1";
    int file;

    // 1. I2C 디바이스 열기
    if ((file = open(i2c_device, O_RDWR)) < 0) {
        perror("Failed to open I2C device");
        return 1;
    }

    // 2. 슬레이브 주소 설정
    if (ioctl(file, I2C_SLAVE, MPU6050_ADDR) < 0) {
        perror("Failed to set I2C slave address");
        close(file);
        return 1;
    }

    // 3. 슬립 해제 (PWR_MGMT_1 ← 0x00)
    uint8_t wake_cmd[2] = {PWR_MGMT_1, 0x00};
    if (write(file, wake_cmd, 2) != 2) {
        perror("Failed to write wake-up command");
        close(file);
        return 1;
    }

    usleep(5000);  // 센서 안정화

    // 4. WHO_AM_I 레지스터 지정
    uint8_t who_am_i_reg = WHO_AM_I_REG;
    if (write(file, &who_am_i_reg, 1) != 1) {
        perror("Failed to write WHO_AM_I register address");
        close(file);
        return 1;
    }

    // 5. WHO_AM_I 값 읽기
    uint8_t id;
    if (read(file, &id, 1) != 1) {
        perror("Failed to read WHO_AM_I");
        close(file);
        return 1;
    }

    std::cout << "WHO_AM_I: 0x" << std::hex << static_cast<int>(id) << std::endl;

    close(file);
    return 0;
}