#include "c_memory.h"

#include "stdlib.h"

inline void* c_malloc(size_t len)
{
	return malloc(len);
}

inline void c_free(void *addr)
{
	free(addr);
}

inline void* c_realloc(void *addr, size_t len)
{
	return realloc(addr,len);
}