#pragma once

#include "Engine/Utilities/ObjectPool.h"

namespace Howlite {

	class IEntityPool {
	public:
		HL_INTERFACE_DISABLE_COPY(IEntityPool);

		virtual void DestroyEntity(class IEntity* InComponent) = 0;
	};

	template<typename EntityType>
	class TEntityPool : public IEntityPool, public TFixedObjectPool<EntityType, 1000> {
	public:
		HL_DISABLE_COPY(TEntityPool);

		TEntityPool() : TFixedObjectPool{ std::string{"EntityPool_"} + std::to_string(EntityType::STATIC_ENTITY_TYPE_ID) }
		{
		}

		// Inherited via IEntityPool
		virtual void DestroyEntity(IEntity* InEntity) override
		{
			this->DestroyObject((void*)InEntity);
		}

	};

}