#pragma once

#include "Engine/Platform.h"
#include "Engine/Memory/Allocator/StackAllocator.h"

namespace Howlite {

	class MemoryManager final {
		static constexpr size_t GLOBAL_MEMORY_SIZE = static_cast<size_t>(1e+9);

	public:
		HL_CLASS_DISABLE_COPY(MemoryManager);
		
		bool Init();
		void Terminate();
		void* Allocate(const size_t InMemorySize, const std::string& InMemoryUser);
		void Free(void* InMemory);

	private:
		StackAllocator mAllocator;

		std::unordered_map<void*, std::string> mMemoryRegistry;

	};

}
