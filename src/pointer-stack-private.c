/*
	Copyright (C) 2013, Triston J. Taylor
*/

/* Provides API:

	extern void * pointer_stack_get_private(PointerStack);
	extern bool pointer_stack_set_private(PointerStack, void *);

*/

/* get the private data associated with a PointerStack */
void * pointer_stack_get_private(PointerStack * stack) {
	if (HavePointerStack) return stack->private;
	return NULL;
}

/* set the private data associated with a PointerStack */
bool pointer_stack_set_private(PointerStack * stack, void * private) {
	if (HavePointerStack) { stack->private = private; return true; }
	return false;
}

