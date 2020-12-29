#pragma once

#include "IAllocator.h"

namespace Howlite {

	struct LinearAllocatorConfig : public IAllocatorConfig {
		HL_CLASS_DEFAULT(LinearAllocatorConfig);

		LinearAllocatorConfig(void* InMemoryPoniter, const size_t InMemorySize) :
			mMemoryPointer{ InMemoryPoniter }, mMemorySize{ InMemorySize }
		{
		}

		void* mMemoryPointer = nullptr;
		size_t mMemorySize = 0u;
	};

	class LinearAllocator : public IAllocator {
	public:
		HL_CLASS_DISABLE_COPY(LinearAllocator);
		// Inherited via IAllocator
		virtual bool Init(const IAllocatorConfig* InConfig) override;
		virtual void Terminate() override;
		virtual void* Allocate(const size_t InMemorySize, const size_t InAlignment) override;
		virtual void Free(void* InMemory) override;
		virtual void Reset() override;

	};

}