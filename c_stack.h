#pragma once
#ifndef _CATACLYSM_STACK_H_
#define _CATACLYSM_STACK_H_

#include "c_memory.h"
#include "c_types.h"

typedef struct
{
	///@brief The block of memory dedicated to holding stack entries.
    uint8_t *allocation;
	///@brief The stack pointer. (Points one entry-length ahead of last entry written.)
    uint8_t *cur_location;
    
	///@brief The number of entries capable of being held.
    size_t count;
	///@brief The size of an entry.
    size_t unit_size;
    
	///@brief The number of entries to expand by if the stack fills.
    size_t expansion_unit;
    
} cstack_t;

///@brief Allocate a stack object.
///@param[in] element_count The number of elements in the stack at creation.
///@param[in] element_size The size per element in the stack.
///@param[in] expansion_unit The number of stack entries to be appended if the stack needs to allocate more space.  Set to zero for static stack.
///@return A cstack_t object or NULL on allocation failure.
inline cstack_t* UTIL_createStack(size_t element_count, size_t element_size, size_t expansion_unit)
{
    // Allocate the cstack_t object.
    cstack_t *the_stack = c_malloc(sizeof(cstack_t));
    
    if(the_stack)
    {
        the_stack->count = element_count;
        the_stack->unit_size = element_size;
        the_stack->expansion_unit = expansion_unit;
        
        // Allocate space.
        the_stack->allocation = c_malloc(the_stack->count * the_stack->unit_size);
        
        if(the_stack->allocation)
        {
            the_stack->cur_location = the_stack->allocation;
        }
        else
        {
            // If anything goes wrong, we don't want a memory leak.
            c_free(the_stack);
            the_stack = NULL;
        }
    }
    
    return the_stack;
    
}

///@brief Destroy a stack object and its underlying allocation.
///@param[in] stack The cstack_t object to be destroyed.
inline void UTIL_destroyStack(cstack_t *stack)
{
    c_free(allocation);
    c_free(stack);
}

///@brief Pops an element off of the stack.
///@param[in] stack A pointer to a valid cstack_t object.
///@return A pointer to a popped element. Returns NULL if no more units on stack.
///@note Caller is responsible for element deallocation if stack unit is a pointer.
inline void* UTIL_popFromStack(cstack_t *stack)
{
    // Have we pushed anything?
    if(stack->cur_location = stack->allocation)
        return NULL;
        
    // Move the stack pointer back and return what's there.
    return (stack->cur_location -= stack->unit_size);
}

///@brief Push an element onto the stack.
///@param[in] stack A pointer to a valid cstack_t object.
///@param[in] object A pointer to an object to append to the stack.
///@return Returns true if successful, false if static stack and no more slots are available.
///@note Caller is responsible for ensuring the size of the stack object matches that of the cstack_t.
inline bool UTIL_pushToStack(cstack_t *stack, void *object)
{
    // Check that the stack has room. (optimized to avoid mults)
    if((stack->cur_location - 1) - stack->allocation > 0)
    {
        // Copy the object over.
        memcpy(stack->cur_location, object, stack->unit_size);
        
        // Update stack pointer.
        stack->cur_location += stack->unit_size;
        
        return true;
    }
    else if(expansion_unit)
    {
        // We need to allocate more space.
        size_t new_count = stack->count + stack->expansion_unit;
        
        // Figure out a pointer delta for how to translate cur_location.
        size_t cur_offset = stack->cur_location - stack->allocation;

        // Check to make sure realloc did not fail.
        void *new_alloc = c_realloc(stack->alloc, new_count * stack->unit_size);
        
        if(!new_alloc)
            return false;
        
        // Everything looks good!
        stack->allocation = new_alloc;
        stack->cur_location = new_alloc + cur_offset;
        stack->count = new_count;
        
        return true;
    }
    
    return false;
}

///@brief Determine if stack is empty.
///@param[in] stack A pointer to a cstack_t.
///@retval true The stack is empty.
///@retval false The stack has units.
inline bool UTIL_isStackEmpty(cstack_t *stack)
{
	return stack->cur_location == stack->allocation;
}

#endif //_CATACLYSM_STACK_H_