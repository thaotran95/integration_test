#include <assert.h>
#include <time.h>
#include <stdio.h>

int main(void)
{
    for (;;) {
        struct timespec tp1;
        int ret;
        ret = clock_gettime(CLOCK_MONOTONIC, &tp1);
        assert(ret == 0);
        //ret = clock_gettime(CLOCK_MONOTONIC_RAW, &tp2);
        //assert(ret == 0);
        //printf("Monotonic: %d.%d Raw: %d.%d\n", tp1.tv_sec, tp1.tv_nsec / 1000, tp2.tv_sec, tp2.tv_nsec / 1000);
        printf("Monotonic: %ld.%ld\n", tp1.tv_sec, tp1.tv_nsec / 1000);
    }

    return 0;
}
