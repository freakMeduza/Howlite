#include "StackAllocator.h"

namespace Howlite {

	bool StackAllocator::Init(const IAllocatorConfig* InConfig)
	{
		const StackAllocatorConfig* config = reinterpret_cast<const StackAllocatorConfig*>(InConfig);
		HL_ASSERT(config, "Config object cast failed!");
		mMemoryPointer = config->mMemoryPointer;
		mMemorySize = config->mMemorySize;
		mMemoryUsed = 0;
		return true;
	}

	void StackAllocator::Terminate()
	{
		mMemoryPointer = nullptr;
		mMemoryUsed = 0;
		mMemorySize = 0;
	}

	void* StackAllocator::Allocate(const size_t InMemorySize, const size_t InAlignment)
	{
		HL_ASSERT(InMemorySize > 0, "InMemorySize should be greater than zero");

		void* current = (void*)((uintptr_t)mMemoryPointer + mMemoryUsed);

		const size_t adjustment = GetAdjustmentWithExtra(current, InAlignment, sizeof(MetaInfo));
		const size_t requirement = adjustment + InMemorySize;

		if (mMemoryUsed + requirement > mMemorySize)
		{
			HL_ERROR("Failed to allocate memory %d bytes\n\tTotalMemory=%d bytes\n\tUsedMemory=%d bytes\n", requirement, mMemorySize, mMemoryUsed);
			return nullptr;
		}

		MetaInfo* metaInfo = (MetaInfo*)(((uintptr_t)current + adjustment) - sizeof(MetaInfo));
		metaInfo->mAdjustment = adjustment;

		mMemoryUsed += requirement;

		return (void*)((uintptr_t)current + adjustment);
	}

	void StackAllocator::Free(void* InMemory)
	{
		MetaInfo* metaInfo = (MetaInfo*)((uintptr_t)InMemory - sizeof(MetaInfo));
		mMemoryUsed = (uintptr_t)InMemory - metaInfo->mAdjustment - (uintptr_t)mMemoryPointer;
	}

	void StackAllocator::Reset()
	{
		mMemoryUsed = 0;
	}

}