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

	if ( ! ThisPointerStack ) PointerStackFalse(PSE_NO_STACK);
	if ( ! ThisPointerStackData ) PointerStackFail(PSE_NO_STACK_DATA);
	if ( ! ThisPointerStackItem ) PointerStackFail(PSE_STACK_EMPTY);

	PointerStackBless(stack->index);

}

/* Get the count of free slots in a PointerStack */
size_t pointer_stack_get_slots(PointerStack * stack) {

	if ( ! ThisPointerStack ) PointerStackFalse(PSE_NO_STACK);
	if ( ! ThisPointerStackData ) PointerStackFail(PSE_NO_STACK_DATA);

	PointerStackBless(stack->units - stack->index);

}

/* Get the count of allocated units in a PointerStack */
size_t pointer_stack_get_units(PointerStack * stack) {

	if ( ! ThisPointerStack ) PointerStackFalse(PSE_NO_STACK);
	if ( ! ThisPointerStackData ) PointerStackFail(PSE_NO_STACK_DATA);

	PointerStackBless(stack->units);

}

/* Deallocate export storage */
bool pointer_stack_free(PointerStackExport block) {

	if ( ! block ) return false;  
	pointer_stack_allocator_release(block); return true;

}

PointerStackExport pointer_stack_export(PointerStack * stack, size_t from, size_t to) {

	if ( ! ThisPointerStack ) PointerStackNull(PSE_NO_STACK);
	if ( ! ThisPointerStackData ) PointerStackAbort(PSE_NO_STACK_DATA);

	if ( from >= stack->index || to >= stack->index ) PointerStackAbort(PSE_INVALID_RANGE);

	register size_t units = (to - from) + 1;
	void ** export = pointer_stack_allocator_lease((1 + units) * sizeof(void *));
	register size_t index = 0, source = from;

	if ( ! PointerStackIsInverted ) while (units--) export[index++] = stack->item[source++];
	else {
		source = to;
		while (units--) export[index++] = stack->item[source--];
	}

	export[index] = NULL;
	PointerStackReturn(export);

}

bool pointer_stack_import(PointerStack * stack, void * item[], size_t begin, size_t end) {

	if ( ! ThisPointerStack ) PointerStackFalse(PSE_NO_STACK);
	if ( ! item ) PointerStackFail(PSE_INVALID_INPUT);

	register size_t count = 0; while (item[count++]);
	if (begin >= count || end >= count) PointerStackFail(PSE_INVALID_RANGE);

	register size_t new_units = (end - begin) + 1;

	// do we have enough units allocated?
	if ((stack->units - stack->index) < (new_units)) /* no */ {
		if (PointerStackIsLocked) PointerStackFail(PSE_STACK_LOCKED);
		size_t units = (new_units + stack->buffer + stack->index);
		if ( stack->limit && units > stack->limit ) {
			size_t subunits = (units - stack->limit); // rollback
			if( subunits <= stack->buffer ) units -= subunits; // unbuffer
			if (units > stack->limit) PointerStackFail(PSE_STACK_LIMITED); //check
		}
		stack->item = pointer_stack_allocator_resize(stack->item, units * sizeof(void *));
	}

	register size_t index = 0;

	if ( ! PointerStackIsInverted ) {
		count = begin;
		while (new_units--) stack->item[stack->index++] = item[count++];
	} else {
		count = end;
		while (new_units--) stack->item[stack->index++] = item[count--];
	}

	PointerStackSuccess(true);

}

