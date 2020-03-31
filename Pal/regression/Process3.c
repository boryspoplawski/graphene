#include "api.h"
#include "pal.h"
#include "pal_debug.h"

int main(int argc, char** argv, char** envp) {
    PAL_STR args[1] = {0};

    // Hack to differentiate parent from child
    if (argc == 1) {
        PAL_HANDLE child = DkProcessCreate(NULL, args);
        if (!child) {
            pal_printf("Failed to create new process!\n");
            return 1;
        }

        pal_printf("Create Process without Executable OK\n");
        PAL_BOL ret;
        do {
            ret = DkSynchronizationObjectWait(child, NO_TIMEOUT);
        } while (!ret && (PAL_NATIVE_ERRNO == PAL_ERROR_INTERRUPTED || PAL_NATIVE_ERRNO == PAL_ERROR_TRYAGAIN));

        if (!ret) {
            pal_printf("Waiting for child resulted in error %s", pal_strerror(PAL_NATIVE_ERRNO));
            return 1;
        }
    }

    return 0;
}
