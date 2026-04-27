#include "terminal.h"
#include <stdint.h>

#define COM1 0x3F8

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

/* Set up COM1 for serial output. */
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

/* Check if the serial port can send data. */
static int serial_is_ready(void)
{
    return inb(COM1 + 5) & 0x20;
}

/* This runs once before the first print. */
static void terminal_init(void)
{
    static int initialized = 0;

    if (initialized == 0)
    {
        serial_init();
        initialized = 1;
    }
}

/* Print one character. */
void putc(char c)
{
    terminal_init();

    while (serial_is_ready() == 0)
    {
    }

    outb(COM1, (uint8_t)c);
}

/* Print a string. */
void puts(const char *str)
{
    int i = 0;

    terminal_init();

    while (str[i] != '\0')
    {
        if (str[i] == '\n')
        {
            putc('\r');
        }

        putc(str[i]);
        i = i + 1;
    }
}

/* Check if the serial port has data to read. */
static int serial_has_data(void)
{
    return inb(COM1 + 5) & 0x01;
}

/* Read one character from serial input. */
char getc(void)
{
    terminal_init();

    while (serial_has_data() == 0)
    {
    }

    return (char)inb(COM1);
}
