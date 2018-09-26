#ifndef LINEARALLOCATOR_H
#define LINEARALLOCATOR_H

#include <memory>
#include <iostream>

#define DEFAULT_BLOCKSIZE 1024

/**
 * @brief The LinearAllocatorSingleton class
 * This class manages the memory pool without anything clever - it just returns
 * a chunk of memory for your delectation.
 */
class LinearAllocatorSingleton {
public:
    /// Singleton access pattern
    static LinearAllocatorSingleton& getInstance() {
        if (m_instance.get() == 0) {
            m_instance.reset(new LinearAllocatorSingleton());
        }
        return *m_instance.get();
    }

    /// Public destructor
    ~LinearAllocatorSingleton() {
        if (m_blk_void != nullptr) {
            delete [] m_blk_char;
        }
        std::cout << "LinearAllocatorSingleton::~LinearAllocatorSingleton()\n";
    }

    /// Calculate the amound of memory remaining
    std::size_t remaining() {
        return (std::size_t) (m_end_char - m_hdr_char);
    }

    /// Request a chunk of memory from the Linear Allocator
    void *alloc(std::size_t _blockSize) /*throws (const std::bad_alloc&)*/;

private:
    /// Private constructors
    LinearAllocatorSingleton(std::size_t _blockSize = DEFAULT_BLOCKSIZE);

    /// Private copy constructor: this is impossible
    LinearAllocatorSingleton(const LinearAllocatorSingleton&) = delete;

    /// Private assignment operator
    void operator=(const LinearAllocatorSingleton&) = delete;

    /// The single instance of our linear allocator
    static std::unique_ptr<LinearAllocatorSingleton> m_instance;

    /// Start of our data block
    union {
        void* m_blk_void;
        char* m_blk_char;
    };

    /// Current head of our data block
    union {
        void* m_hdr_void;
        char* m_hdr_char;
    };

    /// Current end of our data block
    union {
        void* m_end_void;
        char* m_end_char;
    };
};



#endif // LINEARALLOCATOR_H
