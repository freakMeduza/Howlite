#include "HowlitePCH.h"
#include "Engine.h"
#include "Window.h"
#include "Event/Event.h"
#include "Event/MouseEvent.h"
#include "Event/WindowEvent.h"
#include "Event/KeyboardEvent.h"
#include "Graphic/GraphicSystem.h"
#include "Graphic/UI/UISystem.h"
#include "Graphic/Camera.h"
#include "Input/InputSystem.h"

#include "Graphic/Cube.h"
#include "Graphic/Light.h"
#include "Common/Image.h"

namespace {
	// TODO: replace with lua scripting settings
	static constexpr uint32_t DEFAULT_WIDTH = 1280u;
	static constexpr uint32_t DEFAULT_HEIGHT = 720u;
}

namespace Howlite {

	HEngine* HEngine::mEngineInstance = nullptr;

	HEngine::HEngine(HINSTANCE Instance)
	{
		H_ASSERT(mEngineInstance == nullptr, "Engine instance already exists.")

		mEngineInstance = this;
		
		mWindow = CreateScopedPointer<HWindow>(Instance, "Howlite", DEFAULT_WIDTH, DEFAULT_HEIGHT);
		mWindow->SetMessageCallback([this](IHEvent& Event)
		{
			HEventDispatcher dispatcher{ Event };
			dispatcher.Dispatch<HKeyPressedEvent>(H_BIND_EVENT_CALLBACK(HEngine::OnKeyPressed));
			dispatcher.Dispatch<HMouseRawInputEvent>(H_BIND_EVENT_CALLBACK(HEngine::OnMouseRawInput));
			dispatcher.Dispatch<HWindowClosedEvent>(H_BIND_EVENT_CALLBACK(HEngine::OnWindowClosed));
			dispatcher.Dispatch<HWindowResizedEvent>(H_BIND_EVENT_CALLBACK(HEngine::OnWindowResized));
		});

		mInputSystem = CreateScopedPointer<HInputSystem>();
		mGraphicSystem = CreateScopedPointer<HGraphicSystem>(mWindow->GetWindowHandler(), mWindow->GetWidth(), mWindow->GetHeight());

		GetUISystemInstance().BindUIComponent(HUISystem::CreateUIComponent([this]()
		{
			if (ImGui::Begin("Metrics", (bool*)nullptr))
			{
				ImGuiIO& io = ImGui::GetIO();
				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
				ImGui::Checkbox("vsync", &mVSyncIsEnabled);
				ImGui::End();
			}
		}));

		const float width = static_cast<float>(GetWindowInstance().GetWidth());
		const float height = static_cast<float>(GetWindowInstance().GetHeight());
		mCamera = CreateScopedPointer<HCamera>(DirectX::XMMatrixPerspectiveLH(1.0f, height / width, 0.5f, 100.0f));
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

		HGraphicSystem& graphicSystem = GetGraphicSystemInstance();

		HLight light{ GetGraphicSystemInstance(), 0.3f };
		HCube cube{ GetGraphicSystemInstance() };

		while (mIsRun)
		{
			HWindow::ProcessMessages();

			graphicSystem.BeginFrame(HColor::DarkGray);

			light.Bind(GetGraphicSystemInstance());
			cube.Draw(GetGraphicSystemInstance());
			light.Draw(GetGraphicSystemInstance());

			graphicSystem.EndFrame(mVSyncIsEnabled);
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
		H_ASSERT(mGraphicSystem != nullptr, "Failed to get graphic system instance.")
		return *mGraphicSystem;
	}

	HInputSystem& HEngine::GetInputSystemInstance()
	{
		H_ASSERT(mInputSystem != nullptr, "Failed to get input system instance.")
		return *mInputSystem;
	}

	HUISystem& HEngine::GetUISystemInstance()
	{
		return mGraphicSystem->GetUISystemInstance();
	}

	HCamera& HEngine::GetCameraInstance()
	{
		H_ASSERT(mCamera != nullptr, "Failed to get camera instance.")
		return *mCamera;
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

	bool HEngine::OnMouseRawInput(HMouseRawInputEvent& Event)
	{
		const HInputSystem& inputSystem = GetInputSystemInstance();
		const HInputSystem::HPoint& delta = Event.GetPoint();
		HCamera& camera = GetCameraInstance();
		if(inputSystem.IsKeyPressed(VK_MENU))
		{
			if(inputSystem.IsLeftMouseButtonPressed())
			{
				camera.Rotate((float)delta.Y, (float)delta.X);
			}
			else if(inputSystem.IsRightMouseButtonPressed())
			{
				camera.Translate((float)delta.Y);
			}

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
		const float width = static_cast<float>(Event.GetWidth());
		const float height = static_cast<float>(Event.GetHeight());
		GetCameraInstance().SetProjectionMatrix(DirectX::XMMatrixPerspectiveLH(1.0f, height / width, 0.5f, 100.0f));
		GetGraphicSystemInstance().ResizeBuffers(Event.GetWidth(), Event.GetHeight());
		return true;
	}

}