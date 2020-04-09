#include "kheap.h"

extern u32 end;
u32 placement_address = (u32)&end;

u32 alloc_address_int(u32 size, int align, u32 *phys)
{
	if (align == 1 && (placement_address & 0xFFFFF000))
	{
		placement_address &= 0xFFFFF000;
		placement_address += 0x1000;
	}
	if (phys)
	{
		*phys = placement_address;
	}
	u32 tmp = placement_address;
	placement_address += size;
	return tmp;
}

u32 alloc_address_a(u32 size)
{

	return alloc_address_int(size, 1, 0);
}

u32 alloc_address_p(u32 size, u32 *phys)
{
	return alloc_address_int(size, 0, phys);
}

u32 alloc_address_ap(u32 size, u32 *phys)
{
	return alloc_address_int(size, 1, phys);
}

u32 alloc_address(u32 size)
{
	return alloc_address_int(size, 0, 0);
}

void init_heap()
{
	heap_first = 0;
}

void *kmalloc(u32 len)
{
	len += sizeof(header_t);

	header_t *cur_header = heap_first;
	header_t *prev_header = 0;

	while (cur_header)
	{

		if (cur_header->allocated == 0 && cur_header->length >= len)
		{

			split_chunk(cur_header, len);
			cur_header->allocated = 1;

			return (void *)((u32)cur_header + sizeof(header_t));
		}
		prev_header = cur_header;
		cur_header = cur_header->next;
	}

	u32 chunk_start;

	if (prev_header)
	{
		chunk_start = (u32)prev_header + prev_header->length;
	}
	else
	{
		chunk_start = HEAP_START;
		heap_first = (header_t *)chunk_start;
	}

	cur_header = (header_t *)chunk_start;
	cur_header->prev = prev_header;
	cur_header->next = 0;
	cur_header->allocated = 1;
	cur_header->length = len;

	if (prev_header)
	{
		prev_header->next = cur_header;
	}

	return (void *)(chunk_start + sizeof(header_t));
}

void kfree(void *p)
{
	header_t *header = (header_t *)((u32)p - sizeof(header_t));
	header->allocated = 0;

	glue_chunk(header);
}

void free_chunk(header_t *chunk)
{
	if (chunk->prev == 0)
	{
		heap_first = 0;
	}
	else
	{
		chunk->prev->next = 0;
	}
}

void split_chunk(header_t *chunk, u32 len)
{
	if (chunk->length - len > sizeof(header_t))
	{
		header_t *newchunk = (header_t *)((u32)chunk + len);
		newchunk->prev = chunk;
		newchunk->next = chunk->next;
		newchunk->allocated = 0;
		newchunk->length = chunk->length - len;

		chunk->next = newchunk;
		chunk->length = len;
	}
}

void glue_chunk(header_t *chunk)
{
	if (chunk->next && chunk->next->allocated == 0)
	{
		chunk->length = chunk->length + chunk->next->length;
		if (chunk->next->next)
		{
			chunk->next->next->prev = chunk;
		}
		chunk->next = chunk->next->next;
	}

	if (chunk->prev && chunk->prev->allocated == 0)
	{
		chunk->prev->length = chunk->prev->length + chunk->length;
		chunk->prev->next = chunk->next;
		if (chunk->next)
		{
			chunk->next->prev = chunk->prev;
		}
		chunk = chunk->prev;
	}
}
