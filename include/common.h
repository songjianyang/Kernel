#ifndef COMMON
#define COMMON

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned long uint32_t;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

void outb(uint16_t port, uint8_t value);
uint8_t inb(uint16_t port);
uint16_t inw(uint16_t port);

#endif
