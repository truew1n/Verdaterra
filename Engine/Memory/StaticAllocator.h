#ifndef EN_STATIC_ALLOCATOR_H
#define EN_STATIC_ALLOCATOR_H

#define EN_STATIC_ALLOCATOR_DATA_SIZE 8388608ULL


template<typename TElementType>
class TStaticAllocator {
private:
	static uint8_t Data[EN_STATIC_ALLOCATOR_DATA_SIZE];
};

#endif

