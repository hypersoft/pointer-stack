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

	typedef void ** PointerStackExport;

*/

typedef void ** PointerStackExport;

/* Get the count of elements in a PointerStack */
size_t pointer_stack_get_count(PointerStack * stack) {
	if (ThisPointerStack && ThisPointerStackData) return stack->index;
	return 0;
}

/* Get the count of free slots in a PointerStack */
size_t pointer_stack_get_slots(PointerStack * stack) {
	if (ThisPointerStack && ThisPointerStackData) return (stack->units - stack->index);
	return 0;
}

/* Get the count of allocated units in a PointerStack */
size_t pointer_stack_get_units(PointerStack * stack) {
	if (ThisPointerStack && ThisPointerStackData) return (stack->units);
	return 0;
}

/* Deallocate export storage */
bool pointer_stack_free(PointerStackExport block) {
	if (block) { 
		pointer_stack_allocator_release(block); return true;
	} return false;
}

PointerStackExport pointer_stack_export(PointerStack * stack, size_t from, size_t to) {
	if ( ! ThisPointerStack || ! ThisPointerStackData ) return NULL;
	if ( from >= stack->index || to >= stack->index ) return NULL;
	size_t units = (to - from) + 1;
	void ** export = pointer_stack_allocator_lease((1 + units) * sizeof(void *));
	size_t index = 0;
	while (from <= to) export[index++] = stack->item[from++];
	export[units] = NULL;
	return export;
}

bool pointer_stack_import(PointerStack * stack, void * item[], size_t begin, size_t end) {

	// This procedure should allow importing general pointer operation sentinel error (-1).

	if ( ! ThisPointerStack || ! item ) return false;

	register size_t count = 0; while (item[count++]);
	if (begin >= count || end >= count) return false;

	register size_t new_units = (end - begin) + 1;

	// do we have enough units allocated?
	if ((stack->units - stack->index) < (new_units)) /* no */ {
		if (PointerStackIsLocked) return false;
		size_t units = (new_units + stack->buffer + stack->index);
		if ( stack->limit && units > stack->limit ) {
			size_t subunits = (units - stack->limit); // rollback
			if( subunits <= stack->buffer ) units -= subunits; // unbuffer
			if (units > stack->limit) return false; //check
		}
		stack->item = pointer_stack_allocator_resize(stack->item, units * sizeof(void *));
	}

	while (new_units--) stack->item[stack->index++] = item[begin++];

	return true;
}

