#include <tee_internal_api.h>
#include <tee_internal_api_extensions.h>

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdint.h>

#include "include/my_gpio_ta.h"

#include "onnxruntime/core/session/onnxruntime_cxx_api.h"

// Raspberry Pi 3의 Peripheral base 주소 (BCM2837)
#define BCM2837_PERI_BASE 0x3F000000
// GPIO 레지스터의 base 주소 (오프셋 0x200000)
#define GPIO_BASE (BCM2837_PERI_BASE + 0x200000)
// 매핑할 메모리 블록 크기 (4KB)
#define BLOCK_SIZE (4*1024)

volatile unsigned *gpio;


TEE_Result TA_CreateEntryPoint(void) {
    return TEE_SUCCESS;
}

void TA_DestroyEntryPoint(void) {
    DMSG("Secure GPIO TA Destroyed.");
}

// 세션 오픈 시 호출
TEE_Result TA_OpenSessionEntryPoint(uint32_t param_types, TEE_Param params[4],
    void **sess_ctx) 
{
    return TEE_SUCCESS;
}

// 세션 닫힐 때 호출
void TA_CloseSessionEntryPoint(void *sess_ctx) {
    (void)sess_ctx;
    DMSG("GPIO control ended.");
}

TEE_Result TA_InvokeCommandEntryPoint(void *sess_ctx, uint32_t cmd_id, uint32_t param_types,
    TEE_Param params[4]) {
        int mem_fd;
    

    return TEE_SUCCESS;
}