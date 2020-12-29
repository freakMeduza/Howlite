#include "EventManager.h"
#include "Engine/Engine.h"
#include "Engine/Memory/MemoryManager.h"

namespace Howlite {

	bool EventManager::Init()
	{
		LinearAllocatorConfig config;
		config.mMemoryPointer = Engine::GetInstance()->GetMemoryManager()->Allocate(EVENT_BUFFER_MEMORY_SIZE, "EventManager");
		config.mMemorySize = EVENT_BUFFER_MEMORY_SIZE;
		return mAllocator.Init(&config);
	}

	void EventManager::Terminate()
	{
		ClearEventBuffer();
		Engine::GetInstance()->GetMemoryManager()->Free(mAllocator.GetMemoryPointer());
		mAllocator.Terminate();
	}

	void EventManager::Notify()
	{
		for (const IEvent* event : mEventBuffer)
		{
			DispatchEvent(event);
		}

		ClearEventBuffer();
	}

	void EventManager::DispatchEvent(const IEvent* InEvent)
	{
		if (auto it = mEventCallbackRegistry.find(InEvent->GetEventTypeId()); it != mEventCallbackRegistry.end())
		{
			for (const auto& [name, callback] : it->second)
			{
				callback(InEvent);
			}
		}
	}

	void EventManager::ClearEventBuffer() noexcept
	{
		mEventBuffer.clear();
		mAllocator.Reset();
	}

}