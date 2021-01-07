// Copyright (c) 2013 Bluespec, Inc.  All Rights Reserved

#pragma once

// ----------------------------------------------------------------
// For 64-bit:
// The following are interfaces to inline RISC-V assembly instructions
//     RDCYCLE
// For all of them, the result is left in v0 (= x16) per calling convention

// For 32-bit, we get cycle counter out of MCYCLE / MCYCLEH CSRs,
// because some processors, e.g., Rocket32p, do not implement
// RDCYCLEH.

#ifdef RV32
extern uint32_t  read_cycle    (void);    // RDCYCLE
#endif
#ifdef RV64
extern uint64_t  read_cycle    (void);    // RDCYCLE
#endif
