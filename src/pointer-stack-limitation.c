/*
	Copyright (C) 2013, Triston J. Taylor
*/

/* Provides API:

	extern size_t pointer_stack_get_limit(PointerStack);
	extern bool pointer_stack_set_limit(PointerStack, size_t);
 
*/

/* get the data limit associated with a PointerStack */
size_t pointer_stack_get_limit(PointerStack * stack) {
	if (HavePointerStack) return stack->limit;
	return 0;
}

/* set the private data associated with a PointerStack */
bool pointer_stack_set_limit(PointerStack * stack, size_t limit) {
	if (HavePointerStack) {
		if (limit && stack->buffer > limit) stack->buffer = limit;
		stack->limit = limit;
		return true;
	}
	return false;
}

