#include <stdio.h>
#include <time.h>

int main() {
    struct timespec res;

    if (clock_getres(CLOCK_REALTIME, &res) == 0) {
        printf("Clock: CLOCK_REALTIME - Resolution: %ld sec, %ld nsec\n", res.tv_sec, res.tv_nsec);
    } else {
        perror("clock_getres");
    }

    if (clock_getres(CLOCK_MONOTONIC, &res) == 0) {
        printf("Clock: CLOCK_MONOTONIC - Resolution: %ld sec, %ld nsec\n", res.tv_sec, res.tv_nsec);
    } else {
        perror("clock_getres");
    }

    if (clock_getres(CLOCK_PROCESS_CPUTIME_ID, &res) == 0) {
        printf("Clock: CLOCK_PROCESS_CPUTIME_ID - Resolution: %ld sec, %ld nsec\n", res.tv_sec, res.tv_nsec);
    } else {
        perror("clock_getres");
    }

    if (clock_getres(CLOCK_THREAD_CPUTIME_ID, &res) == 0) {
        printf("Clock: CLOCK_THREAD_CPUTIME_ID - Resolution: %ld sec, %ld nsec\n", res.tv_sec, res.tv_nsec);
    } else {
        perror("clock_getres");
    }

    return 0;
}