//	Description: The BumpAllocator struct is used to implement a custom allocator that uses preallocated memory.
//           	 To avoid slow heap allocation we will use the BumpAllocator for our dynamic allocation needs. The
//               BumpAllocator cannot deallocate parts of its memory, only the entire memory. This is fine for now,
//           	 but as the complexity of the project grows we might need to implement additional allocation strategies
// 
//               The implementation of BumpAllocator is in the header file because it uses templates to define behaviour
//               across datypes/container.
//
//	Author: Svante Drakenberg
#pragma once
#include "../Logging/Logger.h"


//Parent struct to all BumpAllocators, they all share the same memory
struct BumpAllocatorMemory {

protected:
    static constexpr std::size_t memorySize = 1024 * 1000;
    static inline std::byte buffer[memorySize];
    static inline std::size_t offset = 0;
};


template <typename T>
struct BumpAllocator : BumpAllocatorMemory {
    using value_type = T;

    // Constructor
    BumpAllocator(){
        LOGDEBUG("Constructing instance of custom allocator.");
    }

    // Copy constructor
    template <typename U>
    BumpAllocator(const BumpAllocator<U>&obj) noexcept {}



    // Allocate memory from the preallocated pool.
    T* allocate(std::size_t n) {
        size_t requiredMemory = n * sizeof(T);                                                                                      //calculates required number of bytes
        if (requiredMemory > memorySize - offset) {                                                                                 //terminates if insufficient memory
            LOGFATAL("Tried allocating %d bytes (%d %s), not enough memory", n * sizeof(T), n, typeid(T).name());
        }
        T* ptr = reinterpret_cast<T*>(&buffer[offset]);                                                                             //get address of available memory
        offset += requiredMemory;                                                                                                   //update offset
        LOGDEBUG("%d/%d bytes in use after allocating %d bytes (%d %s)", offset, memorySize, n * sizeof(T), n, typeid(T).name());
        return ptr;
    }

    // Stateless allocator doesn't deallocate anything
    void deallocate(T* p, std::size_t) noexcept {
        // No-op for stateless allocator
    }

    // Equality operators to fulfill allocator requirements, MIGHT NOT BE NEEDED
    bool operator==(const BumpAllocator&) const noexcept { return true; }
    bool operator!=(const BumpAllocator&) const noexcept { return false; }

};
