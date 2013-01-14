/*
	Copyright (C) 2013, Triston J. Taylor
*/

/* Golden Rule: DO NOT SHARE STACKS AMONG THREADS. */
/* Platinum Rule: DO NOT CALL LIBRARY MANAGEMENT FUNCTIONS FROM AMONG THREADS */

#ifndef _STDLIB_H
#include <stdlib.h> /* malloc, realloc, free */
#endif
#ifndef _STRING_H
#include <string.h> /* memset */
#endif


/* Operation Context */
typedef void * PointerStack;


/* Global Allocation Routines */
typedef void * ( * PointerStackAllocator   ) ( unsigned long );
typedef void   ( * PointerStackDeallocator ) ( void *        );


/* API Procedures */

// Returns a string to the license data distributed with this library originally.
extern const char * pointer_stack_license(void);

extern void pointer_stack_allocation (
	PointerStackAllocator, PointerStackAllocator, PointerStackDeallocator
);

/* Retrieve and clear the error value associated with a PointerStack */
extern unsigned long pointer_stack_error(PointerStack);

/* Setup PointerStack's preallocation buffer */
extern void pointer_stack_buffering(PointerStack, unsigned long);

/* deallocate storage */
extern void pointer_stack_free(void);

/* control index inversion on a PointerStack */
extern void pointer_stack_invert(PointerStack, bool);

/* decrement the reference counted lock on a PointerStack */
extern bool pointer_stack_unlock(PointerStack);

/* increment the reference counted lock on a PointerStack */
extern bool pointer_stack_lock(PointerStack);

/* Get the reference count (locks) on a PointerStack */
unsigned long pointer_stack_get_lock(PointerStack * stack)

/* get the count of elements in a PointerStack */
extern unsigned long pointer_stack_get_count(PointerStack);

/* get the private data associated with a PointerStack */
extern void * pointer_stack_get_private(PointerStack);

/* set the private data associated with a PointerStack */
extern void pointer_stack_set_private(PointerStack, void *);

/* get the data limit associated with a PointerStack */
extern unsigned long pointer_stack_get_limit(PointerStack);

/* set the private data associated with a PointerStack */
void pointer_stack_set_limit(PointerStack, unsigned long);

/* get the pointer to the pointer of an element in a PointerStack */
extern void * pointer_stack_pointer(PointerStack, unsigned long);

/* swap a value in a PointerStack out with new data */
extern void * pointer_stack_poke(PointerStack, unsigned long, void *);

/* Examine a value in a PointerStack */
extern void * pointer_stack_peek(PointerStack, unsigned long);

/* Create a new PointerStack */
PointerStack pointer_stack_create(void);

/* Destroy a PointerStack */
bool pointer_stack_dispose(PointerStack);


