SECTIONS
{
    __startup_save = .;
    .startup 0x80000000: { KEEP(startup.o(.text)) }
    . = 0x88000000;
    .tohost : { *(.tohost) }
    . = __startup_save;
}
INSERT AFTER .bss;
INPUT(syscalls.o)
INPUT(uart.o)
