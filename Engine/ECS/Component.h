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
		ComponentId mComponentId = HL_INVALID_ID;
		EntityId mParentId = HL_INVALID_ID;

	};

	template<typename ComponentType> const ComponentTypeId TComponent<ComponentType>::STATIC_COMPONENT_TYPE_ID = TTypeRegistry<IComponent>::GetId<ComponentType>();
}