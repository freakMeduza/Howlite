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

	void SystemManager::SortSystemPriorityOrder()
	{
		HL_ASSERT(mSystemPriorityOrder.size() <= mSystemRegistry.size(), "Priority order can't be more than system register size");

		if (!mIsDirty)
		{
			return;
		}

		std::qsort(mSystemPriorityOrder.data(), mSystemPriorityOrder.size(), sizeof(ISystem*), [](const void* Left, const void* Right)
		{
			const uint8_t left = static_cast<uint8_t>(((ISystem*)Left)->GetPriority());
			const uint8_t right = static_cast<uint8_t>(((ISystem*)Right)->GetPriority());
			return (int)(left > right) - (left < right);
		});

		mIsDirty = false;
	}

}