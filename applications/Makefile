# -*- makefile -*-


ARCH ?= rv32gc
ABI ?= ilp32f
# needed for uart.c
CLOCK_MHZ ?= 71
XLEN ?= 32
CMODEL?=medlow

# Must have a / at the end. Can be blank if you just want to use PATH
# Should point to your installation of the RISC-V tools which support the new
# instructions in Clarinet
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
CFLAGS += -lm

# needed for riscv_counters.c
CFLAGS += -DRV$(XLEN)
CFLAGS += $(APP_FLAGS)

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
	$(RM) *.o

.PHONY:full_clean
full_clean: clean
	$(RM) $(MAIN) *.elf *.dump

$(MAIN).o: uart.o startup.o syscalls.o riscv_counters.o clarinet.o

$(MAIN): $(MAIN).o clarinet.o riscv_counters.o

objdump: $(MAIN)
	$(OBJDUMP) -xd $(MAIN) > $(MAIN).dump

