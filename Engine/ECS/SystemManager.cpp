#include "SystemManager.h"
#include "Engine/Engine.h"
#include "Engine/Memory/MemoryManager.h"

namespace Howlite {

	bool SystemManager::Init()
	{
		LinearAllocatorConfig config;
		config.mMemoryPointer = Engine::GetInstance()->GetMemoryManager()->Allocate(SYSTEM_BUFFER_MEMORY_SIZE, "SystemManager");
		config.mMemorySize = SYSTEM_BUFFER_MEMORY_SIZE;
		return mAllocator.Init(&config);
	}

	void SystemManager::Terminate()
	{
		for (auto it = mSystemPriorityOrder.rbegin(); it != mSystemPriorityOrder.rend(); ++it)
		{
			(*it)->~ISystem();
			*it = nullptr;
		}

		mSystemPriorityOrder.clear();
		mSystemRegistry.clear();
		Engine::GetInstance()->GetMemoryManager()->Free(mAllocator.GetMemoryPointer());
		mAllocator.Terminate();
	}

}