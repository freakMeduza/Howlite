#pragma once

#include "Engine/Engine.h"
#include "Engine/Utilities/TypeRegistry.h"

namespace Howlite {

	class ISystem {
	public:
		enum class Priority : uint8_t {
			Low,
			Medium,
			Normal,
			Hight
		};

		HL_INTERFACE_DISABLE_COPY(ISystem);

		virtual SystemTypeId GetSystemTypeId() const = 0;
		virtual Priority GetPriority() const = 0;

	};

	template<typename SystemType>
	class TSystem : public ISystem {
	public:
		HL_DISABLE_COPY(TSystem);

		TSystem(const Priority InPriority) : mPriority{ InPriority }
		{
		}

		virtual ~TSystem()
		{

		}

		// Inherited via ISystem
		virtual SystemTypeId GetSystemTypeId() const noexcept override { return STATIC_SYSTEM_TYPE_ID; }
		virtual Priority GetPriority() const noexcept override { return mPriority; }
		
		static const SystemTypeId STATIC_SYSTEM_TYPE_ID;
		
	private:
		Priority mPriority;

	};

	template<typename SystemType> const SystemTypeId TSystem<SystemType>::STATIC_SYSTEM_TYPE_ID = TTypeRegistry<ISystem>::GetId<SystemType>();
}