#include <stdio.h>

//for read_cycle
#include "riscv_counters.h"

// for PRIu64
#include <inttypes.h>

int main (int argc, char ** argv)
{
    uint64_t start=read_cycle();
    printf("Hello World!\n");
    uint64_t end=read_cycle();
    printf("read_cycle: %" PRIu64 " cycles have elapsed.\n",end-start);
    return 0;
}
