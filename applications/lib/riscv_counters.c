// Copyright (c) 2013 Bluespec, Inc.  All Rights Reserved

#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

// ----------------------------------------------------------------
// For 64-bit:
// The following are interfaces to inline RISC-V assembly instructions
//     RDCYCLE
// For all of them, the result is left in v0 (= x16) per calling convention

// For 32-bit, we get cycle counter out of MCYCLE / MCYCLEH CSRs.

// ----------------------------------------------------------------
// RV32 versions

#ifdef RV32

// We use mcycle / mcycleh instead of rdcycle / rdcycleh on 32-bit
// because Rocket32 (P1) seems not to have rdcycleh
uint32_t  read_cycle (void)
{
     uint32_t hi, lo, tmp;

     __asm__ __volatile__ (
        "1:\n"
        "csrr %0, mcycleh\n"
        "csrr %1, mcycle\n"
        "csrr %2, mcycleh\n"
        "bne %0, %2, 1b"
        : "=&r" (hi), "=&r" (lo), "=&r" (tmp));

     // return ((uint64_t)hi << 32) | lo;
     return (lo);
}

#endif

// ----------------------------------------------------------------
// RV64 versions

#ifdef RV64

uint64_t  read_cycle (void)
{
    uint64_t result;

    asm volatile ("RDCYCLE  %0" : "=r" (result));

    return result;
}

#endif

// ----------------------------------------------------------------
