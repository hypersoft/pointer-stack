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
	if ( ! ThisPointerStack ) PointerStackFalse(PSE_NO_STACK);
	PointerStackIsInverted = status;
	PointerStackSuccess(true);
}

/* Retrieve and clear the error value associated with a PointerStack */
size_t pointer_stack_error(PointerStack * stack) {
	register size_t error = PSE_NO_STACK;
	if (ThisPointerStack) {
		error = stack->error;
		stack->error = PSE_NO_ERROR;
	}
	return error;
}

bool pointer_stack_void(PointerStack * stack, size_t count) {

	if ( ! ThisPointerStack ) PointerStackFalse(PSE_NO_STACK);
	if ( ! ThisPointerStackData ) PointerStackFail(PSE_NO_STACK_DATA);
	if ( ! ThisPointerStackItem ) PointerStackFail(PSE_STACK_EMPTY);

	if (count > stack->index) PointerStackFail(PSE_OVERFLOW);
	if ( ! count ) PointerStackFail(PSE_INVALID_INPUT);

	stack->index -= count;

	PointerStackSuccess(true);

}

bool pointer_stack_reverse(PointerStack * stack) {

	if ( ! ThisPointerStack ) PointerStackFalse(PSE_NO_STACK);
	if ( ! ThisPointerStackData ) PointerStackFail(PSE_NO_STACK_DATA);
	if ( ! ThisPointerStackItem ) PointerStackFail(PSE_STACK_EMPTY);

	if ( stack->index == 1 ) PointerStackSuccess(true);

	void * duplicate[stack->index];
	memcpy(duplicate, stack->item, (stack->index * sizeof(void *)));
	register size_t countdown = stack->index;
	register size_t countup = 0;

	while (countdown) stack->item[countup++] = duplicate[--countdown];

	PointerStackSuccess(true);

}

const char * pointer_stack_license(void) { return pointer_stack_license_string; }

