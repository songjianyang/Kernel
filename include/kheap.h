#ifndef KHEAP
#define KHEAP

#include "common.h"

u32 alloc_address_int(u32 size, int align, u32 *phys);

u32 alloc_address_a(u32 size);

u32 alloc_address_p(u32 size, u32 *phys);

u32 alloc_address_ap(u32 size, u32 *phys);

u32 alloc_address(u32 size);

#define HEAP_START 0x200000

typedef struct header
{
	struct header *prev;
	struct header *next;
	u32 allocated : 1;
	u32 length : 31;
} header_t;

void init_heap();

void *kmalloc(u32 len);

void kfree(void *p);

void test_heap();

void free_chunk(header_t *chunk);
void split_chunk(header_t *chunk, u32 len);
void glue_chunk(header_t *chunk);

header_t *heap_first;

#endif
