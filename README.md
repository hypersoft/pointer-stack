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

### API Downplay
<hr>

#### Custom Allocation
The PointerStack allocator manages data allocation, and streamlines the process of
tracking the number of distributed allocation units by providing an internal API.

   00. pointer\_stack\_initialize_allocation(PointerStackAllocator create, PointerStackReallocator resize, PointerStackDeallocator destroy)

	  Called to initialize the allocator API, one must provide allocator, deallocator,
	  and reallocator procedures. The prototype of these procedures must follow the C
	  standard. If this procedure is not called, the C standard is used for each
	  procedure. This procedure may not be called after any dynamic data has been
	  allocated until such time that no data provided by the allocator are allocated.
	  <br><br>

><hr>

#### Lifecycle
Procedures in the 'Lifecycle' category, allow you to create a PointerStack, and
dispose of the same.

   00. PointerStack pointer\_stack\_create(void)

	  Creates a new empty PointerStack and returns a * to the desired object type.
	  <br><br>

   00. bool pointer\_stack\_dispose(PointerStack stack)

	  Attempts to "release" a PointerStack object. The object may not be released, if
	  it does not exist or is locked. The return value always reflects the success of
	  the intended operation.<br><br>

Note: Internally, the API uses PointerStack *, externally the API exposes only a
void * whose type is defined as a 'PointerStack' This affords us an internal struct
design that can be modified readily with backwards compatibility in mind. 

><hr>

#### Optimization
Optimization takes place in the allocation and deallocation routines. Instead of,
reallocating the ["stack frame"](http://en.wikipedia.org/wiki/Stack_frame#Structure)
for each "push" and "pop", space can be automatically reserved or truncated by setting
up a proper buffering scheme.

   00. bool pointer\_stack\_set\_buffering(PointerStack stack, size\_t size)

	  Sets a value that determines how to buffer the PointerStack. If available space,
	  reaches zero, and the buffer is not locked or limited, the value provided here
	  will be pre-allocated. The default is 8 (pointer units).
	  <br><br>

><hr>

#### Limitations
From time to time, a stack may be some thing that can only grow to a limited number of
items, And it may never grow to such a length. For these cases, the following methods
are provided.

   00. bool pointer\_stack\_set\_limit(PointerStack stack, size\_t)

	  Sets the upper limit on the number of items a stack may contain.<br><br>

   00. size\_t pointer\_stack\_get_limit(PointerStack stack)

	  Obtain the value associated with the PointerStack's upper limit.<br><br>

><hr>

#### Private Data
From time to time, a PointerStack may require an association of unmanaged data. Such
as a type descriptor, a procedural header, or anything related to the contents of the
PointerStack, such as
[yet another PointerStack](http://en.wikipedia.org/wiki/Associative_array)! For these
cases, the following methods are provided.

   00. bool pointer\_stack\_set\_private(PointerStack stack, void * data)

	  Writes a private data pointer to the PointerStack object in a reserved location.
	  <br><br>

   00. void * pointer\_stack\_get\_private(PointerStack stack)

	  Obtain the pointer to the private data.<br><br>

><hr>

#### The Pointers of Stack Management
You may or may not have, ever heard of "The Pointers of Stack Management". This term
was coined here by the original project author. Pointers 1-2 are pretty much your
"standard fare". Items further down the list are "trinkets of conventionality". More
or less "Code Hacker's Tools". Oddly enough, "The Pointers of Stack Management", can
be simply called "The P's of Stack Management" because each operation, begins with the
letter 'P'.

   1. bool pointer\_stack\_push(PointerStack stack, void * pointer)

	  Add a pointer value to the top of the PointerStack. Boolean success.<br><br>

   2. void * pointer\_stack\_pop(PointerStack stack)

	  Remove and retrieve the last pointer pushed onto the PointerStack. The item's
	  index is marked as the next location for a push, but is not 'deleted'.<br><br>

   3. void * pointer\_stack\_peek(PointerStack stack, size_t index)

	  Returns the pointer value in the PointerStack at the given index position, or
	  -1 if that value cannot be located. -1 Is the sentinel value for all basic return
	  pointer functions. This is because NULL may actually be the value in the
	  PointerStack.<br><br>

   4. void * pointer\_stack\_poke(PointerStack stack, size_t index, void * pointer)

	  Poke allows one to both set and retreive the current pointer at a given index. 
	  <br><br>

   5. bool pointer\_stack\_pack(PointerStack stack)

	  Pack reclaims unused pointer indexes. This is not done automatically for
	  optimal performance. Pack buffers the PointerStack to whatever buffering has
	  been declared for each PointerStack it is called on. 8 pointers, is the default
	  buffering for a PointerStack.<br><br>

   6. void * pointer\_stack\_pointer(PointerStack stack, size_t index)

	  If you need a pointer to a pointer in a PointerStack, this is your man.<br><br>

><hr>

#### Protection
Sometimes you need a pointer to a pointer, since we have these, we need a reference 
counted way to prevent the PointerStack from growing and shrinking. Reallocation 
sometimes transfers the PointerStack data to a new location. Whenever, a stack is 
limited, lock/unlock, essentially "do nothing", unless the, stack becomes, 
"unlimited".

   00. bool pointer\_stack\_lock(PointerStack stack)

	  Always increments the PointerStack lock reference count by 1.<br><br>

   00. bool pointer\_stack\_unlock(PointerStack stack)

	  Always decrements the PointerStack lock reference count by 1.<br><br>

   00. size\_t pointer\_stack\_get\_lock(PointerStack stack)

	  Always returns the PointerStack lock reference count.<br><br>

><hr>

#### Data I/O
I/O, its not just on/off. Its what computers are supposed to do.

   00. size\_t pointer\_stack\_get\_count(PointerStack stack)

	  Get the number of elements currently in the PointerStack.<br><br>

   00. PointerStackExport * pointer\_stack\_export(PointerStack stack, size\_t from, size\_t to)

	  Given a valid range of elements, export returns a
	  ["plain jane"](http://en.wikipedia.org/wiki/Plain_Jane) newly allocated,
	  zero terminated, array of pointers which must be freed.<br><br>

   00. bool pointer_stack_free(PointerStackExport pointer)

	  Whenever an export is requested, it must be freed by this function.<br><br>

   00. pointer\_stack_import ??

	  Given a zero terminated array of pointers, place each pointer on to the top of
	  the PointerStack.<br><br>

><hr>

#### Extended Operations

   00. bool pointer\_stack\_reverse(PointerStack)

	  Physically, reverses the order of all elements after "packing" to the current
	  defined parameters. This operation is not effected by "lock" as it will not
	  result in an operation that relocates the PointerStack element buffer.<br><br>

   00. bool pointer\_stack\_invert(PointerStack, bool)

	  Mathematically, reverses the order of all elements. This only applies to "peek"
	  and "poke" operations. For all other operations, reverse the PointerStack.<br>
      <br>

   00. bool pointer\_stack\_void(PointerStack, size\_t)

	  Unwind the PointerStack index by a number of items.<br><br>

   00. size\_t pointer\_stack\_error(PointerStack)

	  If something went wrong, call this to get the last error. Consequently, clears
	  the last error.<br><br>

   00. char * pointer\_stack\_license(void)

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
