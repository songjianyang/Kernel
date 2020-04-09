#ifndef INCLUDE_STRING_H_
#define INCLUDE_STRING_H_
#include "common.h"

//void memset(char *dest, char val, uint32_t len);
//void memcpy(char *dest,char *src,uint32_t len);
//void strcpy(char *dest,char *src);
//uint32_t strlen(char *src);

static inline void memset(char *dest, char val, uint32_t len)
{
	char *tmp = (char *)dest;
	for (; len != 0; len--)
	{
		*tmp++ = val;
	}
}

static inline void memcpy(char *dest, char *src, uint32_t len)
{
	for (; len > 0; len--)
	{
		*dest++ = *src++;
	}
}

static inline uint32_t strlen(char *src)
{
	uint32_t l = 0;
	while (*src++ != '\x00')
	{
		l++;
	}
	return l;
}
static inline void strcpy(char *dest, char *src)
{

	uint32_t dest_len = strlen(dest);
	uint32_t src_len = strlen(src);
	char *dst = dest + dest_len;

	uint32_t i = 0;
	for (; i <= src_len; i++)
	{
		*dst++ = *src++;
	}
}

static inline int strcmp(const char *str1, const char *str2)
{
	while (*str1 && *str2 && *str1 == *str2)
	{
		str1++;
		str2++;
	}

	return *str1 - *str2;
}

static inline int bzero(void *dest, uint32_t len)
{
	memset(dest, 0, len);
}

#endif // INCLUDE_STRING_H_
