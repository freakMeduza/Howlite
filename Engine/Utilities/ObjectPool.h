#pragma once

#include "Engine/Engine.h"
#include "Engine/Memory/Allocator/PoolAllocator.h"
#include "Engine/Memory/MemoryManager.h"

namespace Howlite {
	
	template<typename ObjectType, size_t ObjectPoolCapacity>
	class TFixedObjectPool {
		// we would like to set object size to be aligned to system word size,
		// if it's not already aligned (to avoid situation with packed structures
		// and keep possibility to construct internal free list)
		static constexpr size_t OBJECT_SIZE = sizeof(ObjectType) % sizeof(void*) ?
			(sizeof(ObjectType) + sizeof(void*) - (sizeof(ObjectType) % sizeof(void*))) : sizeof(ObjectType);
		static constexpr size_t OBJECT_ALIGNMENT = alignof(ObjectType);
		// we would like to set memory block size to be capable to fit required object count plus add
		// additional space for possible align of given memory pointer
		static constexpr size_t MEMORY_BLOCK_SIZE = OBJECT_SIZE * ObjectPoolCapacity + sizeof(void*);
		static constexpr size_t MEMORY_BLOCK_CAPACITY = ObjectPoolCapacity;

	public:
		HL_DISABLE_COPY(TFixedObjectPool);

		TFixedObjectPool(const std::string& InMemoryUser)
		{
			PoolAllocatorConfig config;
			config.mMemoryPointer = Engine::GetInstance()->GetMemoryManager()->Allocate(MEMORY_BLOCK_SIZE, InMemoryUser);
			config.mMemorySize = MEMORY_BLOCK_SIZE;
			config.mObjectSize = OBJECT_SIZE;
			config.mObjectAlignment = OBJECT_ALIGNMENT;
			mAllocator.Init(&config);
		}

		~TFixedObjectPool()
		{
			for (auto object : mObjectList)
			{
				if (!object)
				{
					HL_ERROR("Object corrupted!\n");
					continue;
				}

				object->~ObjectType();
			}

			Engine::GetInstance()->GetMemoryManager()->Free(mAllocator.GetMemoryPointer());

			mAllocator.Reset();
			mAllocator.Terminate();
		}

		template<typename ... Args>
		ObjectType* CreateObject(Args&& ... InArgs)
		{
			if (mObjectList.size() < MEMORY_BLOCK_CAPACITY)
			{
				ObjectType* object = nullptr;

				void* memory = mAllocator.Allocate(OBJECT_SIZE, OBJECT_ALIGNMENT);

				if (memory)
				{
					new(memory)ObjectType(std::forward<Args>(InArgs)...);
					object = (ObjectType*)memory;
					mObjectList.push_back(object);
				}

				return object;
			}

			return nullptr;
		}

		void DestroyObject(void* InObject)
		{
			if (auto it = std::find(mObjectList.begin(), mObjectList.end(), InObject); it != mObjectList.end())
			{
				((ObjectType*)InObject)->~ObjectType();
				mObjectList.remove((ObjectType*)InObject);
				mAllocator.Free((void*)InObject);
			}
		}

		inline const std::list<ObjectType*> GetObjectList() const noexcept { return mObjectList; }

	private:
		PoolAllocator mAllocator;
		
		std::list<ObjectType*> mObjectList;

	};

}