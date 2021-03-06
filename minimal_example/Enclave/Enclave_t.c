#include "Enclave_t.h"

#include "sgx_trts.h" /* for sgx_ocalloc, sgx_is_outside_enclave */
#include "sgx_lfence.h" /* for sgx_lfence */

#include <errno.h>
#include <mbusafecrt.h> /* for memcpy_s etc */
#include <stdlib.h> /* for malloc/free etc */

#define CHECK_REF_POINTER(ptr, siz) do {	\
	if (!(ptr) || ! sgx_is_outside_enclave((ptr), (siz)))	\
		return SGX_ERROR_INVALID_PARAMETER;\
} while (0)

#define CHECK_UNIQUE_POINTER(ptr, siz) do {	\
	if ((ptr) && ! sgx_is_outside_enclave((ptr), (siz)))	\
		return SGX_ERROR_INVALID_PARAMETER;\
} while (0)

#define CHECK_ENCLAVE_POINTER(ptr, siz) do {	\
	if ((ptr) && ! sgx_is_within_enclave((ptr), (siz)))	\
		return SGX_ERROR_INVALID_PARAMETER;\
} while (0)


typedef struct ms_Ecall_SomeDataProcessing_t {
	char* ms_buf;
	size_t ms_len;
} ms_Ecall_SomeDataProcessing_t;

typedef struct ms_Ecall_MaliciousDataProcessing_t {
	uint64_t ms_p_inside;
} ms_Ecall_MaliciousDataProcessing_t;

typedef struct ms_Ocall_PrintString_t {
	const char* ms_str;
} ms_Ocall_PrintString_t;

static sgx_status_t SGX_CDECL sgx_Ecall_SomeDataProcessing(void* pms)
{
	CHECK_REF_POINTER(pms, sizeof(ms_Ecall_SomeDataProcessing_t));
	//
	// fence after pointer checks
	//
	sgx_lfence();
	ms_Ecall_SomeDataProcessing_t* ms = SGX_CAST(ms_Ecall_SomeDataProcessing_t*, pms);
	sgx_status_t status = SGX_SUCCESS;
	char* _tmp_buf = ms->ms_buf;
	size_t _tmp_len = ms->ms_len;
	size_t _len_buf = _tmp_len;
	char* _in_buf = NULL;

	CHECK_UNIQUE_POINTER(_tmp_buf, _len_buf);

	//
	// fence after pointer checks
	//
	sgx_lfence();

	if (_tmp_buf != NULL && _len_buf != 0) {
		if ((_in_buf = (char*)malloc(_len_buf)) == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		memset((void*)_in_buf, 0, _len_buf);
	}

	Ecall_SomeDataProcessing(_in_buf, _tmp_len);
err:
	if (_in_buf) {
		if (memcpy_s(_tmp_buf, _len_buf, _in_buf, _len_buf)) {
			status = SGX_ERROR_UNEXPECTED;
		}
		free(_in_buf);
	}

	return status;
}

static sgx_status_t SGX_CDECL sgx_Ecall_MaliciousDataProcessing(void* pms)
{
	CHECK_REF_POINTER(pms, sizeof(ms_Ecall_MaliciousDataProcessing_t));
	//
	// fence after pointer checks
	//
	sgx_lfence();
	ms_Ecall_MaliciousDataProcessing_t* ms = SGX_CAST(ms_Ecall_MaliciousDataProcessing_t*, pms);
	sgx_status_t status = SGX_SUCCESS;



	Ecall_MaliciousDataProcessing(ms->ms_p_inside);


	return status;
}

SGX_EXTERNC const struct {
	size_t nr_ecall;
	struct {void* ecall_addr; uint8_t is_priv;} ecall_table[2];
} g_ecall_table = {
	2,
	{
		{(void*)(uintptr_t)sgx_Ecall_SomeDataProcessing, 0},
		{(void*)(uintptr_t)sgx_Ecall_MaliciousDataProcessing, 0},
	}
};

SGX_EXTERNC const struct {
	size_t nr_ocall;
	uint8_t entry_table[1][2];
} g_dyn_entry_table = {
	1,
	{
		{0, 0, },
	}
};


sgx_status_t SGX_CDECL Ocall_PrintString(const char* str)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_str = str ? strlen(str) + 1 : 0;

	ms_Ocall_PrintString_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_Ocall_PrintString_t);
	void *__tmp = NULL;


	CHECK_ENCLAVE_POINTER(str, _len_str);

	ocalloc_size += (str != NULL) ? _len_str : 0;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_Ocall_PrintString_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_Ocall_PrintString_t));
	ocalloc_size -= sizeof(ms_Ocall_PrintString_t);

	if (str != NULL) {
		ms->ms_str = (const char*)__tmp;
		if (memcpy_s(__tmp, ocalloc_size, str, _len_str)) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		__tmp = (void *)((size_t)__tmp + _len_str);
		ocalloc_size -= _len_str;
	} else {
		ms->ms_str = NULL;
	}
	
	status = sgx_ocall(0, ms);

	if (status == SGX_SUCCESS) {
	}
	sgx_ocfree();
	return status;
}

