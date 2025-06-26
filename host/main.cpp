#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdint.h>
#include <cerrno>
#include <cstdio>
#include <cstring>
#include <err.h>

#include <tee_client_api.h>
#include <secure_storage_ta.h>

#include "ksj_onnxruntime/include/test.h"

#ifdef __INTELLISENSE__
extern "C" void perror(const char *s);
extern "C" void* mmap(void*, size_t, int, int, int, off_t);
extern "C" void close(int fd);
extern "C" int usleep(unsigned int usec);

typedef TEEC_Result;
#endif

// Raspberry Pi 3의 Peripheral base 주소 (BCM2837)
#define BCM2837_PERI_BASE 0x3F000000
// GPIO 레지스터의 base 주소 (오프셋 0x200000)
#define GPIO_BASE (BCM2837_PERI_BASE + 0x200000)
// 매핑할 메모리 블록 크기 (4KB)
#define BLOCK_SIZE (4*1024)

// 매핑된 GPIO 메모리를 가리키는 포인터
volatile unsigned *gpio;


int gpio_test() {
    //const char* version = OrtGetApiBase()->GetVersionString();
    //printf("ONNX Runtime version: %s\n", version);

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

struct test_ctx {
	TEEC_Context ctx;
	TEEC_Session sess;
};

void prepare_tee_session(struct test_ctx *ctx)
{
    printf("Preparing TEE session...\n");
    // print UUID
	TEEC_UUID uuid = TA_SECURE_STORAGE_UUID;
    printf("TA UUID: %08x-%04x-%04x-", uuid.timeLow, uuid.timeMid, uuid.timeHiAndVersion);
    for (int i = 0; i < 8; i++) {
        printf("%02x", uuid.clockSeqAndNode[i]);
        if (i == 3) printf("-");
    }
    printf("\n");
    /* Initialize the context */
	uint32_t origin;
	TEEC_Result res;

	/* Initialize a context connecting us to the TEE */
	res = TEEC_InitializeContext(NULL, &ctx->ctx);
	if (res != TEEC_SUCCESS)
		errx(1, "TEEC_InitializeContext failed with code 0x%x", res);

	/* Open a session with the TA */
	res = TEEC_OpenSession(&ctx->ctx, &ctx->sess, &uuid,
			       TEEC_LOGIN_PUBLIC, NULL, NULL, &origin);
	if (res != TEEC_SUCCESS)
		errx(1, "TEEC_Opensession failed with code 0x%x origin 0x%x",
			res, origin);
}

void terminate_tee_session(struct test_ctx *ctx)
{
	TEEC_CloseSession(&ctx->sess);
	TEEC_FinalizeContext(&ctx->ctx);
}

TEEC_Result read_secure_object(struct test_ctx *ctx, char *id,
			char *data, size_t data_len)
{
	TEEC_Operation op;
	uint32_t origin;
	TEEC_Result res;
	size_t id_len = strlen(id);

	memset(&op, 0, sizeof(op));
	op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INPUT,
					 TEEC_MEMREF_TEMP_OUTPUT,
					 TEEC_NONE, TEEC_NONE);

	op.params[0].tmpref.buffer = id;
	op.params[0].tmpref.size = id_len;

	op.params[1].tmpref.buffer = data;
	op.params[1].tmpref.size = data_len;

	res = TEEC_InvokeCommand(&ctx->sess,
				 TA_SECURE_STORAGE_CMD_READ_RAW,
				 &op, &origin);
	switch (res) {
	case TEEC_SUCCESS:
	case TEEC_ERROR_SHORT_BUFFER:
	case TEEC_ERROR_ITEM_NOT_FOUND:
		break;
	default:
		printf("Command READ_RAW failed: 0x%x / %u\n", res, origin);
	}

	return res;
}

TEEC_Result write_secure_object(struct test_ctx *ctx, char *id,
			char *data, size_t data_len)
{
	TEEC_Operation op;
	uint32_t origin;
	TEEC_Result res;
	size_t id_len = strlen(id);

	memset(&op, 0, sizeof(op));
	op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INPUT,
					 TEEC_MEMREF_TEMP_INPUT,
					 TEEC_NONE, TEEC_NONE);

	op.params[0].tmpref.buffer = id;
	op.params[0].tmpref.size = id_len;

	op.params[1].tmpref.buffer = data;
	op.params[1].tmpref.size = data_len;

	res = TEEC_InvokeCommand(&ctx->sess,
				 TA_SECURE_STORAGE_CMD_WRITE_RAW,
				 &op, &origin);
	if (res != TEEC_SUCCESS)
		printf("Command WRITE_RAW failed: 0x%x / %u\n", res, origin);

	switch (res) {
	case TEEC_SUCCESS:
		break;
	default:
		printf("Command WRITE_RAW failed: 0x%x / %u\n", res, origin);
	}

	return res;
}

