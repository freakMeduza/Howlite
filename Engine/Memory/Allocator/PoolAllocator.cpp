#include "PoolAllocator.h"

namespace Howlite {
	
	bool PoolAllocator::Init(const IAllocatorConfig* InConfig)
	{
		const PoolAllocatorConfig* config = reinterpret_cast<const PoolAllocatorConfig*>(InConfig);
		HL_ASSERT(config, "Config object cast failed!");
		mMemoryPointer = config->mMemoryPointer;
		mMemorySize = config->mMemorySize;
		mObjectSize = config->mObjectSize;
		mObjectAlignment = config->mObjectAlignment;
		Reset();
		return true;
	}
	
	void PoolAllocator::Terminate()
	{
		mMemoryPointer = nullptr;
		mFreeList = nullptr;
		mMemoryUsed = 0;
		mMemorySize = 0;
		mObjectSize = 0;
		mObjectAlignment = 0;
	}
	
	void* PoolAllocator::Allocate(const size_t InMemorySize, const size_t InAlignment)
	{
		HL_ASSERT(InMemorySize > 0, "InMemorySize should be greater than zero");
		HL_ASSERT(InAlignment == mObjectAlignment, "InAlignment should be equal object alignment");

		if (mFreeList == nullptr)
		{
			HL_ERROR("Failed to allocate memory. No available capacity.");
			return nullptr;
		}

		void* freeBlock = mFreeList;
		// change the head of free list to the next element
		mFreeList = (void**)*mFreeList;
		mMemoryUsed += mObjectSize;
		return freeBlock;
	}
	
	void PoolAllocator::Free(void* InMemory)
	{
		// push front
		*((void**)InMemory) = mFreeList;
		mFreeList = (void**)InMemory;
		mMemoryUsed -= mObjectSize;
	}
	
	void PoolAllocator::Reset()
	{
		const size_t adjustment = GetAdjustment(mMemoryPointer, mObjectAlignment);
		const size_t objectCount = (mMemorySize - adjustment) / mObjectSize;
		void* current = mMemoryPointer;
		// align first memory address;
		current = (void*)((uintptr_t)current + adjustment);
		// setup first address as a start of free list
		mFreeList = (void**)current;
		// fill free list
		void** freeList = mFreeList;
		for (auto i = 0; i < objectCount - 1; ++i)
		{
			*freeList = (void*)((uintptr_t)freeList + mObjectSize);
			freeList = (void**)*freeList;
		}

		*freeList = nullptr;
	}
}