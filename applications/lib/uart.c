
#include <stdint.h>

#define DEFAULT_BAUDRATE  (115200)

#define NS16550_BASE (0xC0000000ULL)

#define NS16550_CLOCK_RATE  (1000000ULL * CLOCK_MHZ)

enum __attribute__ ((__packed__)) ier_t {
    IER_ERBI = (1<<0),
    IER_ETBEI = (1<<1),
    IER_ELSI = (1<<2),
    IER_EDSSI = (1<<3),
    IER_SLEEP = (1<<4),  // 16650 only
};

enum __attribute__ ((__packed__)) iir_t {
    IIR_IP = 1,
    IIR_NONE = 1,
    IIR_RLSI = 6,
    IIR_RDAI = 4,
    IIR_CTOI = 0xc,
    IIR_THRI = 2,
    IIR_MSRI = 0,
    IIR_F64E = 0x20,
    IIR_FE = 0xc0,
};

enum __attribute__ ((__packed__)) fcr_t {
    FCR_FE = 1,
    FCR_RXFR = 2,
    FCR_TXFR = 4,
    FCR_DMS = 8,
    FCR_F64E = 0x20,
    FCR_RT1 = 0,
    FCR_RT4 = 0x40,
    FCR_RT8 = 0x80,
    FCR_RT14 = 0xc0,
    FCR_RT16 = 0x40,
    FCR_RT32 = 0x80,
    FCR_RT56 = 0xc0,
};

enum __attribute__ ((__packed__)) lcr_t {
    LCR_WLS5 = 0,
    LCR_WLS6 = 1,
    LCR_WLS7 = 2,
    LCR_WLS8 = 3,
    LCR_STB = 4,
    LCR_PEN = 8,
    LCR_EPS = 0x10,
    LCR_SP = 0x20,
    LCR_BC = 0x40,
    LCR_DLAB = 0x80,
};

enum __attribute__ ((__packed__)) mcr_t {
    MCR_DTR = 1,
    MCR_RTS = 2,
    MCR_OUT1 = 4,
    MCR_OUT2 = 8,
    MCR_LOOP = 0x10,
    MCR_AFE = 0x20,
};

enum __attribute__ ((__packed__)) lsr_t {
    LSR_DR = 1,
    LSR_OE = 2,
    LSR_PE = 4,
    LSR_FE = 8,
    LSR_BI = 0x10,
    LSR_THRE = 0x20,
    LSR_TEMT = 0x40,
    LSR_RXFE = 0x80,
};

enum __attribute__ ((__packed__)) msr_t {
    MSR_DCTS = 1,
    MSR_DDSR = 2,
    MSR_TERI = 4,
    MSR_DDCD = 8,
    MSR_CTS = 0x10,
    MSR_DSR = 0x20,
    MSR_RI = 0x40,
    MSR_DCD = 0x80,
};

struct __attribute__ ((aligned (4))) ns16550_pio {
    // 0x000
    union __attribute__ ((aligned (4))) {
	const volatile uint8_t rbr;
	volatile uint8_t thr;
	volatile uint8_t dll;
    };

    // 0x008
    union __attribute__ ((aligned (4))) {
	volatile uint8_t dlm;
	volatile enum ier_t ier;
    };

    // 0x010
    union __attribute__ ((aligned (4))) {
	const volatile enum iir_t iir;
	volatile enum fcr_t fcr;
    };

    // 0x018
    volatile enum lcr_t lcr __attribute__ ((aligned (4)));

    // 0x020
    volatile enum mcr_t mcr __attribute__ ((aligned (4)));

    // 0x028
    volatile enum lsr_t lsr __attribute__ ((aligned (4)));

    // 0x030
    volatile uint8_t msr __attribute__ ((aligned (4)));

    // 0x038
    volatile uint8_t scr __attribute__ ((aligned (4)));
};


static struct ns16550_pio * pio = (void*)NS16550_BASE;

__attribute__ ((constructor))
static int ns16550_init(void) {
    uint32_t divisor;

    pio->ier = 0;

    divisor = NS16550_CLOCK_RATE / (16 * DEFAULT_BAUDRATE);
    pio->lcr |= LCR_DLAB;
    pio->dll = divisor & 0xff;
    pio->dlm = (divisor >> 8) & 0xff;
    pio->lcr &= ~LCR_DLAB;

    pio->lcr = LCR_WLS8;
    pio->fcr = FCR_FE;
    pio->mcr = MCR_RTS;

    return 0;
}

int ns16550_rxready(void) {
    return ((pio->lsr & LSR_DR) != 0);
}

int ns16550_rxchar(void) {
    while ((pio->lsr & LSR_DR) == 0)
	;  // nothing

    return pio->rbr;
}

int ns16550_txchar(int c) {
    while ((pio->lsr & LSR_THRE) == 0)
	;  // nothing

    pio->thr = c;

    return c;
}

void ns16550_flush(void) {
    while ((pio->lsr & LSR_TEMT) == 0)
	;  // nothing
}

int _read(int file, char *ptr, int len) {
    int count = 0;

    while (ns16550_rxready() && (count < len)) {
	*ptr++ = ns16550_rxchar();
	count++;
    }

    return count;
}

int _write(int file, char *ptr, int len) {
    int todo;

    for (todo = 0; todo < len; todo++) {
	ns16550_txchar (*ptr++);
    }

    return len;
}
