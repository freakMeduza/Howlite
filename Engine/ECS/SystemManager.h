#pragma once 

#include "System.h"
#include "Engine/Memory/Allocator/LinearAllocator.h"

namespace Howlite {

	class SystemManager final {
		static constexpr size_t SYSTEM_BUFFER_MEMORY_SIZE = 8388608; // 8mb

	public:
		HL_CLASS_DISABLE_COPY(SystemManager);

		bool Init();
		void Terminate();

		template<typename SystemType, typename ... Args>
		SystemType* AddSystem(Args&& ... InArgs)
		{
			const SystemTypeId typeId = SystemType::STATIC_SYSTEM_TYPE_ID;

			// avoid multiple system registration
			if (auto it = mSystemRegistry.find(typeId); it != mSystemRegistry.end())
			{
				if (it->second != nullptr)
				{
					return (SystemType*)it->second;
				}
			}
			void* pointer = mAllocator.Allocate(sizeof(SystemType), alignof(SystemType));
			HL_ASSERT(pointer, "Failed to allocate memory for system");
			ISystem* system = new(pointer)SystemType{ std::forward<Args>(InArgs)... };
			mSystemRegistry.insert({ typeId, system });
			mSystemPriorityOrder.push_back(system);
			mIsDirty = true;
			return (SystemType*)system;
		}

		template<typename SystemType>
		SystemType* GetSystem()
		{
			if (auto it = mSystemRegistry.find(SystemType::STATIC_SYSTEM_TYPE_ID); it != mSystemRegistry.end())
			{
				return (SystemType*)it->second;
			}

			return nullptr;
		}

		void SortSystemPriorityOrder();

	private:
		LinearAllocator mAllocator;
		std::vector<ISystem*> mSystemPriorityOrder;
		std::unordered_map<SystemTypeId, ISystem*> mSystemRegistry;
		bool mIsDirty = false;
	};

}