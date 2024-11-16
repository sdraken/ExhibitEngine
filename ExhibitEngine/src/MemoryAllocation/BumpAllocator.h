#include "../Logging/Logger.h"

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



    // Allocate memory from the preallocated pool
    T* allocate(std::size_t n) {
        // Calculate the required space and ensure it does not exceed pool size
        size_t requiredMemory = n * sizeof(T);
        if (requiredMemory > memorySize - offset) {
            LOGFATAL("Tried allocating %d bytes (%d %s), not enough memory", n * sizeof(T), n, typeid(T).name());
        }
        T* ptr = reinterpret_cast<T*>(&buffer[offset]);
        offset += requiredMemory;
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
