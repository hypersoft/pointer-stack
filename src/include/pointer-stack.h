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

	/* Global Allocation Routines */
	typedef void * ( * PointerStackAllocator   ) ( size_t );
	typedef void * ( * PointerStackReallocator ) ( void *, size_t );
	typedef void   ( * PointerStackDeallocator ) ( void * );

	/* API Procedures */

	/* main */
	typedef void * PointerStack;

	typedef enum PointerStackError {
		PSE_NO_ERROR = 0,
		PSE_NO_STACK,
		PSE_NO_STACK_DATA,
		PSE_STACK_EMPTY,
		PSE_STACK_LOCKED,
		PSE_STACK_LIMITED,
		PSE_OVERFLOW,
		PSE_INVALID_INPUT,
		PSE_INVALID_RANGE,
	} PointerStackError;
 
	extern bool    pointer_stack_push(PointerStack, void *);
	extern void *  pointer_stack_pop(PointerStack);
	extern void *  pointer_stack_peek(PointerStack, size_t);
	extern void *  pointer_stack_poke(PointerStack, size_t, void *);
	extern bool    pointer_stack_pack(PointerStack stack);
	extern void ** pointer_stack_pointer(PointerStack, size_t);

	/* allocation */
	extern void pointer_stack_initialize_allocation(PointerStackAllocator create, PointerStackReallocator resize, PointerStackDeallocator destroy);

	/* extended */
	extern bool   pointer_stack_reverse(PointerStack);
	extern bool   pointer_stack_void(PointerStack, size_t);
	extern bool   pointer_stack_invert(PointerStack, bool);
	extern size_t pointer_stack_error(PointerStack);
	extern char * pointer_stack_license(void);
	
	/* I/O */
	typedef void ** PointerStackExport;
	extern size_t pointer_stack_get_count(PointerStack);
	extern size_t pointer_stack_get_slots(PointerStack);
	extern size_t pointer_stack_get_units(PointerStack);
	extern bool   pointer_stack_free(PointerStackExport);
	extern PointerStackExport pointer_stack_export(PointerStack, size_t, size_t);
	extern bool pointer_stack_import(PointerStack, void **, size_t, size_t);

	/* lifecycle */
	extern PointerStack pointer_stack_create(void);
	extern bool pointer_stack_dispose(PointerStack);

	/* limitations */
	extern size_t pointer_stack_get_limit(PointerStack);
	extern bool pointer_stack_set_limit(PointerStack, size_t);

	/* optimization */
	extern bool   pointer_stack_set_buffering(PointerStack, size_t);
	extern size_t pointer_stack_get_buffering(PointerStack);

	/* private */
	extern void * pointer_stack_get_private(PointerStack);
	extern bool pointer_stack_set_private(PointerStack, void *);

	/* protection */
	extern bool   pointer_stack_unlock(PointerStack);
	extern bool   pointer_stack_lock(PointerStack);
	extern size_t pointer_stack_get_lock(PointerStack);

#endif /* POINTER_STACK_H */