TEEC_Result delete_secure_object(struct test_ctx *ctx, char *id)
{
	TEEC_Operation op;
	uint32_t origin;
	TEEC_Result res;
	size_t id_len = strlen(id);

	memset(&op, 0, sizeof(op));
	op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INPUT,
					 TEEC_NONE, TEEC_NONE, TEEC_NONE);

	op.params[0].tmpref.buffer = id;
	op.params[0].tmpref.size = id_len;

	res = TEEC_InvokeCommand(&ctx->sess,
				 TA_SECURE_STORAGE_CMD_DELETE,
				 &op, &origin);

	switch (res) {
	case TEEC_SUCCESS:
	case TEEC_ERROR_ITEM_NOT_FOUND:
		break;
	default:
		printf("Command DELETE failed: 0x%x / %u\n", res, origin);
	}

	return res;
}

#define TEST_OBJECT_SIZE	7000

int upload_weights() {
    // write weights to secure storage

    struct test_ctx ctx;
	char obj1_id[] = "object#1";		/* string identification for the object */
	char obj2_id[] = "object#2";		/* string identification for the object */
	char obj1_data[TEST_OBJECT_SIZE];
	char read_data[TEST_OBJECT_SIZE];
	TEEC_Result res;

	printf("Prepare session with the TA\n");
	prepare_tee_session(&ctx);

	/*
	 * Create object, read it, delete it.
	 */
	printf("\nTest on object \"%s\"\n", obj1_id);

	printf("- Create and load object in the TA secure storage\n");

	memset(obj1_data, 0xA1, sizeof(obj1_data));

	res = write_secure_object(&ctx, obj1_id,
				  obj1_data, sizeof(obj1_data));
	if (res != TEEC_SUCCESS)
		errx(1, "Failed to create an object in the secure storage");

	printf("- Read back the object\n");

	res = read_secure_object(&ctx, obj1_id,
				 read_data, sizeof(read_data));
	if (res != TEEC_SUCCESS)
		errx(1, "Failed to read an object from the secure storage");
	if (memcmp(obj1_data, read_data, sizeof(obj1_data)))
		errx(1, "Unexpected content found in secure storage");

	printf("- Delete the object\n");

	res = delete_secure_object(&ctx, obj1_id);
	if (res != TEEC_SUCCESS)
		errx(1, "Failed to delete the object: 0x%x", res);

	/*
	 * Non volatile storage: create object2 if not found, delete it if found
	 */
	printf("\nTest on object \"%s\"\n", obj2_id);

	res = read_secure_object(&ctx, obj2_id,
				  read_data, sizeof(read_data));
	if (res != TEEC_SUCCESS && res != TEEC_ERROR_ITEM_NOT_FOUND)
		errx(1, "Unexpected status when reading an object : 0x%x", res);

	if (res == TEEC_ERROR_ITEM_NOT_FOUND) {
		char data[] = "This is data stored in the secure storage.\n";

		printf("- Object not found in TA secure storage, create it.\n");

		res = write_secure_object(&ctx, obj2_id,
					  data, sizeof(data));
		if (res != TEEC_SUCCESS)
			errx(1, "Failed to create/load an object");

	} else if (res == TEEC_SUCCESS) {
		printf("- Object found in TA secure storage, delete it.\n");

		res = delete_secure_object(&ctx, obj2_id);
		if (res != TEEC_SUCCESS)
			errx(1, "Failed to delete an object");
	}

	printf("\nWe're done, close and release TEE resources\n");
	terminate_tee_session(&ctx);
	return 0;
}

int init_runtime() {
    test_multi_linear3();
    return 0;
}

int main(int argc, char **argv) {
    if (argc == 1) {
        printf("type command to initialization\n");
        return 0;
    }

    if (strcmp(argv[1], "gpio") == 0) {
        return gpio_test();
    }

    if (strcmp(argv[1], "upload") == 0) {
        return upload_weights();
    }

    return 0;
}