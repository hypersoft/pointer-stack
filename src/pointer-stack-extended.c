/*
	Copyright (C) 2013, Triston J. Taylor
*/

/* Provides API:

	extern bool   pointer_stack_reverse(PointerStack);
	extern bool   pointer_stack_void(PointerStack, size_t);
	extern bool   pointer_stack_invert(PointerStack, bool);
	extern size_t pointer_stack_error(PointerStack);
	extern char * pointer_stack_license(void);
 
*/

#include "include/license.inc"

/* control index inversion on a PointerStack */
bool pointer_stack_invert(PointerStack * stack, bool status) {
	if ( ! HavePointerStack ) return false;
	stack->inverted = status;
	return true;
}

/* Retrieve and clear the error value associated with a PointerStack */
size_t pointer_stack_error(PointerStack * stack) {
	size_t error = PSE_NO_STACK;
	if (HavePointerStack) {
		error = stack->error;
		stack->error = PSE_NO_ERROR;
	}
	return error;
}

bool pointer_stack_void(PointerStack * stack, size_t count) {
	if ( ! HavePointerStack || ! HavePointerStackData ) return false;
	if (count <= stack->index) {
		stack->index -= count;
		return true;
	}
	return false;
}

bool pointer_stack_reverse(PointerStack * stack) {

	if ( ! HavePointerStack || ! HavePointerStackData ) return false;
	if ( stack->index == 1 ) return true;

	void * duplicate[stack->index];
	memcpy(duplicate, stack->item, (stack->index * sizeof(void *)));
	size_t countdown = stack->item;
	size_t countup = 0;

	while (countdown) stack->item[countup++] = duplicate[--countdown];

	return true;
}

const char * pointer_stack_license(void) { return pointer_stack_license_string; }

