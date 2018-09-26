#include <iostream>
#include "linearallocator.h"

/**
 * @mainpage Pool Allocators made...difficult
 * In this workshop we're going to explore a bunch of interesting design patterns and
 * practices, and specifically we're going to look at new/delete overloading, pool allocators,
 * singletons and exceptions! I would like the following features, in increasing order of difficulty:
 * -# Implement a simple linear pool allocator class. Essentially this will allocate a block of memory
 *    once. Memory of a particular size can be requested from the allocator, and if there is any
 *    left a pointer will be returned. Note that simply allocating is easy enough - don't bother
 *    with deleting memory, as this will be much much more painful. Also, don't bother implementing
 *    memory reallocation unless you particularly want too - it's not needed for this exercise.
 *    Take a look at http://en.wikipedia.org/wiki/Memory_pool and at
 *    https://molecularmusings.wordpress.com/2012/08/14/memory-allocation-strategies-a-linear-allocator/
 *    (although I didn't use these, so my solution looks quite different).
 * -# Create a class with overloaded new and delete operators through which memory can be allocated
 *    from the pool. This should have some dummy data members to make it occupy some memory.
 * -# Put some exception handling in your pool allocator, so that you handle the situation when
 *    memory is requested but none is available.
 * -# Refactor your linear pool allocator as a Singleton class - this will mean that only one instance
 *    can exist. Consider using smart pointers to manage the lifetime of the memory block.
 */
int main()
{
    return 0;
    // What - you were expecting something here? ;)
}

