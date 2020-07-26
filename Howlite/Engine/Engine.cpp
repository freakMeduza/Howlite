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

#include "Common/Buffer.h"
#include "Renderer/BindableCommon.h"

namespace {

	std::tuple<Howlite::HBuffer, std::vector<uint32_t>> GetCube() noexcept 
	{
		Howlite::HBuffer buffer{ Howlite::HLayout{Howlite::HEAttributeType::Position3D}, 24 };

		static constexpr float side = 1.0f;

		buffer[0].SetAttribute<Howlite::HEAttributeType::Position3D>(DirectX::XMFLOAT3{ -side, -side, -side });  // 0 near side
		buffer[1].SetAttribute<Howlite::HEAttributeType::Position3D>(DirectX::XMFLOAT3{  side, -side, -side });  // 1
		buffer[2].SetAttribute<Howlite::HEAttributeType::Position3D>(DirectX::XMFLOAT3{ -side,  side, -side });  // 2
		buffer[3].SetAttribute<Howlite::HEAttributeType::Position3D>(DirectX::XMFLOAT3{  side,  side, -side });  // 3
		buffer[4].SetAttribute<Howlite::HEAttributeType::Position3D>(DirectX::XMFLOAT3{ -side, -side,  side });  // 4 far side
		buffer[5].SetAttribute<Howlite::HEAttributeType::Position3D>(DirectX::XMFLOAT3{  side, -side,  side });  // 5
		buffer[6].SetAttribute<Howlite::HEAttributeType::Position3D>(DirectX::XMFLOAT3{ -side,  side,  side });  // 6
		buffer[7].SetAttribute<Howlite::HEAttributeType::Position3D>(DirectX::XMFLOAT3{  side,  side,  side });  // 7
		buffer[8].SetAttribute<Howlite::HEAttributeType::Position3D>(DirectX::XMFLOAT3{ -side, -side, -side });  // 8 left side
		buffer[9].SetAttribute<Howlite::HEAttributeType::Position3D>(DirectX::XMFLOAT3{ -side,  side, -side });  // 9
		buffer[10].SetAttribute<Howlite::HEAttributeType::Position3D>(DirectX::XMFLOAT3{ -side, -side,  side }); // 10
		buffer[11].SetAttribute<Howlite::HEAttributeType::Position3D>(DirectX::XMFLOAT3{ -side,  side,  side }); // 11
		buffer[12].SetAttribute<Howlite::HEAttributeType::Position3D>(DirectX::XMFLOAT3{  side, -side, -side }); // 12 right side
		buffer[13].SetAttribute<Howlite::HEAttributeType::Position3D>(DirectX::XMFLOAT3{  side,  side, -side }); // 13
		buffer[14].SetAttribute<Howlite::HEAttributeType::Position3D>(DirectX::XMFLOAT3{  side, -side,  side }); // 14
		buffer[15].SetAttribute<Howlite::HEAttributeType::Position3D>(DirectX::XMFLOAT3{  side,  side,  side }); // 15
		buffer[16].SetAttribute<Howlite::HEAttributeType::Position3D>(DirectX::XMFLOAT3{ -side, -side, -side }); // 16 bottom side
		buffer[17].SetAttribute<Howlite::HEAttributeType::Position3D>(DirectX::XMFLOAT3{  side, -side, -side }); // 17
		buffer[18].SetAttribute<Howlite::HEAttributeType::Position3D>(DirectX::XMFLOAT3{ -side, -side,  side }); // 18
		buffer[19].SetAttribute<Howlite::HEAttributeType::Position3D>(DirectX::XMFLOAT3{  side, -side,  side }); // 19
		buffer[20].SetAttribute<Howlite::HEAttributeType::Position3D>(DirectX::XMFLOAT3{ -side,  side, -side }); // 20 top side
		buffer[21].SetAttribute<Howlite::HEAttributeType::Position3D>(DirectX::XMFLOAT3{  side,  side, -side }); // 21
		buffer[22].SetAttribute<Howlite::HEAttributeType::Position3D>(DirectX::XMFLOAT3{ -side,  side,  side }); // 22
		buffer[23].SetAttribute<Howlite::HEAttributeType::Position3D>(DirectX::XMFLOAT3{  side,  side,  side }); // 23

		std::vector<uint32_t> indices{
			 0,  2,  1,  2,  3,  1,
			 4,  5,  7,  4,  7,  6,
			 8, 10,  9, 10, 11,  9,
			12, 13, 15, 12, 15, 14,
			16, 17, 18, 18, 17, 19,
			20, 23, 21, 20, 22, 23
		};

		return { std::move(buffer), std::move(indices) };
	}
}

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

		auto [buffer, indices] = GetCube();

		SharedPointer<HVertexBuffer> vertexBuffer = CreateSharedPointer<HVertexBuffer>(GetGraphicSystemInstance(), buffer);
		SharedPointer<HIndexBuffer> indexBuffer = CreateSharedPointer<HIndexBuffer>(GetGraphicSystemInstance(), indices);
		SharedPointer<HVertexShader> vertexShader = CreateSharedPointer<HVertexShader>(GetGraphicSystemInstance(), L"VertexShader.cso");
		SharedPointer<HPixelShader> pixelShader = CreateSharedPointer<HPixelShader>(GetGraphicSystemInstance(), L"PixelShader.cso");
		SharedPointer<HTopology> topology = CreateSharedPointer<HTopology>(GetGraphicSystemInstance(), D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		SharedPointer<HInputLayout> inputLayout = CreateSharedPointer<HInputLayout>(GetGraphicSystemInstance(), buffer.GetLayout(), vertexShader->GetShaderBytecode());

		const HWindow& window = GetWindowInstance();

		struct Transform
		{
			DirectX::XMMATRIX ModelMatrix;
			DirectX::XMMATRIX ViewMatrix;
			DirectX::XMMATRIX ProjectionMatrix;
		};

		Transform transform;
		transform.ModelMatrix = DirectX::XMMatrixTranspose(DirectX::XMMatrixIdentity());
		transform.ViewMatrix = DirectX::XMMatrixTranspose(DirectX::XMMatrixLookAtLH(DirectX::XMVectorSet(0.0f, 0.0f, -10.0f, 0.0f),
														  DirectX::XMVectorZero(),
														  DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)));
		transform.ProjectionMatrix = DirectX::XMMatrixTranspose(DirectX::XMMatrixPerspectiveLH(1.0f, (float)window.GetHeight() / (float)window.GetWidth() , 0.5f, 100.0f));

		SharedPointer<HVertexConstantBuffer<Transform>> vertexConstantBuffer = CreateSharedPointer<HVertexConstantBuffer<Transform>>(GetGraphicSystemInstance(), transform, 0u);

		vertexBuffer->Bind(GetGraphicSystemInstance());
		indexBuffer->Bind(GetGraphicSystemInstance());
		vertexShader->Bind(GetGraphicSystemInstance());
		pixelShader->Bind(GetGraphicSystemInstance());
		topology->Bind(GetGraphicSystemInstance());
		inputLayout->Bind(GetGraphicSystemInstance());
		vertexConstantBuffer->Bind(GetGraphicSystemInstance());

		while (mIsRun)
		{
			HWindow::ProcessMessages();

			vertexBuffer->Bind(GetGraphicSystemInstance());
			indexBuffer->Bind(GetGraphicSystemInstance());
			vertexShader->Bind(GetGraphicSystemInstance());
			pixelShader->Bind(GetGraphicSystemInstance());
			topology->Bind(GetGraphicSystemInstance());
			inputLayout->Bind(GetGraphicSystemInstance());
			vertexConstantBuffer->Bind(GetGraphicSystemInstance());
			const float width = (float)window.GetWidth();
			const float height = (float)window.GetHeight();
			transform.ModelMatrix = DirectX::XMMatrixRotationRollPitchYaw(1.0f, 0.0f, 0.5);
			transform.ProjectionMatrix = DirectX::XMMatrixTranspose(DirectX::XMMatrixPerspectiveLH(1.0f, height / width, 0.5f, 100.0f));
			vertexConstantBuffer->Update(GetGraphicSystemInstance(), transform);

			GetGraphicSystemInstance().BeginFrame(HColor::Black());
			GetGraphicSystemInstance().DrawIndexed(indexBuffer->GetIndicesCount());

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