#include "linearallocator.h"

/// Note that if this static member throws an exception there is no way to catch it (is there?)
std::unique_ptr<LinearAllocatorSingleton> LinearAllocatorSingleton::m_instance
    = std::unique_ptr<LinearAllocatorSingleton>(new LinearAllocatorSingleton);

/**
 * @brief LinearAllocatorSingleton::LinearAllocatorSingleton Construct chunk of data of given size
 * @param _blockSize This is currently based on a macro
 */
LinearAllocatorSingleton::LinearAllocatorSingleton(std::size_t _blockSize) {
    try {
        // Attempt to allocate a chunk of memory as chars
        m_blk_char = new char[_blockSize];
    }
    // Note that char's new operator might throw the bad_alloc if out of memory
    catch (const std::bad_alloc&) {
        throw;
    }
    // Catch every other possibility
    catch(...) {
        std::cerr << "LinearAllocatorSingleton::LinearAllocatorSingleton("<<_blockSize<<") - unhandled exception\n";
    }
    m_hdr_void = m_blk_void;
    m_end_void = m_blk_char + _blockSize;
}

/**
 * @brief LinearAllocatorSingleton::alloc Allocate some tasty memory from our own heap of size _blockSize
 * @param _blockSize Size of block to allocate (must be aligned!)
 * @throws std::bad_alloc If we're out of memory
 * @return A pointer to the memory for you to merrily use
 */
void *LinearAllocatorSingleton::alloc(std::size_t _blockSize) {
    try {
        if ((m_hdr_char + _blockSize) > m_end_char) {
            // We've run out of memory - what do we do?
            throw(std::bad_alloc());
        }
    }
    // Catch the case of running out of memory
    catch (const std::bad_alloc&) {
        // We might want to try and resize the block of memory here or something
        throw;
    }
    catch (...) {
        std::cerr << "LinearAllocatorSingleton::alloc("<<_blockSize<<") - unknown error\n";
        exit(0);
    }
    // Note that we're assuming this is nicely aligned (I suspect pain if it isn't)
    void* ret_val = m_hdr_void;
    m_hdr_char += _blockSize;
    return ret_val;
}

