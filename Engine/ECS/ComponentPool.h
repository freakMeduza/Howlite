#pragma once

#include "Engine/Utilities/ObjectPool.h"

namespace Howlite {

	class IComponentPool {
	public:
		HL_INTERFACE_DISABLE_COPY(IComponentPool);

		virtual void DestoryComponent(class IComponent* InComponent) = 0;
	};

	template<typename ComponentType>
	class TComponentPool : public IComponentPool, public TFixedObjectPool<ComponentType, 1000> {
	public:
		HL_DISABLE_COPY(TComponentPool);

		TComponentPool() : TFixedObjectPool{ std::string{"ComponentPool_"} + std::to_string(ComponentType::STATIC_COMPONENT_TYPE_ID) }
		{
		}

		// Inherited via IComponentPool
		virtual void DestoryComponent(IComponent* InComponent) override
		{
			this->DestroyObject((void*)InComponent);
		}

	};

}