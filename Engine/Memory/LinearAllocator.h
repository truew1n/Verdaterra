#ifndef EN_LINEAR_ALLOCATOR_H
#define EN_LINEAR_ALLOCATOR_H

#include <iostream>

static constexpr uint64_t LinearAllocatorBlockSize = 64 * 1024;

template<typename TElementType>
class TLinearAllocator {
private:
	inline static uint8_t Data[LinearAllocatorBlockSize];
	inline static uint64_t CurrentOffset = 0;
};

#endif

