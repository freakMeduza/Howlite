#pragma once

#include "IAllocator.h"

namespace Howlite {

	struct PoolAllocatorConfig : public IAllocatorConfig {
		HL_CLASS_DEFAULT(PoolAllocatorConfig);

		PoolAllocatorConfig(void* InMemoryPointer, const size_t InMemorySize, const size_t InObjectSize, const size_t InObjectAlignment) :
			mMemoryPointer{ InMemoryPointer }, mMemorySize{ InMemorySize }, mObjectSize{ InObjectSize }, mObjectAlignment{ InObjectAlignment }
		{
		}

		void* mMemoryPointer = nullptr;
		size_t mMemorySize = 0;
		size_t mObjectSize = 0;
		size_t mObjectAlignment = 0;
	};

	class PoolAllocator : public IAllocator {
	public:
		HL_INTERFACE_DISABLE_COPY(PoolAllocator);
		// Inherited via IAllocator
		virtual bool Init(const IAllocatorConfig* InConfig) override;
		virtual void Terminate() override;
		virtual void* Allocate(const size_t InMemorySize, const size_t InAlignment) override;
		virtual void Free(void* InMemory) override;
		virtual void Reset() override;

	private:
		void** mFreeList = nullptr;
		size_t mObjectSize = 0;
		size_t mObjectAlignment = 0;
	};


}