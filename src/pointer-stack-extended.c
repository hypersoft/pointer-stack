/*
	Copyright (C) 2013, Triston J. Taylor
*/

/* Provides API:

	pointer_stack_reverse *
	pointer_stack_void *
	extern void   pointer_stack_invert(PointerStack, bool);
	extern size_t pointer_stack_error(PointerStack);
	extern char * pointer_stack_license(void);

	Note: Items marked with an asterisk have not yet been implemented.
 
*/

#include "include/license.inc"

/* control index inversion on a PointerStack */
void pointer_stack_invert(PointerStack * stack, bool status) {
	if (HavePointerStack) stack->inverted = status;
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

const char * pointer_stack_license(void) { return pointer_stack_license_string; }

