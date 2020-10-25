#include <errno.h>
#include <sys/stat.h>
#include <sys/times.h>
#include <sys/time.h>

#ifndef __linux__

__attribute__ ((weak))
int _read(int file, char *ptr, int len) {
    errno = ENOSYS;
    return -1;
}

__attribute__ ((weak))
int _write(int file, char *ptr, int len) {
    errno = ENOSYS;
    return -1;
}


#define CLOCK_PERIOD  (100000000)

int gettimeofday(struct timeval *ptimeval, void *ptimezone) {

    if (ptimeval) {
	long long tv;
#if __riscv_xlen == 64
	asm ("rdtime %0" : "=r" (tv));
#else
	unsigned int tvh;
	unsigned int tvl;
	asm ("rdtime %0;"
	     "rdtimeh %1 " : "=r" (tvl), "=r" (tvh));
	tv = ((long long)tvh) << 32 | tvl;
#endif
	ptimeval->tv_sec = tv / CLOCK_PERIOD;
	ptimeval->tv_usec = tv % CLOCK_PERIOD / (CLOCK_PERIOD / 1000000);
    }

    return 0;
}

unsigned int sleep(unsigned int seconds) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    seconds += tv.tv_sec;

    while (tv.tv_sec < seconds)
	gettimeofday(&tv, NULL);

    return 0;
}

void *_sbrk (int nbytes) {
    errno = ENOMEM;
    return (void *)-1;
}

int _close(int fd) {
    errno = ENOSYS;
    return -1;
}

long _lseek(int fd, long offset, int origin ) {
    errno = ENOSYS;
    return -1;
}

int _fstat(int fd, struct stat *buffer) {
    errno = ENOSYS;
    return -1;
}

int _isatty(int fd) {
    errno = ENOSYS;
    return 0;
}

/* Use the "tohost" test-exit signal to end the program */
void exit_via_tohost(void) __attribute__ ((destructor));
void exit_via_tohost(void)
{
  asm volatile ("li a0, 0x1;"
                "sw a0, tohost, t0");
}

#endif
