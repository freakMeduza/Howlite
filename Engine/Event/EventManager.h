#pragma once

#include "Engine/Platform.h"
#include "Engine/Memory/Allocator/LinearAllocator.h"
#include "Event.h"

namespace Howlite {

	class EventManager final {
		static constexpr size_t EVENT_BUFFER_MEMORY_SIZE = 4194304; // 4mb

		using EventCallback = std::function<void(const IEvent*)>;
	public:
		HL_CLASS_DISABLE_COPY(EventManager);

		bool Init();
		void Terminate();

		template<typename EventType>
		void Subscribe(const std::string& InCallbackName, std::function<void(const EventType*)>&& InEventCallback)
		{
			EventCallback callback = [userCallback = std::move(InEventCallback)](const IEvent* InEvent)
			{
				if (InEvent->GetEventTypeId() == EventType::STATIC_EVENT_TYPE_ID)
				{
					userCallback(static_cast<const EventType*>(InEvent));
				}
			};

			const EventTypeId eventTypeId = EventType::STATIC_EVENT_TYPE_ID;
			
			if (auto it = mEventCallbackRegistry.find(eventTypeId); it == mEventCallbackRegistry.end())
			{
				mEventCallbackRegistry.insert({ eventTypeId, {} });
			}
			
			mEventCallbackRegistry[eventTypeId].push_back({ std::move(InCallbackName), std::move(callback) });
		}

		template<typename EventType>
		void Unsubscribe(const std::string& InCallbackName)
		{
			const EventTypeId eventTypeId = EventType::STATIC_EVENT_TYPE_ID;

			if (auto it = mEventCallbackRegistry.find(eventTypeId); it != mEventCallbackRegistry.end())
			{
				std::vector<std::pair<std::string, EventCallback>>& eventCallbackList = it->second;

				eventCallbackList.erase(std::remove_if(eventCallbackList.begin(), eventCallbackList.end(), [&InCallbackName](const std::pair<std::string, EventCallback>& Other)
				{
					return Other.first == InCallbackName;
				}),
					eventCallbackList.end());
			}
		}

		template<typename EventType, typename ... Args>
		void Post(Args&& ... InArgs)
		{
			void* pointer = mAllocator.Allocate(sizeof(EventType), alignof(EventType));
			if (pointer)
			{
				mEventBuffer.push_back(new(pointer)EventType(std::forward<Args>(InArgs)...));
			}
		}

		template<typename EventType, typename ... Args>
		void Invoke(Args&& ... InArgs)
		{
			void* pointer = mAllocator.Allocate(sizeof(EventType), alignof(EventType));
			if (pointer)
			{
				EventType* eventType = new(pointer)EventType(std::forward<Args>(InArgs)...);
				if (auto it = mEventCallbackRegistry.find(eventType->GetEventTypeId()); it != mEventCallbackRegistry.end())
				{
					for (const auto& [name, callback] : it->second)
					{
						callback(eventType);
					}
				}
			}
		}

		void Notify();

	private:
		void DispatchEvent(const IEvent* InEvent);
		void ClearEventBuffer() noexcept;

		LinearAllocator mAllocator;
		std::list<IEvent*> mEventBuffer;
		std::unordered_map<EventTypeId, std::vector<std::pair<std::string,EventCallback>>> mEventCallbackRegistry;

	};

}