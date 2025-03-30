#include "ksj_gpio.h"

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdint.h>

volatile unsigned *gpio;

void ksj_gpio_init(void) {
    // Initialize GPIO
    int mem_fd;
    void *gpio_map;

    // /dev/mem 파일을 읽기/쓰기 모드로 연다. (root 권한 필요)
    if ((mem_fd = open("/dev/mem", O_RDWR | O_SYNC)) < 0) {
        perror("Error opening /dev/mem. Run as root.");
        return 1;
    }

    // GPIO 메모리 영역을 매핑한다.
    gpio_map = mmap(
        NULL,                // 커널이 적절한 주소 선택
        BLOCK_SIZE,          // 매핑 크기 (4KB)
        PROT_READ | PROT_WRITE,  // 읽기/쓰기 권한
        MAP_SHARED,          // 여러 프로세스 간 공유 가능
        mem_fd,              // 파일 디스크립터
        GPIO_BASE            // 매핑할 물리 주소
    );
    close(mem_fd);  // 더 이상 필요없으므로 닫음

    if (gpio_map == MAP_FAILED) {
        perror("mmap error");
        return 1;
    }

    // 매핑된 메모리를 volatile 포인터로 캐스팅
    gpio = (volatile unsigned *)gpio_map;
}

void ksj_pinMode(int pin, int mode) {
    // Set GPIO pin mode

    // 0~9: GPFSEL0
    // 10~19: GPFSEL1
    // 20~29: GPFSEL2
    // 30~39: GPFSEL3
    // 40~49: GPFSEL4
    // 50~59: GPFSEL5

    switch (mode)
    {
    case OUTPUT:
        gpio[pin/10] &= ~(7 << (pin%10)*3);
        gpio[pin/10] |=  (1 << (pin%10)*3);
        break;
    case INPUT:
        gpio[pin/10] &= ~(7 << (pin%10)*3);
        break;
    default:
        break;
    }
}

void ksj_digitalWrite(int pin, int value) {
    // Write GPIO pin
    if (value == HIGH) {
        gpio[7] = 1 << pin;
    } else {
        gpio[10] = 1 << pin;
    }
}

int ksj_digitalRead(int pin) {
    // Read GPIO pin
    return (gpio[13] & (1 << pin)) ? HIGH : LOW;
}