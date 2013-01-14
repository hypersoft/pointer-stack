[Hypersoft pointer-stack](http://www.github.com/hypersoft/pointer-stack) <markup theme=hypersoft>
=======================

A procedural library of methods for managing stacks of pointers.

#### Status

* Active Development

><hr>

#### So what's a stack anyway?

You should consult this
[Wikipedia Article](http://en.wikipedia.org/wiki/Stack_%28abstract_data_type%29) for
a more informative approach than asking us to explain it for you.

><hr>

#### Why did you create this?

Because we cannot do this at runtime:

	char * array = { "one", "two", "three" };

That is not all that we cannot do by far, but represents the most basic need to
maintain a "naturally ordered collection" of items at runtime.

><hr>

#### The Pointers of Stack Management
You may or may not have, ever heard of "The Pointers of Stack Management". This term
was coined here by the original project author. Pointers 1-2 are pretty much your
"standard fare". Items further down the list are "trinkets of conventionality". More
or less "Code Hacker's Tools".

   1. pointer\_stack_push

	  In order to have a stack, one must have a way to add things to the
	  stack. "push" provides just such a method.<br><br>

   2. pointer\_stack_pop

	  The newest additions to the stack, are usually the first to go. "pop" provides,
	  a way to "git rid" of the last piece added to the stack.<br><br>

   3. pointer\_stack_peek

	  Expectedly, having a stack isn't worth much if you can't "show off" the
	  the individual pieces that make one stack whole. "peek" provides just
	  such a method.<br><br>

   4. pointer\_stack_poke

	  Who says "trading up" is pointless? "poke" provides a method to, trade one stack
	  value with another.<br><br>

   5. pointer\_stack_pack

	  When you "pop" a PointerStack, there is dynamic memory left in its place. This
	  is what's commonly called a "memory leak", essentially, un-re-claimed data.
	  Hypersoft PointerStacks are eager to re-use this data. In situations where
	  re-use of data is favorably unlikely, one may issue a "pack" on a PointerStack,
	  to make the unused data available to other operations.<br><br>

><hr>

#### Limitations
From time to time, a stack may be some thing that can only grow to a limited number of
items, And it may never grow to such a length. For these cases, the following methods
are provided.

   1. pointer\_stack\_set_limit

	  Truncates data, packs stack as well.<br><br>

   2. pointer\_stack\_get_limit

	  Obtain the value associated with the stack's upper limit.<br><br>

><hr>

#### Private Data
From time to time, a stack may require an association of unmanaged data. Such as a type
descriptor, a procedural header, or anything related to the contents of the stack, such
as yet another PointerStack! For these cases, the following methods are provided.

   1. pointer\_stack\_set_private

	  Overwrites current if any.<br><br>

   2. pointer\_stack\_get_private

	  Obtain the pointer to the private data.<br><br>

><hr>

#### Extended Operations

   01. pointer\_stack_elements

	  Get the number of elements currently on the stack.<br><br>

   02. pointer\_stack_lock

	  Sometimes you need a pointer to a pointer, since we have these, we need a
	  reference counted way to prevent the stack from growing and shrinking.
	  Reallocation sometimes transfers the stack data to a new location. Whenever,
	  a stack is limited, lock, essentially "does nothing", unless the, stack becomes,
	  "unlimited".<br><br>

   03. pointer\_stack_unlock

	  decrements the lock reference count by 1.<br><br>

   04. pointer\_stack_reverse

	  Physically, reverses the order of all elements after "packing". This operation
	  is not effected by "lock" as it will not invalidate the primary pointer.<br><br>

   05. pointer\_stack_invert

	  Mathematically, reverses the order of all elements. This only applies to "peek"
	  and "poke" operations. For all other operations, reverse the stack.<br><br>

   06. pointer\_stack_sort

	  Sort the elements by their content address, least to greatest.<br><br>

   07. pointer\_stack_export

	  Given a valid range of elements, export returns a "plain jane" newly allocated,
	  zero terminated, array of pointers which must be freed.<br><br>

   08. pointer\_stack_import

	  Given a zero terminated array of pointers, place each pointer on to the top of
	  the stack.<br><br>

   09. pointer\_stack_void

	  Unwind the stack index by a number of items.<br><br>

   10. pointer\_stack_free

	  Whenever an operation allocates data, it must be freed by this function.<br><br>

   11. pointer\_stack_allocation

	  Called to initialize the API, one must provide allocator, deallocator, and
	  reallocator procedures. The prototype of these procedures must follow the C
	  standard. If this procedure is not called, the C standard is used for each
	  procedure. This procedure may not be called after a stack item has been
	  allocated until such time that no stacks are allocated.<br><br>

   12. pointer\_stack\_auto_pack

	  Sets a value, that states when to automatically pack the stack due to excess.
	  <br><br>

   13. pointer\_stack_buffer

	  Sets a value, that states how many units should be pre-allocated when space is
	  needed.<br><br>

   13. pointer\_stack_optimize

	  Sets the value of "auto_pack" and "buffer".<br><br>

Notes

* Combinations of import, export, and void can be used to "slice", "join",
"duplicate", "concatenate", or otherwise manipulate entire stacks.

* Calling "free" on a pointer stack does not free private data, or any element data.

* The pointer stack API, does not provide a way to get the actual stack pointer. This
  is in the full interest of data integrity. You should use the operationally sane
  "export" if you need to manually modify data, or pass it along to something else.

><hr>

