#pragma once

#include "Engine/Platform.h"

namespace Howlite {

	struct IAllocatorConfig {
		HL_INTERFACE_DEFAULT(IAllocatorConfig);
	};

	class IAllocator {
	public:
		HL_INTERFACE_DISABLE_COPY(IAllocator);

		virtual bool Init(const IAllocatorConfig* InConfig) = 0;
		virtual void Terminate() = 0;
		virtual void* Allocate(const size_t InMemorySize, const size_t InAlignment) = 0;
		virtual void Free(void* InMemory) = 0;
		virtual void Reset() = 0;

		inline void* GetMemoryPointer() const noexcept { return mMemoryPointer; }
		inline size_t GetMemorySize() const noexcept { return mMemorySize; }
		inline size_t GetMemoryUsed() const noexcept { return mMemoryUsed; }

	protected:
		inline void* AlignForward(const void* InMemoryPointer, const size_t InAlignment) const noexcept
		{
			return (void*)(((uintptr_t)InMemoryPointer + (uintptr_t)(InAlignment - 1)) & (uintptr_t)~(InAlignment - 1));
		}

		inline size_t GetAdjustment(const void* InMemoryPointer, const size_t InAlignment) const noexcept
		{
			const size_t adjustment = InAlignment - ((uintptr_t)InMemoryPointer & (uintptr_t)(InAlignment - 1));
			return adjustment == InAlignment ? 0u : adjustment;
		}

		inline size_t GetAdjustmentWithExtra(const void* InMemoryPointer, const size_t InAlignment, const size_t InExtra) const noexcept
		{
			size_t adjustment = GetAdjustment(InMemoryPointer, InAlignment);
			size_t required = InExtra;

			if (adjustment < required)
			{
				required -= adjustment;
				// increase adjustment to fit required memory size
				adjustment += InAlignment * (required / InAlignment);

				if (required % InAlignment > 0) adjustment += InAlignment;
			}

			return adjustment;
		}

		void* mMemoryPointer = nullptr;
		size_t mMemorySize = 0;
		size_t mMemoryUsed = 0;
	};

}