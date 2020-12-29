#pragma once

#include "Component.h"
#include "ComponentPool.h"

namespace Howlite {

	class ComponentManager {
	public:
		HL_CLASS_DISABLE_COPY(ComponentManager);
		
		bool Init();
		void Terminate();

		template<typename ComponentType, typename ... Args>
		ComponentType* AddComponent(const EntityId InEntityId, Args&& ... InArgs)
		{
			ComponentType* component = GetComponentPool<ComponentType>()->CreateObject(std::forward<Args>(InArgs)...);

			const ComponentId componentId = CaptureComponentId(component);

			component->mComponentId = componentId;
			component->mParentId = InEntityId;

			Map(InEntityId, componentId, ComponentType::STATIC_COMPONENT_TYPE_ID);

			return component;
		}

		template<typename ComponentType>
		void RemoveComponent(const EntityId InEntityId)
		{
			const ComponentTypeId componentTypeId = ComponentType::STATIC_COMPONENT_TYPE_ID;
			const ComponentId componentId = mEntityComponentMap[InEntityId][componentTypeId];

			IComponent* component = mComponentList[componentId];

			if (!component)
			{
				HL_ERROR("Error: trying to remove unexisting component");
				return;
			}

			Unmap(InEntityId, componentId, componentTypeId);
			
			ReleaseComponentId(componentId);

			GetComponentPool<ComponentType>()->DestroyObject(component);
		}

		template<typename ComponentType>
		ComponentType* GetComponent(const EntityId InEntityId)
		{
			const ComponentTypeId componentTypeId = ComponentType::STATIC_COMPONENT_TYPE_ID;
			const ComponentId componentId = mEntityComponentMap[InEntityId][componentTypeId];
			return componentId == HL_INVALID_ID ? nullptr : static_cast<ComponentType*>(mComponentList[componentId]);
		}

		// NOTE: will remove all components of specific entity id
		void RemoveAll(const EntityId InEntityID);

	private:
		// Returning component pool associated to the given component type 
		// if pool doesn't exist it will be created
		template<typename ComponentType>
		TComponentPool<ComponentType>* GetComponentPool()
		{
			const auto typeId = ComponentType::STATIC_COMPONENT_TYPE_ID;

			if (auto it = mComponentPoolMap.find(typeId); it != mComponentPoolMap.end())
			{
				return static_cast<TComponentPool<ComponentType>*>(it->second.get());
			}
			else
			{
				mComponentPoolMap.insert({ typeId, std::make_unique<TComponentPool<ComponentType>>() });
				return static_cast<TComponentPool<ComponentType>*>(mComponentPoolMap[typeId].get());
			}
		}

		static constexpr size_t ARRAY_GROW_SIZE = 1024;
		// NOTE: we will try to find free component id slot in mComponentList
		// if it failured, mComponentList will be scaled to contains required slot,
		// actualy size of array will grow for ARRAY_GROW_SIZE
		// this will reduce unnessesary memory allocation's count.
		ComponentId CaptureComponentId(IComponent* InComponent);
		void ReleaseComponentId(const ComponentId InComponentId);

		void Map(const EntityId InEntityId, const ComponentId InComponentId, const ComponentTypeId InComponentTypeId);
		void Unmap(const EntityId InEntityId, const ComponentId InComponentId, const ComponentTypeId InComponentTypeId);

		// NOTE: the index of a component in this array will be its ComponentId,
		// this will provide easy access to the component N(1)
		std::vector<IComponent*> mComponentList;
		// NOTE: the index of parent array is an EntityId so we can easily get all mapped	
		// components array for the specific entity, then use ComponentTypeId as an index to	
		// search ComponentId N(1)
		std::vector<std::vector<ComponentId>> mEntityComponentMap;

		std::unordered_map<ComponentTypeId, std::unique_ptr<IComponentPool>> mComponentPoolMap;

	};

}