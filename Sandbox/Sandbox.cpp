#include <Engine/Platform.h>
#include <Engine/Engine.h>
#include <Engine/Window.h>
#include <Engine/ECS/SystemManager.h>
#include <Engine/ECS/EntityManager.h>
#include <Engine/ECS/ComponentManager.h>
#include <Engine/Event/EventManager.h>

using namespace Howlite;

struct CloseEvent : public TEvent<CloseEvent> {};
struct TickEvent : public TEvent<TickEvent> {};
struct DrawEvent : public TEvent<DrawEvent> {};

struct MyEntity : public TEntity<MyEntity> {
	uint64_t v0;
	uint64_t v1;
	uint64_t v2;
	uint64_t v3;
	uint64_t v4;
	uint64_t v5;
};

struct MySystem : public TSystem<MySystem> {
	MySystem() : TSystem(Priority::Hight)
	{
		HL_INFO("%s\n", __FUNCTION__);
	}

	~MySystem()
	{
		HL_INFO("%s\n", __FUNCTION__);
	}

};

struct MyComponent : public TComponent<MyComponent> {
};

struct MyComponentEx : public TComponent<MyComponentEx> {
};

void test()
{
	SystemManager* systemManager = Engine::GetInstance()->GetSystemManager();
	systemManager->AddSystem<MySystem>();
}

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	HL_UNUSED(hInstance);
	HL_UNUSED(hPrevInstance);
	HL_UNUSED(lpCmdLine);
	HL_UNUSED(nShowCmd);

	Engine engine;
	Window window;

	bool isRunning = true;

	engine.Init();
	
	engine.GetEventManager()->Subscribe<CloseEvent>("close_event", [&isRunning](const CloseEvent*)
	{
		isRunning = false;
	});

	engine.GetEventManager()->Subscribe<TickEvent>("tick_event", [](const TickEvent*)
	{
		
	});

	engine.GetEventManager()->Subscribe<DrawEvent>("draw_event", [](const DrawEvent*)
	{
		
	});

	window.Init(1280, 720, "Some window");
	window.SetMessageCallback([](HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)->LRESULT
	{
		switch (Msg)
		{
			case WM_CLOSE:
			{
				// do not pass WM_CLOSE to DefWindowProc to avoid window auto-destroy
				Engine::GetInstance()->GetEventManager()->Post<CloseEvent>();
				return 0;
			}
			case WM_KEYDOWN:
				[[fallthrough]];
			case WM_SYSKEYDOWN:
			{
				if ((HIWORD(lParam) & KF_REPEAT) == 0 && wParam == VK_ESCAPE)
				{
					Engine::GetInstance()->GetEventManager()->Post<CloseEvent>();
				}
				break;
			}
		}

		return ::DefWindowProc(hWnd, Msg, wParam, lParam);
	});

	test();

	while (isRunning)
	{
		MSG msg;
		ZeroMemory(&msg, sizeof(msg));
		while (::PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}

		engine.GetEventManager()->Post<TickEvent>();
		engine.GetEventManager()->Post<DrawEvent>();
		engine.GetEventManager()->Notify();
	}

	window.Terminate();
	engine.Terminate();

	return 0;
}