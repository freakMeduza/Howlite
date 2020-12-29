#pragma once

#include "Platform.h"

namespace Howlite {

	class MemoryManager;
	class EventManager;
	class ComponentManager;
	class EntityManager;
	class SystemManager;

	class Engine {
		static Engine* STATIC_ENGINE_INSTANCE;

	public:
		HL_CLASS_DISABLE_COPY(Engine);

		bool Init();
		void Terminate();

		static Engine* GetInstance() noexcept;
		
		MemoryManager* GetMemoryManager() const;
		EventManager* GetEventManager() const;
		ComponentManager* GetComponentManager() const;
		EntityManager* GetEntityManager() const;
		SystemManager* GetSystemManager() const;

	private:
		std::unique_ptr<MemoryManager> mMemoryManager = nullptr;
		std::unique_ptr<EventManager> mEventManager = nullptr;
		std::unique_ptr<ComponentManager> mComponentManager = nullptr;
		std::unique_ptr<EntityManager> mEntityManager = nullptr;
		std::unique_ptr<SystemManager> mSystemManager = nullptr;
	};

}