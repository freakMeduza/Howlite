#include "ComponentManager.h"

namespace Howlite {

	bool ComponentManager::Init()
	{
		static const size_t typeCount = TTypeRegistry<IComponent>::GetTypeCount();
		// NOTE: just give some inital space for contaners
		mComponentList.resize(ARRAY_GROW_SIZE, nullptr);
		mEntityComponentMap.resize(ARRAY_GROW_SIZE);
		for (auto i = 0; i < ARRAY_GROW_SIZE; ++i)
		{
			mEntityComponentMap[i].resize(typeCount, HL_INVALID_ID);
		}
		return true;
	}

	void ComponentManager::Terminate()
	{
		mComponentList.clear();
		mEntityComponentMap.clear();
		mComponentPoolMap.clear();
	}

	void ComponentManager::RemoveAll(const EntityId InEntityId)
	{
		HL_ASSERT(false, "void ComponentManager::RemoveAll(const EntityId InEntityID) is not implemented");

		static const size_t typeCount = TTypeRegistry<IComponent>::GetTypeCount();
		
		for (ComponentTypeId componentTypeId = 0; componentTypeId < typeCount; ++componentTypeId)
		{
			const ComponentId componentId = mEntityComponentMap[InEntityId][componentTypeId];

			if (componentId == HL_INVALID_ID)
			{
				continue;
			}

			IComponent* component = mComponentList[componentId];
			if (component)
			{
				auto it = mComponentPoolMap.find(componentTypeId);
				HL_ASSERT(it != mComponentPoolMap.end(), "Trying to release component that wasn't created by Component Manager");
				it->second->DestoryComponent(component);
				Unmap(InEntityId, componentId, componentTypeId);
				ReleaseComponentId(componentId);
			}
		}
	}

	ComponentId ComponentManager::CaptureComponentId(IComponent* InComponent)
	{
		size_t id = 0;
		for (; id < mComponentList.size(); ++id)
		{
			if (mComponentList[id] == nullptr)
			{
				mComponentList[id] = InComponent;
				return id;
			}
		}

		mComponentList.resize(mComponentList.size() + ARRAY_GROW_SIZE, nullptr);
		mComponentList[id] = InComponent;
		return id;
	}

	void ComponentManager::ReleaseComponentId(const ComponentId InComponentId)
	{
		HL_ASSERT(InComponentId != HL_INVALID_ID && InComponentId < mComponentList.size(), "Invalid component Id");
		mComponentList[InComponentId] = nullptr;
	}

	void ComponentManager::Map(const EntityId InEntityId, const ComponentId InComponentId, const ComponentTypeId InComponentTypeId)
	{
		// NOTE: this value will be calculated at runtime, so	
		// we will know the total number of types	
		// we have for specific ParentType
		static const size_t typeCount = TTypeRegistry<IComponent>::GetTypeCount();

		if (mEntityComponentMap.size() - 1 < InEntityId)
		{
			const size_t oldSize = mEntityComponentMap.size();
			const size_t newSize = oldSize + ARRAY_GROW_SIZE;

			for (auto i = oldSize; i < newSize; ++i)
			{
				mEntityComponentMap[i].resize(typeCount, HL_INVALID_ID);
			}
		}

		mEntityComponentMap[InEntityId][InComponentTypeId] = InComponentId;
	}

	void ComponentManager::Unmap(const EntityId InEntityId, const ComponentId InComponentId, const ComponentTypeId InComponentTypeId)
	{
		HL_ASSERT(InComponentId != HL_INVALID_ID, "Invalid component Id");
		if (mEntityComponentMap[InEntityId][InComponentTypeId] == InComponentId)
		{
			mEntityComponentMap[InEntityId][InComponentTypeId] = HL_INVALID_ID;
			return;
		}
		HL_ERROR("Component mapping invalid!");
	}

}