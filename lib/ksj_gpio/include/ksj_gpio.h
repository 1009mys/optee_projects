#ifndef KSJ_GPIO_H
#define KSJ_GPIO_H

#define INPUT 0
#define OUTPUT 1

#define LOW 0
#define HIGH 1

// Raspberry Pi 3의 Peripheral base 주소 (BCM2837)
#define BCM2837_PERI_BASE 0x3F000000
// GPIO 레지스터의 base 주소 (오프셋 0x200000)
#define GPIO_BASE (BCM2837_PERI_BASE + 0x200000)
// 매핑할 메모리 블록 크기 (4KB)
#define BLOCK_SIZE (4*1024)

void ksj_gpio_init(void);

void ksj_pinMode(int pin, int mode);
void ksj_digitalWrite(int pin, int value);
int ksj_digitalRead(int pin);


#endif // KSJ_GPIO_H