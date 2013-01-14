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
const char * pointer_stack_license(void);

void pointer_stack_allocation (
	PointerStackAllocator, PointerStackAllocator, PointerStackDeallocator
);

