#define _GNU_SOURCE
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/wait.h>

int elo = 0;

pid_t gettid(void) {
    return syscall(SYS_gettid);
}

void *threadproc(void *_unused) {
    //fprintf(stdout, "%d\n", elo);
    //fflush(stdout);
    elo++;
    return NULL;
}

#define ILEWONTKOW 1000
#define ILEPETLI 4

static void cr(const char *s) {
    int fd = open(s, O_CREAT | O_EXCL | O_RDWR, 0777);
    if (fd < 0) {
        printf("open of \"%s\" failed: %m\n", s);
        _exit(0);
    }
    close(fd);
//    unlink(s);
}

static volatile int wf = 0;

static void *th(void *x) {
    char buf[0x1000] = { 0 };
    snprintf(buf, sizeof buf, "/tmp/%08x", (unsigned int)(long)x);
    while (!wf);
    cr(buf);
    return x;
}

int main() {
    int status = 0,
        x,
        j, i;
    pthread_t t[ILEPETLI];

    setbuf(stdout, NULL);
    setbuf(stderr, NULL);

    for (j = 0; j < ILEWONTKOW/ILEPETLI; ++j) {
        printf("Loop %d of %d\n", j+1, ILEWONTKOW/ILEPETLI);
        for (i = 0; i < ILEPETLI; ++i) {
            //pthread_create(t+i, NULL, threadproc, NULL);
            pthread_create(t+i, NULL, th, (void*)(long)(j*ILEPETLI + i));
        }
        wf = 1;
        //printf("GUWNY: %d %d\n", getpid(), gettid());
        puts("Joining");
        for (i = 0; i < ILEPETLI; ++i) {
            x = pthread_join(t[i], NULL);
            //printf("Joined: %d\n", x);
            status = 0;
            x = wait(&status);
            //printf("Wait: %d Status: %d\n", x, status);
        }
        wf = 0;
    }

    puts("Done");
    status = 0;
    x = wait(&status);
    printf("Wait: %d Status: %d\n", x, status);
    return 0;
}
