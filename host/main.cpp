#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdint.h>
#include <cerrno>
#include <cstdio>

#include <onnxruntime/core/session/onnxruntime_cxx_api.h>

#ifdef __INTELLISENSE__
extern "C" void perror(const char *s);
extern "C" void* mmap(void*, size_t, int, int, int, off_t);
extern "C" void close(int fd);
extern "C" int usleep(unsigned int usec);
#endif

// Raspberry Pi 3의 Peripheral base 주소 (BCM2837)
#define BCM2837_PERI_BASE 0x3F000000
// GPIO 레지스터의 base 주소 (오프셋 0x200000)
#define GPIO_BASE (BCM2837_PERI_BASE + 0x200000)
// 매핑할 메모리 블록 크기 (4KB)
#define BLOCK_SIZE (4*1024)

// 매핑된 GPIO 메모리를 가리키는 포인터
volatile unsigned *gpio;

int main(int argc, char **argv) {
    int mem_fd;
    void *gpio_map;

    // /dev/mem 파일을 읽기/쓰기 모드로 연다. (root 권한 필요)
    if ((mem_fd = open("/dev/mem", O_RDWR | O_SYNC)) < 0) {
        perror("Error opening /dev/mem. Run as root.");
        return 1;
    }

    // GPIO 메모리 영역을 매핑한다.
    gpio_map = mmap(
        NULL,                // 커널이 적절한 주소 선
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

    // GPIO 18번 핀을 출력 모드로 설정
    // GPIO 18은 GPFSEL1 레지스터(인덱스 1)의 24~26비트에 해당함.
    gpio[1] &= ~(7 << 24);  // 해당 비트들을 클리어
    gpio[1] |=  (1 << 24);  // 출력 모드 (001)로 설정

    // GPIO 23번 핀을 출력 모드로 설정
    // GPIO 23은 GPFSEL2 레지스터(인덱스 2)의 9~11비트에 해당함.
    gpio[2] &= ~(7 << 9);  // 해당 비트들을 클리어
    gpio[2] |=  (1 << 9);  // 출력 모드 (001)로 설정

    // LED 깜빡임 예제: GPIO 18번 핀을 주기적으로 토글
    while (1) {
        // GPSET0 레지스터 (인덱스 7)를 사용하여 GPIO 18을 HIGH로 설정
        gpio[7] = 1 << 18;
        usleep(500000);  // 500ms 대기

        // GPCLR0 레지스터 (인덱스 10)를 사용하여 GPIO 18을 LOW로 설정
        gpio[10] = 1 << 18;
        usleep(500000);  // 500ms 대기

        // GPSET0 레지스터 (인덱스 7)를 사용하여 GPIO 23을 HIGH로 설정
        gpio[7] = 1 << 23;
        usleep(500000);  // 500ms 대기

        // GPCLR0 레지스터 (인덱스 10)를 사용하여 GPIO 23을 LOW로 설정
        gpio[10] = 1 << 23;
        usleep(500000);  // 500ms 대기
        
    }

    return 0;
}