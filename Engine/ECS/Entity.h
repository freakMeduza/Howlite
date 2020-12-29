#pragma once

#include "Engine/Engine.h"
#include "Engine/Utilities/TypeRegistry.h"

namespace Howlite {

	class IEntity {
	public:
		HL_INTERFACE_DEFAULT(IEntity);

		virtual EntityId GetEntityId() const = 0;
		virtual EntityTypeId GetEntityTypeId() const = 0;

		template<typename ComponentType, typename ... Args>
		ComponentType* AddComponent(Args&& ... InArgs)
		{
			return Engine::GetInstance()->GetComponentManager()->AddComponent<ComponentType>(GetEntityId(), std::forward<Args>(InArgs)...);
		}

		template<typename ComponentType>
		void RemoveComponent()
		{
			return Engine::GetInstance()->GetComponentManager()->RemoveComponent<ComponentType>(GetEntityId());
		}

		template<typename ComponentType>
		ComponentType* GetComponent()
		{
			return Engine::GetInstance()->GetComponentManager()->GetComponent<ComponentType>(GetEntityId());
		}
	};

	template<typename EntityType>
	class TEntity : public IEntity {
		friend class EntityManager;

	public:
		HL_CLASS_DEFAULT(TEntity);
		// Inherited via IEntity
		virtual EntityId GetEntityId() const noexcept override { return mEntityId; }
		virtual EntityTypeId GetEntityTypeId() const noexcept override { return STATIC_ENTITY_TYPE_ID; }
		
		static const EntityTypeId STATIC_ENTITY_TYPE_ID;

	private:
		EntityId mEntityId = HL_INVALID_ID;

	};

	template<typename EntityType> const EntityTypeId TEntity<EntityType>::STATIC_ENTITY_TYPE_ID = TTypeRegistry<IEntity>::GetId<EntityType>();
}