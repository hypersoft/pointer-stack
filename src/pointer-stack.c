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

// test case macros, assuming stack is type of struct -> PointerStack
#define ThisPointerStack stack
#define ThisPointerStackData stack->item
#define ThisPointerStackItem stack->index
#define ThisPointerStackHasSlot (stack->units > stack->index)
#define PointerStackIsLocked stack->lock
#define PointerStackIsLimited stack->limit
#define PointerStackIsBuffered stack->buffer
#define PointerStackIsInverted stack->inverted
#define PointerStackSuccess(value) return (stack->error = PSE_NO_ERROR) + (size_t) true
#define PointerStackBless(value) return (stack->error = PSE_NO_ERROR) + (size_t) value
#define PointerStackFalse(CODE) { return false; }
#define PointerStackFail(CODE) { stack->error = CODE; return false; }
#define PointerStackNull(CODE) return NULL
#define PointerStackAbort(CODE) { stack->error = CODE; return NULL; }
#define PointerStackReturn(POINTER) stack->error = PSE_NO_ERROR; return (POINTER)

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
	PSE_STACK_EMPTY,
	PSE_STACK_LOCKED,
	PSE_STACK_LIMITED,
	PSE_OVERFLOW,
	PSE_INVALID_INPUT,
} PointerStackError;

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

	register size_t value = *item;
	if (lower > upper || value > upper) return false;
	upper -= lower; value -= lower;
	*item = ((upper - value) + lower);
	return true;

}

/* Push item onto stack */
bool pointer_stack_push(PointerStack * stack, void * pointer) {

	if ( ! ThisPointerStack ) PointerStackFalse(PSE_NO_STACK);

	register size_t units = (1 + stack->units);

	if ( ! ThisPointerStackHasSlot || ! ThisPointerStackData ) { 
	 	if (PointerStackIsLocked) PointerStackFail(PSE_STACK_LOCKED);
		units += (stack->buffer);
		if ( PointerStackIsLimited && units > PointerStackIsLimited ) {
			size_t subunits = (units - PointerStackIsLimited); // rollback
			if( subunits <= stack->buffer ) units -= subunits; // unbuffer
			if (units > PointerStackIsLimited) PointerStackFail(PSE_OVERFLOW); //check
		}
		ThisPointerStackData = pointer_stack_allocator_resize(ThisPointerStackData, units * sizeof(void *));
		stack->units = units;
	}

	ThisPointerStackData[stack->index++] = pointer;

	PointerStackSuccess(true);

}

/* Pop item off of stack top */
void * pointer_stack_pop(PointerStack * stack) {

	if ( ! ThisPointerStack ) PointerStackNull(PSE_NO_STACK);
	if ( ! ThisPointerStackData ) PointerStackAbort(PSE_NO_STACK_DATA);
	if ( ! ThisPointerStackItem ) PointerStackAbort(PSE_STACK_EMPTY);

	PointerStackReturn(ThisPointerStackData[--stack->index]);

}

/* Have a look, see... */
void * pointer_stack_peek(PointerStack * stack, size_t index) {

	if ( ! ThisPointerStack ) PointerStackNull(PSE_NO_STACK);
	if ( ! ThisPointerStackData ) PointerStackAbort(PSE_NO_STACK_DATA);
	if ( ! ThisPointerStackItem ) PointerStackAbort(PSE_STACK_EMPTY);

	if (PointerStackIsInverted) 
		if ( ! invert_range_item(0, stack->index - 1, &index) )
			PointerStackAbort(PSE_OVERFLOW);

	if (index >= stack->index) PointerStackAbort(PSE_OVERFLOW);

	PointerStackReturn(ThisPointerStackData[index]);

}

/* Swap meet, dealers choice */
void * pointer_stack_poke(PointerStack * stack, size_t index, void * pointer) {

	if ( ! ThisPointerStack ) PointerStackNull(PSE_NO_STACK);
	if ( ! ThisPointerStackData ) PointerStackAbort(PSE_NO_STACK_DATA);
	if ( ! ThisPointerStackItem ) PointerStackAbort(PSE_STACK_EMPTY);

	if (PointerStackIsInverted)
		if ( ! invert_range_item(0, stack->index - 1, &index) ) 
			PointerStackAbort(PSE_OVERFLOW);

	if (index >= stack->index) PointerStackAbort(PSE_OVERFLOW);

	register void * result = ThisPointerStackData[index];
	ThisPointerStackData[index] = pointer;

	PointerStackReturn(result);

}

/* Sign, deliver, stack package */
bool pointer_stack_pack(PointerStack * stack) {

	if ( ! ThisPointerStack ) PointerStackFalse(PSE_NO_STACK);
	if ( PointerStackIsLocked ) PointerStackFail(PSE_STACK_LOCKED);

	register size_t units = (stack->units - stack->index);
	register size_t buffer = (stack->buffer + 1);
	
	if (units > buffer || units < buffer) units = buffer;

	if ( PointerStackIsLimited && units > PointerStackIsLimited ) {
		size_t subunits = (units - PointerStackIsLimited); // rollback
		if( subunits <= stack->buffer ) units -= subunits; // unbuffer
		if (units > PointerStackIsLimited) PointerStackFail(PSE_STACK_LIMITED); //check
	}

	if (units == stack->units) PointerStackFail(PSE_INVALID_INPUT);	

	ThisPointerStackData = pointer_stack_allocator_resize(ThisPointerStackData, units * sizeof(void *));
	stack->units = units;

	PointerStackSuccess(true);

}

/* get unit location */
void ** pointer_stack_pointer(PointerStack * stack, size_t index) {

	if ( ! ThisPointerStack ) PointerStackNull(PSE_NO_STACK);
	if ( ! ThisPointerStackData ) PointerStackAbort(PSE_NO_STACK_DATA);
	if ( ! ThisPointerStackItem ) PointerStackAbort(PSE_STACK_EMPTY);
	if (index >= stack->index) PointerStackAbort(PSE_OVERFLOW);

	PointerStackReturn(ThisPointerStackData + index);

}
