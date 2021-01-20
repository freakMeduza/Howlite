#pragma once

#include "Entity.h"
#include "EntityPool.h"

namespace Howlite {

	class EntityManager final {
	public:
		HL_CLASS_DISABLE_COPY(EntityManager);
		
		bool Init();
		void Terminate();

		template<typename EntityType, typename ... Args>
		EntityType* CreateEntity(Args&& ... InArgs)
		{
			void* entityMemory = GetEntityPool<EntityType>()->CreateObject();

			const EntityId entityId = CaptureEntityId((EntityType*)entityMemory);

			((EntityType*)entityMemory)->mEntityId = entityId;
			
			EntityType* entity = new (entityMemory)EntityType(std::forward<Args>(InArgs)...);
			
			return entity;
		}

		void ClearPendingRemoveList();
		void DestroyEntity(const EntityId InEntityId);
		IEntity* GetEntity(const EntityId InEntityId);

		inline const std::vector<IEntity*> GetEntityList() const noexcept { return mEntityList; }

	private:
		// Returning entity pool associated to the given entity type 
		// if pool doesn't exist it will be created
		template<typename EntityType>
		TEntityPool<EntityType>* GetEntityPool()
		{
			const auto typeId = EntityType::STATIC_ENTITY_TYPE_ID;

			if (auto it = mEntityPoolMap.find(typeId); it != mEntityPoolMap.end())
			{
				return static_cast<TEntityPool<EntityType>*>(it->second.get());
			}
			else
			{
				mEntityPoolMap.insert({ typeId, std::make_unique<TEntityPool<EntityType>>() });
				return static_cast<TEntityPool<EntityType>*>(mEntityPoolMap[typeId].get());
			}
		}

		static constexpr size_t ARRAY_GROW_SIZE = 1024;
		// NOTE: we will try to find free entity id slot in mEntityList
		// if it failured, mEntityList will be scaled to contains required slot,
		// actualy size of array will grow for ARRAY_GROW_SIZE
		// this will reduce unnessesary memory allocation's count.
		EntityId CaptureEntityId(IEntity* InEntity);
		void ReleaseEntityId(const EntityId InEntityId);
		// NOTE: the index of a entity in this array will be its EntityID,
		// this will provide easy access to the entity N(1)
		std::vector<IEntity*> mEntityList;

		std::list<EntityId> mPendingRemoveList;

		std::unordered_map<EntityTypeId, std::unique_ptr<IEntityPool>> mEntityPoolMap;

	};

}