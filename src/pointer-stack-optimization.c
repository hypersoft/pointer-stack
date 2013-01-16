/*
	Copyright (C) 2013, Triston J. Taylor
*/

/* Provides API:

	extern bool pointer_stack_set_buffering(PointerStack, size_t);
 
*/

/* Setup PointerStack's preallocation buffer */
bool pointer_stack_set_buffering(PointerStack * stack, size_t size) {
	if (HavePointerStack) { stack->buffer = size; return true; }
	return false;
}

