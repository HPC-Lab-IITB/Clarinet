###  -*-Makefile-*-

# *** DO NOT EDIT! ***
# *** This file is program-generated, not hand-written. ***
# ================================================================

REPO ?= ../..
ARCH ?= RV64ACDFIMSU

# ================================================================
# RISC-V config macros passed into Bluespec 'bsc' compiler

BSC_COMPILATION_FLAGS += \
	-D RV64 \
	-D ISA_PRIV_M  -D ISA_PRIV_U  -D ISA_PRIV_S  \
	-D SV39  \
	-D ISA_I  -D ISA_M  -D ISA_A  -D ISA_F  -D ISA_D  -D ISA_FD_DIV  -D ISA_C  \
	-D SHIFT_BARREL    \
	-D MULT_SYNTH    \
	-D Near_Mem_Caches    \
	-D FABRIC64    \

# Compilation flags and paths for Melodica
BSC_COMPILATION_FLAGS += \
	-D POSIT       	\
	-D QUILLS	\

# For Melodica debug statements
# BSC_COMPILATION_FLAGS += -D RANDOM -D RANDOM_PRINT \

MELODICA_DIRS = $(REPO)/Melodica/src_bsv:$(REPO)/Melodica/src_bsv/Fdp:$(REPO)/Melodica/src_bsv/FtoP:$(REPO)/Melodica/src_bsv/lib:$(REPO)/Melodica/src_bsv/PtoF:$(REPO)/Melodica/src_bsv/PtoQ:$(REPO)/Melodica/src_bsv/QtoP

# Default ISA test

TEST ?= rv64ui-p-add

#================================================================
# Common boilerplate rules

include $(REPO)/builds/Resources/Include_Common.mk

#================================================================
# Makefile rules for building for specific simulator: bluesim

include $(REPO)/builds/Resources/Include_verilator.mk

