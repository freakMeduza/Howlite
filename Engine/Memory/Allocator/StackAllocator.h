#pragma once

#include "IAllocator.h"

namespace Howlite {

	struct StackAllocatorConfig : public IAllocatorConfig {
		HL_CLASS_DEFAULT(StackAllocatorConfig);

		StackAllocatorConfig(void* InMemoryPoniter, const size_t InMemorySize) :
			mMemoryPointer{ InMemoryPoniter }, mMemorySize{ InMemorySize }
		{
		}

		void* mMemoryPointer = nullptr;
		size_t mMemorySize = 0u;
	};

	class StackAllocator : public IAllocator {
	public:
		HL_CLASS_DISABLE_COPY(StackAllocator);
		// Inherited via IAllocator
		virtual bool Init(const IAllocatorConfig* InConfig) override;
		virtual void Terminate() override;
		virtual void* Allocate(const size_t InMemorySize, const size_t InAlignment) override;
		virtual void Free(void* InMemory) override;
		virtual void Reset() override;

	private:
		struct MetaInfo {
			size_t mAdjustment = 0;
		};

	};

}