/*
	Copyright (C) 2013, Triston J. Taylor
*/

/* Provides API:

	extern bool    pointer_stack_push(PointerStack, void *);
	extern void *  pointer_stack_pop(PointerStack);
	extern void *  pointer_stack_peek(PointerStack, size_t);
	extern void *  pointer_stack_poke(PointerStack, size_t, void *);
	extern bool    pointer_stack_pack(PointerStack stack);
	extern void ** pointer_stack_pointer(PointerStack, size_t);

*/

#include <stdlib.h> /* malloc, realloc, free */
#include <string.h> /* memset, size_t */
#include <stdbool.h> /* bool */

/* Internal Macros */

// test case macros, assuming stack is type of PointerStack
#define HavePointerStack stack
#define HavePointerStackData stack->item
#define HavePointerStackItem stack->index
#define HavePointerStackSlot stack->units > stack->index
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

static void * PS_ACTION_NULL = (void *)(-1LL);

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

	if ( ! HavePointerStack || pointer == PS_ACTION_NULL || PointerStackIsLocked) return false;

	size_t units = (1 + stack->units);

	if ( ! HavePointerStackData ) {
		units += stack->buffer;
		if ( stack->limit && units > stack->limit ) {
			units -= stack->limit;
			if ( ! units ) return false;
		}
		stack->item = pointer_stack_allocator_lease(units * sizeof(void *));
		stack->units = units;
	}

	if ( ! HavePointerStackSlot ) { 
		units += (stack->buffer);
		if ( stack->limit && units > stack->limit ) {
			units -= stack->limit;
			if ( ! units ) return false;
		}
		stack->item = pointer_stack_allocator_resize(stack->item, units * sizeof(void *));
		stack->units = units;
	}

	stack->item[stack->index++] = pointer;
	return true;

}

/* Pop item off of stack top */
void * pointer_stack_pop(PointerStack * stack) {
	void * pointer = PS_ACTION_NULL;
	if (HavePointerStack && HavePointerStackData && HavePointerStackItem)
		pointer = stack->item[--stack->index];
	return pointer;
}

void * pointer_stack_peek(PointerStack * stack, size_t index) {

	void * pointer = PS_ACTION_NULL;

	if (PointerStackIsInverted) invert_range_item(0, stack->index, &index);

	if (HavePointerStack && HavePointerStackData) {
		if (index < stack->units)
			pointer = stack->item[index];
	}

	return pointer;

}

void * pointer_stack_poke(PointerStack * stack, size_t index, void * pointer) {

	void * result = PS_ACTION_NULL;

	if ( ! HavePointerStack || ! HavePointerStackData || index >= stack->index || pointer == result)
		return result;

	result = stack->item[index]; stack->item[index] = pointer;

	return result;

}

bool pointer_stack_pack(PointerStack * stack) {

	if ( ! HavePointerStack || PointerStackIsLocked ) return false;

	size_t units = (stack->units - stack->index);
	size_t buffer = (stack->buffer + 1);
	
	if (units > buffer || units < buffer) units = buffer;

	if ( stack->limit && units > stack->limit ) {
		units -= stack->limit;
		if ( ! units ) return false;
	}

	if (units == stack->units) return false;	

	stack->item = pointer_stack_allocator_resize(stack->item, units * sizeof(void *));
	stack->units = units;
	return true;

}

void ** pointer_stack_pointer(PointerStack * stack, size_t index) {

	void * pointer = PS_ACTION_NULL;

	if ( ! HavePointerStack || ! HavePointerStackData ) return pointer;

	if (index < stack->index) pointer = (stack->item + index);

	return pointer;

}
