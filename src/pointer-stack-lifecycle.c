/*
	Copyright (C) 2013, Triston J. Taylor
*/

/* Provides API:

	extern PointerStack pointer_stack_create(void);
	extern bool pointer_stack_dispose(PointerStack);
 
*/

/* Create a new PointerStack */
PointerStack * pointer_stack_create(void) {

	void * result = pointer_stack_allocator_lease(sizeof(PointerStack));

	if (result) {
		memset(result, 0, sizeof(PointerStack));
		result->buffer = 8;
	}

	return result;

}

/* Destroy a PointerStack */
bool pointer_stack_dispose(PointerStack * stack) {

	if (HavePointerStack) {
		if (HavePointerStackData) pointer_stack_allocator_release(stack->item);
		pointer_stack_allocator_release(stack);
		return true;
	}

	return false;

}

