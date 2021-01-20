#pragma once

#include "Engine/Platform.h"
#include "Engine/Utilities/TypeRegistry.h"

namespace Howlite {

	class IComponent {
	public:
		HL_INTERFACE_DEFAULT(IComponent);
		
		virtual ComponentId GetComponentId() const = 0;
		virtual ComponentTypeId GetComponentTypeId() const = 0;
		virtual EntityId GetParentId() const = 0;

	};

	template<typename ComponentType>
	class TComponent : public IComponent {
		friend class ComponentManager;

	public:
		HL_CLASS_DEFAULT(TComponent);
		// Inherited via IComponent
		virtual ComponentId GetComponentId() const noexcept override { return mComponentId; }
		virtual ComponentTypeId GetComponentTypeId() const noexcept override { return STATIC_COMPONENT_TYPE_ID; }
		virtual EntityId GetParentId() const noexcept override { return mParentId; }

		static const ComponentTypeId STATIC_COMPONENT_TYPE_ID;

	private:
		// NOTE: These properties will be set explicetly by
		// component manager, we shouldn't give them default value
		// in class constructor
		ComponentId mComponentId;
		EntityId mParentId;

	};

	template<typename ComponentType> const ComponentTypeId TComponent<ComponentType>::STATIC_COMPONENT_TYPE_ID = TTypeRegistry<IComponent>::GetId<ComponentType>();
}