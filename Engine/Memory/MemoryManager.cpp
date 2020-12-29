#include "MemoryManager.h"

namespace Howlite {

	void* MemoryManager::Allocate(const size_t InMemorySize, const std::string& InMemoryUser)
	{
		HL_DEBUG("Request for %d bytes of memory by user %s\n", InMemorySize, InMemoryUser.c_str());
		void* pointer = mAllocator.Allocate(InMemorySize, sizeof(void*));
		HL_DEBUG("Allocated %d bytes of memory by user %s pointer [%p]\n", InMemorySize, InMemoryUser.c_str(), pointer);
		mMemoryRegistry.insert({ pointer, InMemoryUser });
		return pointer;
	}

	void MemoryManager::Free(void* InMemory)
	{
		if (auto it = mMemoryRegistry.find(InMemory); it != mMemoryRegistry.end())
		{
			HL_DEBUG("Release memory pointer [%p] by user %s\n", it->first, it->second.c_str());
			mAllocator.Free(it->first);
			mMemoryRegistry.erase(it);
			return;
		}
		HL_WARN("Couldn't free memory pointer [%p] is not in registry\n", InMemory);
	}

	bool MemoryManager::Init()
	{
		HL_DEBUG("Initialize memory manager\n");
		void* pointer = malloc(GLOBAL_MEMORY_SIZE);
		HL_ASSERT(pointer, "Failed to get memory from OS");
		HL_DEBUG("Allocated %d bytes of memory pointer [%p]\n", GLOBAL_MEMORY_SIZE, pointer);
		StackAllocatorConfig config;
		config.mMemoryPointer = pointer;
		config.mMemorySize = GLOBAL_MEMORY_SIZE;
		return mAllocator.Init(&config);
	}

	void MemoryManager::Terminate()
	{
		HL_DEBUG("Release memory manager\n");
		free(mAllocator.GetMemoryPointer());
		mAllocator.Terminate();
		mMemoryRegistry.clear();
	}

}