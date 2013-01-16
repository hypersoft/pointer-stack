/*
	Copyright (C) 2013, Triston J. Taylor
*/

/* Provides API:

	pointer_stack_reverse *
	pointer_stack_invert *
	pointer_stack_void *
	pointer_stack_error
	pointer_stack_license

	Note: Items marked with an asterisk have not yet been implemented.
 
*/

/* control index inversion on a PointerStack */
void pointer_stack_invert(PointerStack * stack, bool status) {
	if (HavePointerStack) stack->inverted = status;
}

/* Retrieve and clear the error value associated with a PointerStack */
unsigned long pointer_stack_error(PointerStack * stack) {
	unsigned long error = PSE_NO_STACK;
	if (HavePointerStack) {
		error = stack->error;
		stack->error = PSE_NO_ERROR;
	}
	return error;
}

#include "include/license.inc"

const char * pointer_stack_license(void) { return pointer_stack_license_string; }

