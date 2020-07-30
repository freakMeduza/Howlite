#include "HowlitePCH.h"
#include "Engine.h"
#include "Window.h"
#include "Event/Event.h"
#include "Event/MouseEvent.h"
#include "Event/WindowEvent.h"
#include "Event/KeyboardEvent.h"
#include "Renderer/GraphicSystem.h"
#include "Renderer/UI/UISystem.h"
#include "Renderer/Camera.h"
#include "Input/InputSystem.h"

#include "Renderer/Cube.h"

namespace Howlite {

	HEngine* HEngine::mEngineInstance = nullptr;

	HEngine::HEngine(HINSTANCE Instance)
	{
		H_ASSERT(mEngineInstance == nullptr, "Engine instance already exists.")

		static constexpr uint32_t width = 1280u;
		static constexpr uint32_t height = 720u;

		mEngineInstance = this;
		
		mWindow = CreateScopedPointer<HWindow>(Instance, "Howlite", width, height);
		mWindow->SetMessageCallback([this](IHEvent& Event)
		{
			HEventDispatcher dispatcher{ Event };
			dispatcher.Dispatch<HKeyPressedEvent>(H_BIND_EVENT_CALLBACK(HEngine::OnKeyPressed));
			dispatcher.Dispatch<HMouseRawInputEvent>(H_BIND_EVENT_CALLBACK(HEngine::OnMouseRawInput));
			dispatcher.Dispatch<HWindowClosedEvent>(H_BIND_EVENT_CALLBACK(HEngine::OnWindowClosed));
			dispatcher.Dispatch<HWindowResizedEvent>(H_BIND_EVENT_CALLBACK(HEngine::OnWindowResized));
		});

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

		const DirectX::XMMATRIX& projectionMatrix = DirectX::XMMatrixPerspectiveLH(1.0f,
																				   static_cast<float>(GetWindowInstance().GetHeight()) / static_cast<float>(GetWindowInstance().GetWidth()),
																				   0.5f,
																				   100.0f);
		mCamera = CreateScopedPointer<HCamera>(projectionMatrix);
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

		HCube cube{ "Cube Entity", GetGraphicSystemInstance() };

		while (mIsRun)
		{
			HWindow::ProcessMessages();

			GetGraphicSystemInstance().BeginFrame(HColor::DarkGray);

			cube.Update(0.0f);
			cube.Draw(GetGraphicSystemInstance());

			GetGraphicSystemInstance().EndFrame();
			GetGraphicSystemInstance().SetVSyncEnabled(mVSyncIsEnabled);
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
		H_UNUSED(Event)
		return false;
	}

}