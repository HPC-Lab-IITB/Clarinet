# -*- makefile -*-


ARCH ?= rv32imafc
ifeq ($(ARCH),)
$(error ERROR: need to specify ARCH.  Consider using riscvexplorer-findcore)
endif

ABI ?= ilp32f
ifeq ($(ABI),)
$(error ERROR: need to specify ABI.  Consider using riscvexplorer-findcore)
endif

# needed for uart.c
CLOCK_MHZ ?= 71
ifeq ($(CLOCK_MHZ),)
$(error ERROR: need to specify CLOCK_MHZ.  Consider using riscvexplorer-findcore)
endif

XLEN?=32
ifeq ($(XLEN),)
$(error ERROR: need to specify XLEN.  Consider using riscvexplorer-findcore)
endif

CMODEL?=medlow
ifeq ($(CMODEL),)
$(error ERROR: need to specify CMODEL.  Consider using riscvexplorer-findcore)
endif

# Must have a / at the end
# Can be blank if you just want to use PATH
TOOLCHAIN?=/home/nsharma/x-tools/riscv32-unknown-elf/bin/

CROSS_COMPILE?=riscv$(XLEN)-unknown-elf-

AR=$(TOOLCHAIN)$(CROSS_COMPILE)ar
CC=$(TOOLCHAIN)$(CROSS_COMPILE)gcc
CXX=$(TOOLCHAIN)$(CROSS_COMPILE)g++
LD=$(TOOLCHAIN)$(CROSS_COMPILE)ld
OBJCOPY=$(TOOLCHAIN)$(CROSS_COMPILE)objcopy
OBJDUMP=$(TOOLCHAIN)$(CROSS_COMPILE)objdump
STRIP=$(TOOLCHAIN)$(CROSS_COMPILE)strip

ASFLAGS += -march=$(ARCH) -mabi=$(ABI)

CFLAGS += -march=$(ARCH) -mabi=$(ABI)
CFLAGS += -specs=bare.specs
CFLAGS += -g
CFLAGS += -DCLOCK_MHZ=$(CLOCK_MHZ)
CFLAGS += -mcmodel=$(CMODEL)
CFLAGS += -I../lib

# needed for riscv_counters.c
CFLAGS += -DRV$(XLEN)


LDLIBS += -lm

LDFLAGS += -march=$(ARCH) -mabi=$(ABI)
LDFLAGS += -Wl,-e,_reset
LDFLAGS += -Wl,-Tbare.lds
LDFLAGS += -Wl,-Ttext-segment=0x80000000
LDFLAGS += -specs=bare.specs

VPATH = ../lib

.PHONY: default
default:
	@echo
	@echo no default target, try \'make help\'
	@echo
	@false

.PHONY: help
help:
	@echo
	@echo "Usage:"
	@echo "    make all      # compile exectable 'main'"
	@echo "    make clean    # remove executable and object files"
	@echo "    make sim      # run executable under simulation"
	@echo
	@echo "Use the program 'riscvexplorer-findcore' to select a core and"
	@echo "set environment variables needed for this Makefile."
	@echo

MAIN ?= main

.PHONY: all
all: $(MAIN) objdump

.PHONY:clean
clean:
	$(RM) $(MAIN) *.o

$(MAIN).o: uart.o startup.o syscalls.o riscv_counters.o clarinet.o

$(MAIN): $(MAIN).o clarinet.o riscv_counters.o

objdump: $(MAIN)
	$(OBJDUMP) -D $(MAIN) > $(MAIN).dump

