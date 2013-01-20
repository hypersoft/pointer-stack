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

/* set the data limit associated with a PointerStack */
bool pointer_stack_set_limit(PointerStack * stack, size_t limit) {

	if (HavePointerStack) {

		if ( limit && HaveStackData && (stack->units > limit) ) {

			// truncate instruction.
			stack->index = stack->units = limit;

			// shrink wrap it...
			stack->item = pointer_stack_allocator_resize(stack->item, stack->units * sizeof(void *));

		}

		// set the limit
		stack->limit = limit;

		return true;

	}

	return false;

}

