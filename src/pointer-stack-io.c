/*
	Copyright (C) 2013, Triston J. Taylor
*/

/* Provides API:

	extern size_t pointer_stack_get_count(PointerStack);
	extern size_t pointer_stack_get_slots(PointerStack);
	extern size_t pointer_stack_get_units(PointerStack);
	extern bool   pointer_stack_free(PointerStackExport);

	extern PointerStackExport pointer_stack_export(PointerStack, size_t, size_t);
	extern bool pointer_stack_import(PointerStack, void **, size_t, size_t);
	Note: Items marked with an asterisk have not yet been implemented.
 
*/

// we import this here to avoid having to deal with buffering code that's already been written..
extern bool   pointer_stack_push(PointerStack *, void *);

typedef void ** PointerStackExport;

/* Get the count of elements in a PointerStack */
size_t pointer_stack_get_count(PointerStack * stack) {
	if (HavePointerStack && HavePointerStackData) return stack->index;
	return 0;
}

/* Get the count of free slots in a PointerStack */
size_t pointer_stack_get_slots(PointerStack * stack) {
	if (HavePointerStack && HavePointerStackData) return (stack->units - stack->index);
	return 0;
}

/* Get the count of free slots in a PointerStack */
size_t pointer_stack_get_units(PointerStack * stack) {
	if (HavePointerStack && HavePointerStackData) return (stack->units);
	return 0;
}

/* Deallocate export storage */
bool pointer_stack_free(PointerStackExport block) {
	if (block) { 
		pointer_stack_allocator_release(block); return true;
	} return false;
}

PointerStackExport pointer_stack_export(PointerStack * stack, size_t from, size_t to) {
	if ( ! HavePointerStack || ! HavePointerStackData ) return NULL;
	if ( from >= stack->index || to >= stack->index ) return NULL;
	size_t units = (to - from) + 1;
	void ** export = pointer_stack_allocator_lease((1 + units) * sizeof(void *));
	size_t index = 0;
	while (from <= to) export[index++] = stack->item[from++];
	export[units] = NULL;
	return export;
}

bool pointer_stack_import(PointerStack * stack, void * item[], size_t begin, size_t end) {
	// this could be optimized with a proper temporary buffer setting and some calcs..
	bool result;
	size_t count = 0;
	while (item[count++]);
	if (begin >= count || end >= count) return false;
	size_t units = (end - begin) + 1;
	while (units-- && (result = pointer_stack_push(stack, item[begin++])));
	return result;
}
