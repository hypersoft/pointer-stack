/*
	Copyright (C) 2013, Triston J. Taylor
*/

/* Golden Rule: DO NOT SHARE STACKS AMONG THREADS. */

/* Platinum Rule: DO NOT CALL LIBRARY MANAGEMENT FUNCTIONS FROM AMONG THREADS. THIS
   INCLUDES STACK ALLOCATION, BUT DOES NOT INCLUDE STACK_{PUSH,POP,ETC}
*/

#ifndef _STDLIB_H
#include <stdlib.h> /* malloc, realloc, free */
#endif
#ifndef _STRING_H
#include <string.h> /* memset */
#endif

static typedef void * ( * PointerStackAllocator  ) ( unsigned long );
static typedef void   ( * PointerStackDeallocator) ( void *        );

static PointerStackAllocator 	allocate 	= &malloc;
static PointerStackAllocator 	reallocate 	= &realloc;
static PointerStackDeallocator 	deallocate 	= &free;

static unsigned long stacks = 0; /* bool/refcount: disable init? */

/* Externally, this is only a (void *) */
static typedef struct PointerStack {

	void ** item;
	void * private;

	unsigned long index;
	unsigned long units;
	unsigned long count;
	unsigned long lock;
	unsigned long limit;
	unsigned long buffer;
	unsigned long error;

	bool inverted;

} PointerStack;

/* Start with the easy stuff */

/* internal function to retrieve inverted index */
static bool invert_range_item(unsigned long lower, unsigned long upper, unsigned long * item) {

	unsigned long value = *item;
	if (lower > upper || value > upper) return false;
	upper -= lower; value -= lower;
	*item = ((upper - value) + lower);
	return true;

}

/* Retrieve and clear the error value associated with a PointerStack */
unsigned long pointer_stack_error(PointerStack * stack) {
	unsigned long error = stack->error; stack->error = 0;
	return error;
}

/* Setup PointerStack's preallocation buffer */
void pointer_stack_set_buffering(PointerStack * stack, unsigned long value) {
	stack->buffer = value;
}

/* deallocate random storage */
void pointer_stack_free(void * data) {
	if (data) {
		deallocate(data);
		stacks--;
	}
}

/* control index inversion on a PointerStack */
void pointer_stack_invert(PointerStack * stack, bool status) {
	stack->inverted = status;
}

/* decrement the reference counted lock on a PointerStack */
bool pointer_stack_unlock(PointerStack * stack) {
	stack->lock--;
	return ( ! stack->lock);
}

/* increment the reference counted lock on a PointerStack */
bool pointer_stack_lock(PointerStack * stack) {
	stack->lock++;
	return true;
}

/* get the count of elements in a PointerStack */
unsigned long pointer_stack_get_count(PointerStack * stack) {
	return stack->count;
}

/* get the private data associated with a PointerStack */
void * pointer_stack_get_private(PointerStack * stack) {
	return stack->private;
}

/* set the private data associated with a PointerStack */
void pointer_stack_set_private(PointerStack * stack, void * private) {
	stack->private = private;
}

/* get the data limit associated with a PointerStack */
unsigned long pointer_stack_get_limit(PointerStack * stack) {
	return stack->limit;
}

/* set the private data associated with a PointerStack */
bool pointer_stack_set_limit(PointerStack * stack, unsigned long limit) {
	stack->limit = limit;
	return true;
}

/* get the pointer to the pointer of an element in a PointerStack */
void * pointer_stack_pointer(PointerStack * stack, unsigned long index) {
	return stack->item + index;
}

/* swap a value in a PointerStack out with new data */
void * pointer_stack_poke(PointerStack * stack, unsigned long index, void * insert) {
	void * result = stack->item[index];
	stack->item[index] = insert;
	return result;
}

/* Examine a value in a PointerStack */
void * pointer_stack_peek(PointerStack * stack, unsigned long index) {
	return stack->item[index];
}

/* Create a new PointerStack */
PointerStack * pointer_stack_create(void) {
	void * result = allocate(sizeof(PointerStack));
	if (result) { memset(result, 0, sizeof(PointerStack)); stacks++; }
	result->buffer = 8;
	return result;
}

/* Destroy a PointerStack */
bool pointer_stack_dispose(PointerStack * stack) {
	if (stack->lock || ! stack) return false;
	pointer_stack_free(stack->item);
	deallocate(stack); stacks--;
	return true;
}

