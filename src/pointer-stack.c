/*
	Copyright (C) 2013, Triston J. Taylor
*/

/* Provides Internals:

	void * pointer_stack_allocator_lease(size_t size);
	void * pointer_stack_allocator_resize(void * data, size_t size);
	void   pointer_stack_allocator_release(void * data);

	Provides API:

	extern void pointer_stack_initialize_allocation(PointerStackAllocator create, PointerStackAllocator resize, PointerStackDeallocator destroy);

	Note: Items marked with an asterisk have not yet been implemented.
 
*/

#ifndef _STDLIB_H
#include <stdlib.h> /* malloc, realloc, free */
#endif
#ifndef _STRING_H
#include <string.h> /* memset, size_t */
#endif

/* Internal Macros */

// test case macros, assuming stack is type of PointerStack
#define HavePointerStack stack
#define HavePointerStackData stack->item
#define PointerStackIsLocked stack->lock
#define PointerStackIsLimited stack->limit
#define PointerStackIsBuffered stack->buffer
#define PointerStackIsInverted stack->inverted

/* Externally, this is only a (void *) */
typedef struct PointerStack {

	void ** item;	// pointer array pointer
	void * private;	// private data pointer

	size_t index;	// current stack index
	size_t units;	// how many items are allocated for storage
	size_t lock;	// the lock reference count
	size_t limit;	// the maximum number of allocated units
	size_t buffer;	// how many units to preallocate when no space is available
	size_t error;	// the last error code

	bool inverted;	// state of index inversion for peek, poke

} PointerStack;

typedef enum PointerStackError {
	PSE_NO_ERROR = 0,
	PSE_NO_STACK,
	PSE_NO_STACK_DATA,
	PSE_STACK_LOCKED,
	PSE_STACK_LIMITED,
	PSE_OVERFLOW,
} PointerStackError;

/* Externally this is only a (void *) */
typedef void PointerStackExport;

#include "pointer-stack-allocation.c"
#include "pointer-stack-extended.c"
#include "pointer-stack-io.c"
#include "pointer-stack-lifecycle.c"
#include "pointer-stack-limitation.c"
#include "pointer-stack-optimization.c"
#include "pointer-stack-private.c"
#include "pointer-stack-protection.c"

/* internal function to retrieve inverted index */
static bool invert_range_item(size_t lower, size_t upper, size_t * item) {

	size_t value = *item;
	if (lower > upper || value > upper) return false;
	upper -= lower; value -= lower;
	*item = ((upper - value) + lower);
	return true;

}

/* Push item onto stack */
bool pointer_stack_push(PointerStack * stack, void * pointer) {
	stack->item[stack->index++] = pointer;
	return true
}

/* Pop item off of stack top */
void * pointer_stack_pop(PointerStack * stack) {
	return stack->item[--stack->index];
}

