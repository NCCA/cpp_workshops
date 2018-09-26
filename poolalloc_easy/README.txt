* Pool Allocators made...difficult

In this workshop we're going to explore a bunch of interesting design patterns and practices, and specifically we're going to look at new/delete overloading, pool allocators, singletons and exceptions! I would like the following features, in increasing order of difficulty:

1) I've provided a LinearPoolAllocator for your pleasure - take a look at the header to see how the thing is supposed to work.
2) Create a class with overloaded new and delete operators through which memory can be allocated from the pool. This should have some dummy data members to make it occupy some memory.
3) Put some exception handling in your pool allocator, so that you handle the situation when memory is requested but none is available.
4) Refactor your linear pool allocator as a Singleton class - this will mean that only one instance can exist. Consider using smart pointers to manage the lifetime of the memory block.

Richard Southern
08/03/2017
