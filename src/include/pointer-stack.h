/*
	Copyright (C) 2013, Triston J. Taylor
*/

/* Platinum Rule: DO NOT SHARE STACKS AMONG THREADS. */

/* Golden Rule: INITIALIZE STACK ALLOCATOR ONCE, BEFORE ANY ALLOCATIONS */

/* Silver Rule: DO NOT USE LIFECYLE FUNCTIONS FROM WITHIN THREADS */

#ifndef POINTER_STACK_H

	#define POINTER_STACK_H

	#ifndef _STRING_H
		#include <string.h> /* size_t */
	#endif

	/* Operation Context */
	typedef void * PointerStack;

	typedef enum PointerStackError {
		PSE_NO_ERROR = 0,
		PSE_NO_STACK,
		PSE_NO_STACK_DATA,
		PSE_STACK_LOCKED,
		PSE_STACK_LIMITED,
		PSE_OVERFLOW,
	} PointerStackError;

	/* Internally Allocated Export Data */
	typedef void * PointerStackExport;

	/* Global Allocation Routines */
	typedef void * ( * PointerStackAllocator   ) ( size_t );
	typedef void   ( * PointerStackDeallocator ) ( void * );

	/* API Procedures */

	/* main */

	/* allocation */
	extern void pointer_stack_initialize_allocation(PointerStackAllocator create, PointerStackAllocator resize, PointerStackDeallocator destroy);

	/* extended */
	
	/* I/O */

	/* lifecycle */
	extern PointerStack pointer_stack_create(void);
	extern bool pointer_stack_dispose(PointerStack);

	/* limitations */
	extern size_t pointer_stack_get_limit(PointerStack);
	extern bool pointer_stack_set_limit(PointerStack, size_t);

	/* optimization */
	extern bool pointer_stack_set_buffering(PointerStack, size_t);

	/* private */
	extern void * pointer_stack_get_private(PointerStack);
	extern bool pointer_stack_set_private(PointerStack, void *);

	/* protection */
	extern bool   pointer_stack_unlock(PointerStack * stack);
	extern bool   pointer_stack_lock(PointerStack * stack);
	extern size_t pointer_stack_get_lock(PointerStack * stack);

#endif /* POINTER_STACK_H */
