#include "EntityManager.h"
#include "Engine/Engine.h"
#include "ComponentManager.h"

namespace Howlite {

	bool EntityManager::Init()
	{
		mEntityList.resize(ARRAY_GROW_SIZE, nullptr);
		return true;
	}

	void EntityManager::Terminate()
	{
		ClearPendingRemoveList();
		mEntityList.clear();
		mEntityPoolMap.clear();
	}

	void EntityManager::ClearPendingRemoveList()
	{
		for (const EntityId& entityId : mPendingRemoveList)
		{
			IEntity* entity = mEntityList[entityId];

			if (auto it = mEntityPoolMap.find(entity->GetEntityTypeId()); it != mEntityPoolMap.end())
			{
				Engine::GetInstance()->GetComponentManager()->RemoveAll(entityId);
				it->second->DestroyEntity(entity);
			}

			ReleaseEntityId(entityId);
		}
		// TODO: we have to keep allocated memory and just track
		// index of last element in array to avoid unnecessary memory allocations
		mPendingRemoveList.clear();
	}

	void EntityManager::DestroyEntity(const EntityId InEntityId)
	{
		mPendingRemoveList.push_back(InEntityId);
	}

	IEntity* EntityManager::GetEntity(const EntityId InEntityId)
	{
		return mEntityList[InEntityId];
	}

	EntityId EntityManager::CaptureEntityId(IEntity* InEntity)
	{
		size_t id = 0;
		for (; id < mEntityList.size(); ++id)
		{
			if (mEntityList[id] == nullptr)
			{
				mEntityList[id] = InEntity;
				return id;
			}
		}
		mEntityList.resize(mEntityList.size() + ARRAY_GROW_SIZE, nullptr);
		mEntityList[id] = InEntity;
		return id;
	}

	void EntityManager::ReleaseEntityId(const EntityId InEntityId)
	{
		HL_ASSERT(InEntityId != HL_INVALID_ID && InEntityId < mEntityList.size(), "Invalid entity Id");
		mEntityList[InEntityId] = nullptr;
	}

}