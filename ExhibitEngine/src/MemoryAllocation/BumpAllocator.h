#include <cstddef>
#include <new>  // for std::bad_alloc
#include <memory>

#include "iostream"
//IS IT BETTER IF WE EXPLICITLY DECLARE THE CLASS AND ITS VALUES???
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
        std::cout << "constructing allocator" << std::endl;
    }

    // Copy constructor
    template <typename U>
    BumpAllocator(const BumpAllocator<U>&obj) noexcept {}



    // Allocate memory from the preallocated pool
    T* allocate(std::size_t n) {
        std::cout << "Trying to allocate" << n << " "<< typeid(T).name() << " for a total of " << n*sizeof(T) << "bytes, "<< memorySize - offset << " bytes available" << std::endl;
        // Calculate the required space and ensure it does not exceed pool size
        size_t requiredMemory = n * sizeof(T);
        if (requiredMemory > memorySize - offset) {
            throw std::bad_alloc(); // If not enough memory, throw an exception
        }
        T* ptr = reinterpret_cast<T*>(&buffer[offset]);
        offset += requiredMemory;
        std::cout << "successcull allocation, " << memorySize- offset << " bytes left"  << std::endl;
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
