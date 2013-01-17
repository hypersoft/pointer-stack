/*
	Copyright (C) 2013, Triston J. Taylor
*/

/* Provides API:

	extern size_t pointer_stack_get_count(PointerStack);
	extern bool   pointer_stack_free(PointerStackExport);

	extern PointerStackExport pointer_stack_export *
	extern bool pointer_stack_import *

	Note: Items marked with an asterisk have not yet been implemented.
 
*/

typedef void PointerStackExport;

/* Get the count of elements in a PointerStack */
size_t pointer_stack_get_count(PointerStack * stack) {
	if (HavePointerStack && HavePointerStackData) return stack->index;
	return 0;
}

/* Deallocate export storage */
bool pointer_stack_free(PointerStackExport * block) {
	if (block) { 
		pointer_stack_release(block); return true;
	} return false;
}

