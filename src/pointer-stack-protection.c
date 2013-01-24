/*
	Copyright (C) 2013, Triston J. Taylor
*/

/* Provides API:

	extern bool   pointer_stack_unlock(PointerStack);
	extern bool   pointer_stack_lock(PointerStack);
	extern size_t pointer_stack_get_lock(PointerStack);

*/

/* Decrement the reference counted lock on a PointerStack */
bool pointer_stack_unlock(PointerStack * stack) {
	if ( ! ThisPointerStack ) PointerStackFalse(PSE_NO_STACK);
	if (PointerStackIsLocked) stack->lock--;
	PointerStackBless( stack->lock == 0 );
}

/* Increment the reference counted lock on a PointerStack */
bool pointer_stack_lock(PointerStack * stack) {
	if ( ! ThisPointerStack ) PointerStackFalse(PSE_NO_STACK);
	stack->lock++;
	PointerStackSuccess(true);
}

/* Get the reference count (locks) on a PointerStack */
size_t pointer_stack_get_lock(PointerStack * stack) {
	if ( ! ThisPointerStack ) PointerStackFalse(PSE_NO_STACK);
	PointerStackBless(stack->lock);
}

