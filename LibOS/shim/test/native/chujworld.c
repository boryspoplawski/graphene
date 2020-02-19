#define _GNU_SOURCE
#include <dirent.h>
#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <sys/sendfile.h>

#define ENDPATH "/tmp/ende"

#define WON() return 0

void waitforend(void) {
    int fd;
    while (1) {
        fd = open(ENDPATH, 0);
        if (fd >= 0) {
            close(fd);
            return;
        }
        sleep(1);
    }
}

void dirread(const char *dname) {
    DIR *dp;
    struct dirent *de;

    dp = opendir(dname);
    if (!dp) {
        printf("cant open dir: %m\n");
        return;
    }

    while (1) {
        errno = 0;
        de = readdir(dp);
        if (!de) {
            if (errno) {
                printf("ERROR: readdir: %s\n", strerror(errno));
                break;
            } else {
                break;
            }
        }
        printf("GOT: %s\n", de->d_name);
    }
    closedir(dp);
}

static void eee(void) {
    int fd = open("/proc/cpuinfo", O_RDONLY);
    char c;
    if (fd < 0) {
        printf("ERROR: %m\n");
        return;
    }
    while (read(fd, &c, 1) > 0) {
        write(1, &c, 1);
    }
    write(1, "\n", 1);
    close(fd);
}

int main(int argc, char *argv[]) {
    int status;
    pid_t p;
    ssize_t ign;

    setbuf(stdout, NULL);
    setbuf(stderr, NULL);

    eee();
    return 0;

    p = fork();
    if (p < 0) {
        puts("FAIL!!!!");
        return 1;
    } else if (p == 0) {
        //sleep(1);
        printf("CHILD: %p\n", argv[0]);
        int fd = open("/proc/self/maps", O_RDONLY);
        while (1) {
            char c;
            if (read(fd, &c, 1) != 1) {
                break;
            }
            ign = write(1, &c, 1);
        }
        ign = write(1, "\n", 1);
        (void)ign;
        return 0;
    }
    puts("PARENT");
    if (wait4(p, &status, 0, NULL) < 0) {
        puts("wait");
        return 1;
    }
    puts("ENDE");
    return 0;
}