/* This is not a thread safe operation. Avoid use wherever possible */
void pointer_stack_initialize(PointerStackAllocator create, PointerStackAllocator resize, PointerStackDeallocator destroy) {


	if (stacks) return; // can't change allocators or deallocators after allocation.

	/* one might argue that these are tied together.. We are not arguing that */

	if (create) allocate = create;
	if (resize) reallocate = resize;
	if (destroy) deallocate = destroy;

	/* sink or swim! */

}

/* auto-generated binary include: "LICENSE.md"

encoded by bin2inc (C) 2012 Hypersoft bash-masters
git it here: https://github.com/hypersoft/bash-masters

content-length: 1318L

*/

// Manually added by Triston J. Taylor. This implement, requires a static identifier.

static unsigned char pointer_stack_license_string[] = {

    0x48, 0x79, 0x70, 0x65, 0x72, 0x73, 0x6f, 0x66, 0x74, 0x20, 0x50, 0x6f, 0x69, 0x6e, 
    0x74, 0x65, 0x72, 0x53, 0x74, 0x61, 0x63, 0x6b, 0x0a, 0x0a, 0x43, 0x6f, 0x70, 0x79, 
    0x72, 0x69, 0x67, 0x68, 0x74, 0x20, 0x28, 0x63, 0x29, 0x20, 0x32, 0x30, 0x31, 0x33, 
    0x2c, 0x20, 0x54, 0x72, 0x69, 0x73, 0x74, 0x6f, 0x6e, 0x20, 0x4a, 0x2e, 0x20, 0x54, 
    0x61, 0x79, 0x6c, 0x6f, 0x72, 0x0a, 0x0a, 0x41, 0x6c, 0x6c, 0x20, 0x72, 0x69, 0x67, 
    0x68, 0x74, 0x73, 0x20, 0x72, 0x65, 0x73, 0x65, 0x72, 0x76, 0x65, 0x64, 0x2e, 0x0a, 
    0x0a, 0x52, 0x65, 0x64, 0x69, 0x73, 0x74, 0x72, 0x69, 0x62, 0x75, 0x74, 0x69, 0x6f, 
    0x6e, 0x20, 0x61, 0x6e, 0x64, 0x20, 0x75, 0x73, 0x65, 0x20, 0x69, 0x6e, 0x20, 0x73, 
    0x6f, 0x75, 0x72, 0x63, 0x65, 0x20, 0x61, 0x6e, 0x64, 0x20, 0x62, 0x69, 0x6e, 0x61, 
    0x72, 0x79, 0x20, 0x66, 0x6f, 0x72, 0x6d, 0x73, 0x2c, 0x20, 0x77, 0x69, 0x74, 0x68, 
    0x20, 0x6f, 0x72, 0x20, 0x77, 0x69, 0x74, 0x68, 0x6f, 0x75, 0x74, 0x20, 0x6d, 0x6f, 
    0x64, 0x69, 0x66, 0x69, 0x63, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x2c, 0x20, 0x61, 0x72, 
    0x65, 0x20, 0x70, 0x65, 0x72, 0x6d, 0x69, 0x74, 0x74, 0x65, 0x64, 0x20, 0x70, 0x72, 
    0x6f, 0x76, 0x69, 0x64, 0x65, 0x64, 0x20, 0x74, 0x68, 0x61, 0x74, 0x20, 0x74, 0x68, 
    0x65, 0x20, 0x66, 0x6f, 0x6c, 0x6c, 0x6f, 0x77, 0x69, 0x6e, 0x67, 0x20, 0x63, 0x6f, 
    0x6e, 0x64, 0x69, 0x74, 0x69, 0x6f, 0x6e, 0x73, 0x20, 0x61, 0x72, 0x65, 0x20, 0x6d, 
    0x65, 0x74, 0x3a, 0x0a, 0x0a, 0x2a, 0x20, 0x52, 0x65, 0x64, 0x69, 0x73, 0x74, 0x72, 
    0x69, 0x62, 0x75, 0x74, 0x69, 0x6f, 0x6e, 0x73, 0x20, 0x6f, 0x66, 0x20, 0x73, 0x6f, 
    0x75, 0x72, 0x63, 0x65, 0x20, 0x63, 0x6f, 0x64, 0x65, 0x20, 0x6d, 0x75, 0x73, 0x74, 
    0x20, 0x72, 0x65, 0x74, 0x61, 0x69, 0x6e, 0x20, 0x74, 0x68, 0x65, 0x20, 0x61, 0x62, 
    0x6f, 0x76, 0x65, 0x20, 0x63, 0x6f, 0x70, 0x79, 0x72, 0x69, 0x67, 0x68, 0x74, 0x20, 
    0x6e, 0x6f, 0x74, 0x69, 0x63, 0x65, 0x2c, 0x20, 0x74, 0x68, 0x69, 0x73, 0x20, 0x6c, 
    0x69, 0x73, 0x74, 0x20, 0x6f, 0x66, 0x20, 0x63, 0x6f, 0x6e, 0x64, 0x69, 0x74, 0x69, 
    0x6f, 0x6e, 0x73, 0x20, 0x61, 0x6e, 0x64, 0x20, 0x74, 0x68, 0x65, 0x20, 0x66, 0x6f, 
    0x6c, 0x6c, 0x6f, 0x77, 0x69, 0x6e, 0x67, 0x20, 0x64, 0x69, 0x73, 0x63, 0x6c, 0x61, 
    0x69, 0x6d, 0x65, 0x72, 0x2e, 0x0a, 0x2a, 0x20, 0x52, 0x65, 0x64, 0x69, 0x73, 0x74, 
    0x72, 0x69, 0x62, 0x75, 0x74, 0x69, 0x6f, 0x6e, 0x73, 0x20, 0x69, 0x6e, 0x20, 0x62, 
    0x69, 0x6e, 0x61, 0x72, 0x79, 0x20, 0x66, 0x6f, 0x72, 0x6d, 0x20, 0x6d, 0x75, 0x73, 
    0x74, 0x20, 0x72, 0x65, 0x70, 0x72, 0x6f, 0x64, 0x75, 0x63, 0x65, 0x20, 0x74, 0x68, 
    0x65, 0x20, 0x61, 0x62, 0x6f, 0x76, 0x65, 0x20, 0x63, 0x6f, 0x70, 0x79, 0x72, 0x69, 
    0x67, 0x68, 0x74, 0x20, 0x6e, 0x6f, 0x74, 0x69, 0x63, 0x65, 0x2c, 0x20, 0x74, 0x68, 
    0x69, 0x73, 0x20, 0x6c, 0x69, 0x73, 0x74, 0x20, 0x6f, 0x66, 0x20, 0x63, 0x6f, 0x6e, 
    0x64, 0x69, 0x74, 0x69, 0x6f, 0x6e, 0x73, 0x20, 0x61, 0x6e, 0x64, 0x20, 0x74, 0x68, 
    0x65, 0x20, 0x66, 0x6f, 0x6c, 0x6c, 0x6f, 0x77, 0x69, 0x6e, 0x67, 0x20, 0x64, 0x69, 
    0x73, 0x63, 0x6c, 0x61, 0x69, 0x6d, 0x65, 0x72, 0x20, 0x69, 0x6e, 0x20, 0x74, 0x68, 
    0x65, 0x20, 0x64, 0x6f, 0x63, 0x75, 0x6d, 0x65, 0x6e, 0x74, 0x61, 0x74, 0x69, 0x6f, 
    0x6e, 0x20, 0x61, 0x6e, 0x64, 0x2f, 0x6f, 0x72, 0x20, 0x6f, 0x74, 0x68, 0x65, 0x72, 
    0x20, 0x6d, 0x61, 0x74, 0x65, 0x72, 0x69, 0x61, 0x6c, 0x73, 0x20, 0x70, 0x72, 0x6f, 
    0x76, 0x69, 0x64, 0x65, 0x64, 0x20, 0x77, 0x69, 0x74, 0x68, 0x20, 0x74, 0x68, 0x65, 
    0x20, 0x64, 0x69, 0x73, 0x74, 0x72, 0x69, 0x62, 0x75, 0x74, 0x69, 0x6f, 0x6e, 0x2e, 
    0x0a, 0x0a, 0x54, 0x48, 0x49, 0x53, 0x20, 0x53, 0x4f, 0x46, 0x54, 0x57, 0x41, 0x52, 
    0x45, 0x20, 0x49, 0x53, 0x20, 0x50, 0x52, 0x4f, 0x56, 0x49, 0x44, 0x45, 0x44, 0x20, 
    0x42, 0x59, 0x20, 0x54, 0x48, 0x45, 0x20, 0x43, 0x4f, 0x50, 0x59, 0x52, 0x49, 0x47, 
    0x48, 0x54, 0x20, 0x48, 0x4f, 0x4c, 0x44, 0x45, 0x52, 0x53, 0x20, 0x41, 0x4e, 0x44, 
    0x20, 0x43, 0x4f, 0x4e, 0x54, 0x52, 0x49, 0x42, 0x55, 0x54, 0x4f, 0x52, 0x53, 0x20, 
    0x22, 0x41, 0x53, 0x20, 0x49, 0x53, 0x22, 0x20, 0x41, 0x4e, 0x44, 0x20, 0x41, 0x4e, 
    0x59, 0x20, 0x45, 0x58, 0x50, 0x52, 0x45, 0x53, 0x53, 0x20, 0x4f, 0x52, 0x20, 0x49, 
    0x4d, 0x50, 0x4c, 0x49, 0x45, 0x44, 0x20, 0x57, 0x41, 0x52, 0x52, 0x41, 0x4e, 0x54, 
    0x49, 0x45, 0x53, 0x2c, 0x20, 0x49, 0x4e, 0x43, 0x4c, 0x55, 0x44, 0x49, 0x4e, 0x47, 
    0x2c, 0x20, 0x42, 0x55, 0x54, 0x20, 0x4e, 0x4f, 0x54, 0x20, 0x4c, 0x49, 0x4d, 0x49, 
    0x54, 0x45, 0x44, 0x20, 0x54, 0x4f, 0x2c, 0x20, 0x54, 0x48, 0x45, 0x20, 0x49, 0x4d, 
    0x50, 0x4c, 0x49, 0x45, 0x44, 0x20, 0x57, 0x41, 0x52, 0x52, 0x41, 0x4e, 0x54, 0x49, 
    0x45, 0x53, 0x20, 0x4f, 0x46, 0x20, 0x4d, 0x45, 0x52, 0x43, 0x48, 0x41, 0x4e, 0x54, 
    0x41, 0x42, 0x49, 0x4c, 0x49, 0x54, 0x59, 0x20, 0x41, 0x4e, 0x44, 0x20, 0x46, 0x49, 
    0x54, 0x4e, 0x45, 0x53, 0x53, 0x20, 0x46, 0x4f, 0x52, 0x20, 0x41, 0x20, 0x50, 0x41, 
    0x52, 0x54, 0x49, 0x43, 0x55, 0x4c, 0x41, 0x52, 0x20, 0x50, 0x55, 0x52, 0x50, 0x4f, 
    0x53, 0x45, 0x20, 0x41, 0x52, 0x45, 0x20, 0x44, 0x49, 0x53, 0x43, 0x4c, 0x41, 0x49, 
    0x4d, 0x45, 0x44, 0x2e, 0x20, 0x49, 0x4e, 0x20, 0x4e, 0x4f, 0x20, 0x45, 0x56, 0x45, 
    0x4e, 0x54, 0x20, 0x53, 0x48, 0x41, 0x4c, 0x4c, 0x20, 0x54, 0x48, 0x45, 0x20, 0x43, 
    0x4f, 0x50, 0x59, 0x52, 0x49, 0x47, 0x48, 0x54, 0x20, 0x48, 0x4f, 0x4c, 0x44, 0x45, 
    0x52, 0x20, 0x4f, 0x52, 0x20, 0x43, 0x4f, 0x4e, 0x54, 0x52, 0x49, 0x42, 0x55, 0x54, 
    0x4f, 0x52, 0x53, 0x20, 0x42, 0x45, 0x20, 0x4c, 0x49, 0x41, 0x42, 0x4c, 0x45, 0x20, 
    0x46, 0x4f, 0x52, 0x20, 0x41, 0x4e, 0x59, 0x20, 0x44, 0x49, 0x52, 0x45, 0x43, 0x54, 
    0x2c, 0x20, 0x49, 0x4e, 0x44, 0x49, 0x52, 0x45, 0x43, 0x54, 0x2c, 0x20, 0x49, 0x4e, 
    0x43, 0x49, 0x44, 0x45, 0x4e, 0x54, 0x41, 0x4c, 0x2c, 0x20, 0x53, 0x50, 0x45, 0x43, 
    0x49, 0x41, 0x4c, 0x2c, 0x20, 0x45, 0x58, 0x45, 0x4d, 0x50, 0x4c, 0x41, 0x52, 0x59, 
    0x2c, 0x20, 0x4f, 0x52, 0x20, 0x43, 0x4f, 0x4e, 0x53, 0x45, 0x51, 0x55, 0x45, 0x4e, 
    0x54, 0x49, 0x41, 0x4c, 0x20, 0x44, 0x41, 0x4d, 0x41, 0x47, 0x45, 0x53, 0x20, 0x28, 
    0x49, 0x4e, 0x43, 0x4c, 0x55, 0x44, 0x49, 0x4e, 0x47, 0x2c, 0x20, 0x42, 0x55, 0x54, 
    0x20, 0x4e, 0x4f, 0x54, 0x20, 0x4c, 0x49, 0x4d, 0x49, 0x54, 0x45, 0x44, 0x20, 0x54, 
    0x4f, 0x2c, 0x20, 0x50, 0x52, 0x4f, 0x43, 0x55, 0x52, 0x45, 0x4d, 0x45, 0x4e, 0x54, 
    0x20, 0x4f, 0x46, 0x20, 0x53, 0x55, 0x42, 0x53, 0x54, 0x49, 0x54, 0x55, 0x54, 0x45, 
    0x20, 0x47, 0x4f, 0x4f, 0x44, 0x53, 0x20, 0x4f, 0x52, 0x20, 0x53, 0x45, 0x52, 0x56, 
    0x49, 0x43, 0x45, 0x53, 0x3b, 0x20, 0x4c, 0x4f, 0x53, 0x53, 0x20, 0x4f, 0x46, 0x20, 
    0x55, 0x53, 0x45, 0x2c, 0x20, 0x44, 0x41, 0x54, 0x41, 0x2c, 0x20, 0x4f, 0x52, 0x20, 
    0x50, 0x52, 0x4f, 0x46, 0x49, 0x54, 0x53, 0x3b, 0x20, 0x4f, 0x52, 0x20, 0x42, 0x55, 
    0x53, 0x49, 0x4e, 0x45, 0x53, 0x53, 0x20, 0x49, 0x4e, 0x54, 0x45, 0x52, 0x52, 0x55, 
    0x50, 0x54, 0x49, 0x4f, 0x4e, 0x29, 0x20, 0x48, 0x4f, 0x57, 0x45, 0x56, 0x45, 0x52, 
    0x20, 0x43, 0x41, 0x55, 0x53, 0x45, 0x44, 0x20, 0x41, 0x4e, 0x44, 0x20, 0x4f, 0x4e, 
    0x20, 0x41, 0x4e, 0x59, 0x20, 0x54, 0x48, 0x45, 0x4f, 0x52, 0x59, 0x20, 0x4f, 0x46, 
    0x20, 0x4c, 0x49, 0x41, 0x42, 0x49, 0x4c, 0x49, 0x54, 0x59, 0x2c, 0x20, 0x57, 0x48, 
    0x45, 0x54, 0x48, 0x45, 0x52, 0x20, 0x49, 0x4e, 0x20, 0x43, 0x4f, 0x4e, 0x54, 0x52, 
    0x41, 0x43, 0x54, 0x2c, 0x20, 0x53, 0x54, 0x52, 0x49, 0x43, 0x54, 0x20, 0x4c, 0x49, 
    0x41, 0x42, 0x49, 0x4c, 0x49, 0x54, 0x59, 0x2c, 0x20, 0x4f, 0x52, 0x20, 0x54, 0x4f, 
    0x52, 0x54, 0x20, 0x28, 0x49, 0x4e, 0x43, 0x4c, 0x55, 0x44, 0x49, 0x4e, 0x47, 0x20, 
    0x4e, 0x45, 0x47, 0x4c, 0x49, 0x47, 0x45, 0x4e, 0x43, 0x45, 0x20, 0x4f, 0x52, 0x20, 
    0x4f, 0x54, 0x48, 0x45, 0x52, 0x57, 0x49, 0x53, 0x45, 0x29, 0x20, 0x41, 0x52, 0x49, 
    0x53, 0x49, 0x4e, 0x47, 0x20, 0x49, 0x4e, 0x20, 0x41, 0x4e, 0x59, 0x20, 0x57, 0x41, 
    0x59, 0x20, 0x4f, 0x55, 0x54, 0x20, 0x4f, 0x46, 0x20, 0x54, 0x48, 0x45, 0x20, 0x55, 
    0x53, 0x45, 0x20, 0x4f, 0x46, 0x20, 0x54, 0x48, 0x49, 0x53, 0x20, 0x53, 0x4f, 0x46, 
    0x54, 0x57, 0x41, 0x52, 0x45, 0x2c, 0x20, 0x45, 0x56, 0x45, 0x4e, 0x20, 0x49, 0x46, 
    0x20, 0x41, 0x44, 0x56, 0x49, 0x53, 0x45, 0x44, 0x20, 0x4f, 0x46, 0x20, 0x54, 0x48, 
    0x45, 0x20, 0x50, 0x4f, 0x53, 0x53, 0x49, 0x42, 0x49, 0x4c, 0x49, 0x54, 0x59, 0x20, 
    0x4f, 0x46, 0x20, 0x53, 0x55, 0x43, 0x48, 0x20, 0x44, 0x41, 0x4d, 0x41, 0x47, 0x45, 
    0x2e, 0x0a, 0x00

};

const char * pointer_stack_license(void) { return pointer_stack_license_string; }

