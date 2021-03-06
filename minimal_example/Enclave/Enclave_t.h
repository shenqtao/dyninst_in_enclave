#ifndef ENCLAVE_T_H__
#define ENCLAVE_T_H__

#include <stdint.h>
#include <wchar.h>
#include <stddef.h>
#include "sgx_edger8r.h" /* for sgx_ocall etc. */

#include "user_types.h"

#include <stdlib.h> /* for size_t */

#define SGX_CAST(type, item) ((type)(item))

#ifdef __cplusplus
extern "C" {
#endif

void Ecall_SomeDataProcessing(char* buf, size_t len);
void Ecall_MaliciousDataProcessing(uint64_t p_inside);

sgx_status_t SGX_CDECL Ocall_PrintString(const char* str);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
