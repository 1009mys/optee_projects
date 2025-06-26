#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#define BSC1_BASE 0x3F804000  // Raspberry Pi 2/3 기준
#define PAGE_SIZE 4096
#define BLOCK_SIZE 4096

// BSC 레지스터 오프셋
#define BSC_C     0x00
#define BSC_S     0x04
#define BSC_DLEN  0x08
#define BSC_A     0x0C
#define BSC_FIFO  0x10

// BSC 제어 비트
#define BSC_C_I2CEN 0x00008000
#define BSC_C_ST    0x00000080
#define BSC_C_CLEAR 0x00000030
#define BSC_C_READ  0x00000001

// BSC 상태 비트
#define BSC_S_TA       0x00000001  // Transfer Active
#define BSC_S_DONE     0x00000002  // Transfer Done
#define BSC_S_TXW      0x00000004  // TX FIFO needs writing
#define BSC_S_RXR      0x00000008  // RX FIFO needs reading
#define BSC_S_TXD      0x00000010  // TX FIFO can accept data
#define BSC_S_RXD      0x00000020  // RX FIFO contains data
#define BSC_S_TXE      0x00000040  // TX FIFO empty
#define BSC_S_RXF      0x00000080  // RX FIFO full
#define BSC_S_ERR      0x00000100  // ACK Error
#define BSC_S_CLKT     0x00000200  // Clock Stretch Timeout
#define BSC_S_RESV     0xFFFFFC00  // Reserved bits

void setup_bsc1();

void setup_bsc1();
void write_register(uint8_t device_addr, uint8_t reg_addr, uint8_t value);
void read_block(uint8_t device_addr, uint8_t reg_addr, uint8_t* buffer, uint8_t length);
int16_t combine_bytes(uint8_t msb, uint8_t lsb);
uint8_t test_register(uint8_t device_addr, uint8_t reg_addr);
void print_bsc_status();
void wake_mpu6050(uint8_t device_addr);