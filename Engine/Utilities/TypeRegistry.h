#pragma once

#include "Engine/Platform.h"

namespace Howlite {

	using TypeId = size_t;

	template<typename ParentType>
	class HL_API TTypeRegistry {
		static TypeId mTypeCount;

	public:
		HL_CLASS_STATIC(TTypeRegistry);

		template<typename Type>
		static const TypeId GetId()
		{
			static const TypeId id = mTypeCount++;
			return id;
		}

		static size_t GetTypeCount()
		{
			return mTypeCount;
		}

	};

}