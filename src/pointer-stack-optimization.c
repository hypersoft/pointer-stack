/*
	Copyright (C) 2013, Triston J. Taylor
*/

/* Provides API:

	extern bool   pointer_stack_set_buffering(PointerStack, size_t);
	extern size_t pointer_stack_get_buffering(PointerStack);
 
*/

/* Setup PointerStack's preallocation buffer */
bool pointer_stack_set_buffering(PointerStack * stack, size_t size) {
	if (HavePointerStack) { stack->buffer = size; return true; }
	return false;
}

size_t pointer_stack_get_buffering(PointerStack * stack) {
	if ( ! HavePointerStack) return 0;
	return stack->buffer;	
}
