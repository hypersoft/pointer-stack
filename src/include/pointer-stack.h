/*
	Copyright (C) 2013, Triston J. Taylor
*/

/* Platinum Rule: DO NOT SHARE STACKS AMONG THREADS. */

/* Golden Rule: INITIALIZE STACK ALLOCATOR ONCE, BEFORE ANY ALLOCATIONS */

/* Silver Rule: DO NOT USE LIFECYLE FUNCTIONS FROM WITHIN THREADS */

#ifndef POINTER_STACK_H

	#define POINTER_STACK_H

	#include <string.h> /* size_t */
	#include <stdbool.h> /* bool */

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

	// Procedures returning a pointer, will return this value to indicate null as null
	// may be a valid pointer value but -1 will never be a valid pointer address.
	void * PS_ACTION_NULL = (void *)(-1LL);

	#define PointerStackActionNull(PTR) PS_ACTION_NULL == PTR
 
	/* Internally Allocated Export Data */
	typedef void * PointerStackExport;

	/* Global Allocation Routines */
	typedef void * ( * PointerStackAllocator   ) ( size_t );
	typedef void * ( * PointerStackReallocator ) ( void *, size_t );
	typedef void   ( * PointerStackDeallocator ) ( void * );

	/* API Procedures */

	/* main */
	extern bool   pointer_stack_push(PointerStack, void *);
	extern void * pointer_stack_pop(PointerStack);
	extern void * pointer_stack_peek(PointerStack, size_t);
	extern void * pointer_stack_poke(PointerStack, size_t, void *);
	extern bool   pointer_stack_pack(PointerStack * stack);
	extern void * pointer_stack_pointer(PointerStack, size_t);

	/* allocation */
	extern void pointer_stack_initialize_allocation(PointerStackAllocator create, PointerStackReallocator resize, PointerStackDeallocator destroy);

	/* extended */
	extern bool   pointer_stack_reverse(PointerStack);
	extern bool   pointer_stack_void(PointerStack, size_t);
	extern bool   pointer_stack_invert(PointerStack, bool);
	extern size_t pointer_stack_error(PointerStack);
	extern char * pointer_stack_license(void);
	
	/* I/O */
	extern size_t pointer_stack_get_count(PointerStack);
	extern bool   pointer_stack_free(PointerStackExport);

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

