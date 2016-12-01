#pragma once
#ifndef _CATACLYSM_MEMORY_H_
#define _CATACLYSM_MEMORY_H_

#include "c_types.h"

void* c_malloc(size_t len);

void c_free(void *addr);

void* c_realloc(void *addr, size_t len);

#endif //_CATACLYSM_MEMORY_H_