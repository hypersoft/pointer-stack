/*
	Copyright (C) 2013, Triston J. Taylor
*/

/* Provides Internals:

	void * pointer_stack_allocator_lease(size_t size);
	void * pointer_stack_allocator_resize(void * data, size_t size);
	void   pointer_stack_allocator_release(void * data);

	Provides API:

	extern void pointer_stack_initialize_allocation(PointerStackAllocator create, PointerStackAllocator resize, PointerStackDeallocator destroy);

*/

typedef void * ( * PointerStackAllocator   ) ( size_t         );
typedef void * ( * PointerStackReallocator ) ( void *, size_t );
typedef void   ( * PointerStackDeallocator ) ( void *         );

static PointerStackAllocator 	pointer_stack_allocate  	= &malloc;
static PointerStackReallocator 	pointer_stack_reallocate 	= &realloc;
static PointerStackDeallocator 	pointer_stack_deallocate 	= &free;

static size_t allocated = 0; /* bool/refcount: disable init? */

/* This is not a thread safe operation. Avoid use wherever possible */
void pointer_stack_initialize_allocation(PointerStackAllocator create, PointerStackReallocator resize, PointerStackDeallocator destroy) {

	if (allocated) return; // can't change allocators or deallocators after allocation.

	/* One might argue that these are tied together.. We are not arguing that */

	if (create) pointer_stack_allocate = create;
	if (resize) pointer_stack_reallocate = resize;
	if (destroy) pointer_stack_deallocate = destroy;

	/* Sink or swim! */

}

void * pointer_stack_allocator_lease(size_t size) {
	void * pointer = pointer_stack_allocate(size);
	if (pointer) allocated++;
	return pointer;
}

void * pointer_stack_allocator_resize(void * data, size_t size) {
	void * pointer = NULL;
	if (allocated) { pointer = pointer_stack_reallocate(data, size); }
	return pointer;
}

void pointer_stack_allocator_release(void * data) {
	if (data && allocated) {
		pointer_stack_deallocate(data);
		allocated--;
	}
}

