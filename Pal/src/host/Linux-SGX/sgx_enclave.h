#include "pal_linux.h"
#include "pal_security.h"

int ecall_enclave_start(char* libpal_uri, char* args, size_t args_size, char* env, size_t env_size, void* fun_ptr);

int ecall_thread_start(void* x);

int ecall_thread_reset(void);
