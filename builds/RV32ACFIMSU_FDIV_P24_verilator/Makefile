###  -*-Makefile-*-

# *** DO NOT EDIT! ***
# *** This file is program-generated, not hand-written. ***
# ================================================================

REPO ?= ../..
ARCH ?= RV32ACFIMSU

# ================================================================
# RISC-V config macros passed into Bluespec 'bsc' compiler

BSC_COMPILATION_FLAGS += \
	-D RV32 \
	-D ISA_PRIV_M  -D ISA_PRIV_U  -D ISA_PRIV_S  \
	-D SV32  \
	-D ISA_I  -D ISA_M  -D ISA_A  -D ISA_F  -D ISA_C  \
	-D INCLUDE_FDIV \
	-D INCLUDE_FSQRT \
	-D SHIFT_BARREL    \
	-D MULT_SYNTH    \
	-D Near_Mem_Caches    \
	-D FABRIC64    \
	-D WATCH_TOHOST    \

# Compilation flags and paths for Melodica
BSC_COMPILATION_FLAGS += \
	-D POSIT       	\
	-D QUILLS	\
	-D INCLUDE_PDIV \

# For Melodica debug statements
# BSC_COMPILATION_FLAGS += -D RANDOM -D RANDOM_PRINT \

# paths to melodica bsv code
CUSTOM_DIRS = ./src_bsv:$(REPO)/Melodica/src_bsv:$(REPO)/Melodica/src_bsv/common:$(REPO)/Melodica/src_bsv/lib:$(REPO)/Melodica/src_bsv/FtoP:$(REPO)/Melodica/src_bsv/Fused_Op:$(REPO)/Melodica/src_bsv/PtoF:$(REPO)/Melodica/src_bsv/PtoQ:$(REPO)/Melodica/src_bsv/QtoP

# Default ISA test

TEST ?= rv32ui-p-add

#================================================================
# Common boilerplate rules

include $(REPO)/builds/Resources/Include_Common.mk

#================================================================
# Makefile rules for building for specific simulator: bluesim

include $(REPO)/builds/Resources/Include_verilator.mk

