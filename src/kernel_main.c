#include <stdint.h>

#define MULTIBOOT2_HEADER_MAGIC 0xe85250d6
#define COM1 0x3F8

/* This header lets GRUB recognize the kernel as multiboot2. */
const unsigned int multiboot_header[] __attribute__((section(".multiboot"))) = {
    MULTIBOOT2_HEADER_MAGIC,
    0,
    24,
    -(MULTIBOOT2_HEADER_MAGIC + 24),
    0,
    8
};

/* Write one byte to an I/O port. */
static inline void outb(uint16_t port, uint8_t value)
{
    __asm__ __volatile__("outb %0, %1" : : "a"(value), "Nd"(port));
}

/* Read one byte from an I/O port. */
static inline uint8_t inb(uint16_t port)
{
    uint8_t value;
    __asm__ __volatile__("inb %1, %0" : "=a"(value) : "Nd"(port));
    return value;
}

/* Set up COM1 so QEMU can show serial output in the terminal. */
static void serial_init(void)
{
    outb(COM1 + 1, 0x00);
    outb(COM1 + 3, 0x80);
    outb(COM1 + 0, 0x03);
    outb(COM1 + 1, 0x00);
    outb(COM1 + 3, 0x03);
    outb(COM1 + 2, 0xC7);
    outb(COM1 + 4, 0x0B);
}

/* Wait until the serial port is ready to send. */
static int serial_is_ready(void)
{
    return inb(COM1 + 5) & 0x20;
}

/* Send one character through COM1. */
static void serial_putc(char c)
{
    while (serial_is_ready() == 0)
    {
    }

    outb(COM1, (uint8_t)c);
}

/* Print a string through COM1. */
static void serial_print(const char *str)
{
    int i = 0;

    while (str[i] != '\0')
    {
        if (str[i] == '\n')
        {
            serial_putc('\r');
        }

        serial_putc(str[i]);
        i = i + 1;
    }
}

void main(void)
{
    serial_init();

    /* This confirms the kernel started running. */
    serial_print("COMP 310 project booted successfully.\n");

    while (1)
    {
    }
}