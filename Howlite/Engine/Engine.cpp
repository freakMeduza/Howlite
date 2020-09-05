#include "HowlitePCH.h"
#include "Engine.h"
#include "Window.h"
#include "Event/Event.h"
#include "Event/MouseEvent.h"
#include "Event/WindowEvent.h"
#include "Event/KeyboardEvent.h"
#include "Graphic/GraphicSystem.h"
#include "Graphic/Camera.h"
#include "Input/InputSystem.h"
#include "UI/UISystem.h"

#include "Graphic/Cube.h"
#include "Graphic/Light.h"
#include "Graphic/Model.h"
#include "Common/UUID.h"
#include "Common/Image.h"
#include "Common/String.h"

namespace {
	// TODO: replace with lua scripting settings
	static constexpr uint32_t DEFAULT_WIDTH = 1600u;
	static constexpr uint32_t DEFAULT_HEIGHT = 900u;
}

namespace Howlite {

	HEngine* HEngine::mEngineInstance = nullptr;

	HEngine::HEngine(HINSTANCE Instance)
	{
		H_ASSERT(mEngineInstance == nullptr, "Engine instance already exists.")

		mEngineInstance = this;

		mWindow = CreateScopedPointer<HWindow>(Instance, "Howlite", DEFAULT_WIDTH, DEFAULT_HEIGHT);
		mWindow->SetMessageCallback([this](IEvent& Event)
		{
			HEventDispatcher dispatcher{ Event };
			dispatcher.Dispatch<HKeyPressedEvent>(H_BIND_EVENT(HEngine::OnKeyPressed));
			dispatcher.Dispatch<HMouseRawInputEvent>(H_BIND_EVENT(HEngine::OnMouseRawInput));
			dispatcher.Dispatch<HWindowClosedEvent>(H_BIND_EVENT(HEngine::OnWindowClosed));
			dispatcher.Dispatch<HWindowResizedEvent>(H_BIND_EVENT(HEngine::OnWindowResized));
		});

		mInputSystem = CreateScopedPointer<HInputSystem>();
		mGraphicSystem = CreateScopedPointer<HGraphicSystem>(mWindow->GetWindowHandler(), mWindow->GetWidth(), mWindow->GetHeight());
		mUISystem = CreateScopedPointer<HUISystem>(mWindow->GetWindowHandler(), mGraphicSystem->GetDeviceInternal(), mGraphicSystem->GetContextInternal());
		mUISystem->SetIsEnabled(true);

		const float width = static_cast<float>(GetWindowInstance().GetWidth());
		const float height = static_cast<float>(GetWindowInstance().GetHeight());

		mCamera = CreateScopedPointer<HCamera>(*mGraphicSystem, DirectX::XMMatrixPerspectiveLH(1.0f, height / width, 0.5f, 2000.0f));
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
		HGraphicSystem& graphicSystem = GetGraphicSystemInstance();

		mLight = CreateSharedPointer<HLight>(GetGraphicSystemInstance(), 10.0f);
		mModel = CreateScopedPointer<HModel>(GetGraphicSystemInstance(), "Assets\\monster\\monster.obj");

		mIsRun = true;

		while (mIsRun)
		{
			HWindow::ProcessMessages();

			graphicSystem.BeginFrame(HColor::DarkGray);

			mLight->Bind(GetGraphicSystemInstance());
			mCamera->Bind(GetGraphicSystemInstance());
			mModel->Bind(GetGraphicSystemInstance());
			mModel->Draw(GetGraphicSystemInstance());
			mLight->Draw(GetGraphicSystemInstance());

			// UI Drawing Pass
			if (mUISystem->IsEnabled())
			{
				mUISystem->BeginFrame();

				static bool lightWindowIsOpen = false;
				static bool cameraWindowIsOpen = false;
				static bool materialWindowIsOpen = false;
				static bool demoWindowIsOpen = false;

				// =========================================================================
				const ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_NoDockingInCentralNode | ImGuiDockNodeFlags_PassthruCentralNode;
				const ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoBackground |
					ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
					ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

				ImGuiViewport* viewport = ImGui::GetMainViewport();
				ImGui::SetNextWindowPos(viewport->GetWorkPos());
				ImGui::SetNextWindowSize(viewport->GetWorkSize());
				ImGui::SetNextWindowViewport(viewport->ID);

				ImGui::Begin("DockSpace Demo", (bool*)nullptr, windowFlags);

				// DockSpace
				ImGui::DockSpace(ImGui::GetID("HowliteDockspace"), ImVec2(0.0f, 0.0f), dockspaceFlags);

				// MenuBar
				if (ImGui::BeginMenuBar())
				{
					// File Menu
					if (ImGui::BeginMenu("File"))
					{
						if (ImGui::MenuItem("Exit")) mIsRun = false;
						ImGui::EndMenu();
					}

					// Window Menu
					if (ImGui::BeginMenu("Window"))
					{
						ImGui::MenuItem("Light", nullptr, &lightWindowIsOpen);
						ImGui::MenuItem("Camera", nullptr, &cameraWindowIsOpen);
						ImGui::MenuItem("Material", nullptr, &materialWindowIsOpen);
						ImGui::MenuItem("ImGui Demo", nullptr, &demoWindowIsOpen);
						ImGui::EndMenu();
					}

					ImGui::EndMenuBar();
				}

				ImGui::End();

				if (lightWindowIsOpen) mLight->DrawUIWindow(&lightWindowIsOpen);
				if (cameraWindowIsOpen) mCamera->DrawUIWindow(&cameraWindowIsOpen);
				if (materialWindowIsOpen) mModel->DrawUIWindow(&materialWindowIsOpen);
				if (demoWindowIsOpen) ImGui::ShowDemoWindow(&demoWindowIsOpen);
				// =========================================================================

				mUISystem->EndFrame();
			}

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
		H_ASSERT(mUISystem != nullptr, "Failed to get UI system instance.")
		return *mUISystem;
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
			HUISystem& uiSystem = GetUISystemInstance();
			const bool isEnabled = !uiSystem.IsEnabled();
			uiSystem.SetIsEnabled(isEnabled);
			return true;
		}
		return false;
	}

	bool HEngine::OnMouseRawInput(HMouseRawInputEvent& Event)
	{
		const HInputSystem& inputSystem = GetInputSystemInstance();
		const HInputSystem::HPoint& delta = Event.GetPoint();
		HCamera& camera = GetCameraInstance();
		if (inputSystem.IsKeyPressed(VK_CONTROL))
		{
			if (inputSystem.IsLeftMouseButtonPressed())
			{
				camera.Rotate((float)delta.Y, (float)delta.X);
			}
			else if (inputSystem.IsRightMouseButtonPressed())
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
		GetCameraInstance().SetProjectionMatrix(DirectX::XMMatrixPerspectiveLH(1.0f, height / width, 0.5f, 2000.0f));
		GetGraphicSystemInstance().ResizeBuffers(Event.GetWidth(), Event.GetHeight());
		return true;
	}

}