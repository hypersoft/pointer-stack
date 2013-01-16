[Hypersoft pointer-stack](http://www.github.com/hypersoft/pointer-stack) <markup theme=hypersoft>
=======================

A procedural library of methods (written in C) for managing stacks of pointers.

#### Status

   :warning: Active Development

><hr>

#### So what's a stack anyway?

You should consult this
[Wikipedia Article](http://en.wikipedia.org/wiki/Stack_%28abstract_data_type%29) for
a more informative approach than asking us to explain it to you.

><hr>

#### Why did you create this?

Because we cannot do this at runtime:

	char * array[] = { "one", "two", "three", NULL };

That is not all that we cannot do by far, but represents the most basic need to
maintain a "naturally ordered collection" of items at runtime.

><hr>

#### The Pointers of Stack Management
You may or may not have, ever heard of "The Pointers of Stack Management". This term
was coined here by the original project author. Pointers 1-2 are pretty much your
"standard fare". Items further down the list are "trinkets of conventionality". More
or less "Code Hacker's Tools". Oddly enough, "The Pointers of Stack Management", can
be simply called "The P's of Stack Management" because each operation, begins with the
letter 'P'.

   1. pointer\_stack_push

	  In order to have a stack, one must have a way to add things to the
	  stack. "push" provides just such a method.<br><br>

   2. pointer\_stack_pop

	  The newest additions to the PointerStack, are usually the first to go. "pop"
	  provides, a way to "git rid" of the last piece added to the PointerStack.
	  <br><br>

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

   6. pointer\_stack_pointer

	  If you need a pointer to a pointer in a PointerStack, this is your man.<br><br>

><hr>

#### Allocator Initialization

   00. pointer\_stack_allocation

	  Called to initialize the API, one must provide allocator, deallocator, and
	  reallocator procedures. The prototype of these procedures must follow the C
	  standard. If this procedure is not called, the C standard is used for each
	  procedure. This procedure may not be called after any dynamic data has been
	  allocated until such time that no data provided by the allocator are allocated.
	  <br><br>

><hr>

#### Lifecycle

   00. pointer\_stack_create

	  Creates a new empty PointerStack.<br><br>

   00. pointer\_stack_dispose

	  Attempts to "release" a PointerStack object.<br><br>

><hr>

#### Limitations
From time to time, a stack may be some thing that can only grow to a limited number of
items, And it may never grow to such a length. For these cases, the following methods
are provided.

   00. pointer\_stack\_set_limit

	  Truncates data, packs stack as well.<br><br>

   00. pointer\_stack\_get_limit

	  Obtain the value associated with the PointerStack's upper limit.<br><br>

><hr>

#### Private Data
From time to time, a PointerStack may require an association of unmanaged data. Such
as a type descriptor, a procedural header, or anything related to the contents of the
PointerStack, such as
[yet another PointerStack](http://en.wikipedia.org/wiki/Associative_array)! For these
cases, the following methods are provided.

   00. pointer\_stack\_set_private

	  Overwrites current if any.<br><br>

   00. pointer\_stack\_get_private

	  Obtain the pointer to the private data.<br><br>

><hr>

#### Protection
Sometimes you need a pointer to a pointer, since we have these, we need a reference 
counted way to prevent the PointerStack from growing and shrinking. Reallocation 
sometimes transfers the PointerStack data to a new location. Whenever, a stack is 
limited, lock/unlock, essentially "do nothing", unless the, stack becomes, 
"unlimited".

   00. pointer\_stack_lock

	  Always increments the lock reference count by 1.<br><br>

   00. pointer\_stack_unlock

	  Always decrements the lock reference count by 1.<br><br>

   00. pointer\_stack\_get_lock

	  Always returns the lock reference count.<br><br>

><hr>

#### Optimization
Optimization takes place in the allocation and deallocation routines. Instead of,
reallocating the ["stack frame"](http://en.wikipedia.org/wiki/Stack_frame#Structure)
for each "push" and "pop", space can be automatically reserved or truncated by setting
up a proper buffering scheme.

   00. pointer\_stack\_set_buffering

	  Sets a value that determines how to buffer the PointerStack. If available space,
	  reaches zero, and the buffer is not locked or limited, the value provided here
	  will be pre-allocated. Whenever this value is non-zero, automatic buffering
	  may take place during push/pop operations. The default is 8 units (pointers).
	  <br><br>

><hr>

#### Data I/O
I/O, its not just on/off. Its what computers are supposed to do.

   00. pointer\_stack\_get_count

	  Get the number of elements currently in the PointerStack.<br><br>

   00. pointer\_stack_export

	  Given a valid range of elements, export returns a
	  ["plain jane"](http://en.wikipedia.org/wiki/Plain_Jane) newly allocated,
	  zero terminated, array of pointers which must be freed.<br><br>

   00. pointer\_stack_free

	  Whenever an export is requested, it must be freed by this function.<br><br>

   00. pointer\_stack_import

	  Given a zero terminated array of pointers, place each pointer on to the top of
	  the PointerStack.<br><br>

><hr>

#### Extended Operations

   00. pointer\_stack_reverse

	  Physically, reverses the order of all elements after "packing" to the current
	  defined parameters. This operation is not effected by "lock" as it will not
	  result in an operation that relocates the PointerStack element buffer.<br><br>

   00. pointer\_stack_invert

	  Mathematically, reverses the order of all elements. This only applies to "peek"
	  and "poke" operations. For all other operations, reverse the PointerStack.<br>
      <br>

   00. pointer\_stack_void

	  Unwind the PointerStack index by a number of items.<br><br>

   00. pointer\_stack_error

	  If something went wrong, call this to get the last error. Consequently, clears
	  the last error.<br><br>

   00. pointer\_stack_license

	  Obtains a char * to the license associated with this library for display at the
	  user's request, or developer's behest.<br><br>

Notes

* Combinations of import, export, and void can be used to "slice", "join",
  "duplicate", "concatenate", or otherwise manipulate entire PointerStacks.

* Calling "free" on a PointerStack does not free private data, or any element data.

* The PointerStack API, does not recommend manipulating the actual PointerStack
  pointer. This is in the full interest of data integrity. You should use the
  operationally sane "export" if you need to manually modify data, or pass it along to
  something else. Using pointer\_stack_pointer, is a pretty good way to
  "Shoot your eye out kid". Recommendation: adjust for richochet, shoot straight, and 
  laugh triumphantly in the face of fear.

* The logical allocation unit/alignment for a platform, (theorhetically), should be
  the same size as a pointer. This means a pointer stack, in the hands of a master
  craftsman can be used to allocate machine correct, overflow free
  [multi-byte-plexable](http://en.wikipedia.org/wiki/Multiplexer),
  [pipeline surfing](http://en.wikipedia.org/wiki/Instruction_level_parallelism)
  buffers. Not something we would do, just a fun fact one should know.

><hr>


<div align=center>          :zap: Points of Authority :zap:                     </div>
