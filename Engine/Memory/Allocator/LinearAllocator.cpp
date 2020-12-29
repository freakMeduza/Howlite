#include "LinearAllocator.h"

namespace Howlite {

	bool LinearAllocator::Init(const IAllocatorConfig* InConfig)
	{
		const LinearAllocatorConfig* config = reinterpret_cast<const LinearAllocatorConfig*>(InConfig);
		HL_ASSERT(config, "Config object cast failed!");
		mMemoryPointer = config->mMemoryPointer;
		mMemorySize = config->mMemorySize;
		mMemoryUsed = 0;
		return true;
	}

	void LinearAllocator::Terminate()
	{
		mMemoryPointer = nullptr;
		mMemoryUsed = 0;
		mMemorySize = 0;
	}

	void* LinearAllocator::Allocate(const size_t InMemorySize, const size_t InAlignment)
	{
		HL_ASSERT(InMemorySize > 0, "InMemorySize should be greater than zero");

		void* current = (void*)((uintptr_t)mMemoryPointer + mMemoryUsed);

		const size_t adjustment = GetAdjustment(current, InAlignment);
		const size_t requirement = adjustment + InMemorySize;

		if (mMemoryUsed + requirement > mMemorySize)
		{
			HL_ERROR("Failed to allocate memory %d bytes\n\tTotalMemory=%d bytes\n\tUsedMemory=%d bytes\n", requirement, mMemorySize, mMemoryUsed);
			return nullptr;
		}

		mMemoryUsed += requirement;

		return (void*)((uintptr_t)current + adjustment);
	}

	void LinearAllocator::Free(void* InMemory)
	{
		HL_UNUSED(InMemory);
		HL_ASSERT(false, "LinearAllocator doesn't spport free operator. Use reset instead");
	}

	void LinearAllocator::Reset()
	{
		mMemoryUsed = 0;
	}

}