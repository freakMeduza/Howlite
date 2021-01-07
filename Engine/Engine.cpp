#include "Engine.h"
#include "Memory/MemoryManager.h"
#include "Event/EventManager.h"
#include "ECS/ComponentManager.h"
#include "ECS/EntityManager.h"
#include "ECS/SystemManager.h"
#include "Render/RenderSystem.h"
#include "Input/InputSystem.h"

namespace Howlite {

	Engine* Engine::STATIC_ENGINE_INSTANCE = nullptr;

	bool Engine::Init()
	{
		// NOTE: we have a windows application, so we have to
		// create console for debug output
#ifdef _DEBUG
		::AllocConsole();
		freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
#endif

		STATIC_ENGINE_INSTANCE = this;

		mMemoryManager = std::make_unique<MemoryManager>();
		mEventManager = std::make_unique<EventManager>();
		mComponentManager = std::make_unique<ComponentManager>();
		mEntityManager = std::make_unique<EntityManager>();
		mSystemManager = std::make_unique<SystemManager>();

		const bool isManagersInitialized = mMemoryManager->Init() && mEventManager->Init() && 
			mComponentManager->Init() && mEntityManager->Init() && mSystemManager->Init();

		const bool isSystemsInitialized = mSystemManager->AddSystem<RenderSystem>() && mSystemManager->AddSystem<InputSystem>();

		return isManagersInitialized && isSystemsInitialized;
	}

	void Engine::Terminate()
	{
		mSystemManager->Terminate();
		mEntityManager->Terminate();
		mComponentManager->Terminate();
		mEventManager->Terminate();
		mMemoryManager->Terminate();

		STATIC_ENGINE_INSTANCE = nullptr;

#ifdef _DEBUG
		::FreeConsole();
		fclose(stdout);
#endif
	}

	Engine* Engine::GetInstance() noexcept
	{
		return STATIC_ENGINE_INSTANCE;
	}

	MemoryManager* Engine::GetMemoryManager() const
	{
		HL_ASSERT(mMemoryManager, "Memory manager instance is not initialized");
		return mMemoryManager.get();
	}

	EventManager* Engine::GetEventManager() const
	{
		HL_ASSERT(mEventManager, "Event manager instance is not initialized");
		return mEventManager.get();
	}

	ComponentManager* Engine::GetComponentManager() const
	{
		HL_ASSERT(mComponentManager, "Component manager instance is not initialized");
		return mComponentManager.get();
	}

	EntityManager* Engine::GetEntityManager() const
	{
		HL_ASSERT(mEntityManager, "Entity manager instance is not initialized");
		return mEntityManager.get();
	}

	SystemManager* Engine::GetSystemManager() const
	{
		HL_ASSERT(mSystemManager, "System manager instance is not initialized");
		return mSystemManager.get();
	}

}