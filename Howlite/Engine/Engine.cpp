#include "HowlitePCH.h"
#include "Engine.h"
#include "Window.h"
#include "Event/Event.h"
#include "Event/MouseEvent.h"
#include "Event/WindowEvent.h"
#include "Event/KeyboardEvent.h"
#include "Renderer/GraphicSystem.h"
#include "Renderer/UI/UISystem.h"
#include "Input/InputSystem.h"

namespace Howlite {

	HEngine* HEngine::mEngineInstance = nullptr;

	HEngine::HEngine(HINSTANCE Instance, LPSTR CmdLine)
	{
		H_UNUSED(CmdLine)
		H_ASSERT(mEngineInstance == nullptr, "Engine instance already exists.")

		static constexpr uint32_t width = 1280u;
		static constexpr uint32_t height = 720u;

		mEngineInstance = this;
		
		mWindow = CreateScopedPointer<HWindow>(Instance, "Howlite", width, height);
		mWindow->SetMessageCallback([this](IHEvent& Event)
		{
			HEventDispatcher dispatcher{ Event };

			dispatcher.Dispatch<HKeyPressedEvent>(H_BIND_EVENT_CALLBACK(HEngine::OnKeyPressed));
			dispatcher.Dispatch<HWindowClosedEvent>(H_BIND_EVENT_CALLBACK(HEngine::OnWindowClosed));
			dispatcher.Dispatch<HWindowResizedEvent>(H_BIND_EVENT_CALLBACK(HEngine::OnWindowResized));
		});
	}

	HEngine::~HEngine()
	{
		mEngineInstance = nullptr;
	}

	HEngine& HEngine::GetInstance()
	{
		H_ASSERT(mEngineInstance != nullptr, "Failed to get engine instance.")
		return *mEngineInstance;
	}

	int HEngine::Run()
	{
		mIsRun = true;

		static bool show = true;
		static bool vsync = GetGraphicSystemInstance().IsVSyncEnabled();

		while (mIsRun)
		{
			HWindow::ProcessMessages();

			GetGraphicSystemInstance().BeginFrame(HColor::Black());

			if(ImGui::Begin("Metrics", &show))
			{
				ImGuiIO& io = ImGui::GetIO();
				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
				ImGui::Checkbox("vsync", &vsync);
				ImGui::End();
			}

			GetGraphicSystemInstance().EndFrame();
			GetGraphicSystemInstance().SetVSyncEnabled(vsync);
		}

		return 0;
	}

	HWindow& HEngine::GetWindowInstance()
	{
		H_ASSERT(mWindow != nullptr, "Failed to get window instance.")
		return *mWindow;
	}

	HGraphicSystem& HEngine::GetGraphicSystemInstance()
	{
		return GetWindowInstance().GetGraphicSystemInstance();
	}

	HInputSystem& HEngine::GetInputSystemInstance()
	{
		return GetWindowInstance().GetInputSystemInstance();
	}

	HUISystem& HEngine::GetUISystemInstance()
	{
		return GetWindowInstance().GetGraphicSystemInstance().GetUISystemInstance();
	}

	bool HEngine::OnKeyPressed(HKeyPressedEvent& Event)
	{
		if (Event.GetKey() == VK_ESCAPE)
		{
			mIsRun = false;
			return true;
		}

		return false;
	}

	bool HEngine::OnWindowClosed(HWindowClosedEvent& Event)
	{
		H_UNUSED(Event)
		mIsRun = false;
		return true;
	}

	bool HEngine::OnWindowResized(HWindowResizedEvent& Event)
	{
		H_UNUSED(Event)
		return false;
	}

}