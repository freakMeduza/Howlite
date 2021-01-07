#include <Engine/Platform.h>
#include <Engine/Engine.h>
#include <Engine/Window.h>
#include <Engine/ECS/SystemManager.h>
#include <Engine/ECS/EntityManager.h>
#include <Engine/ECS/ComponentManager.h>
#include <Engine/Event/EventManager.h>
#include <Engine/Input/InputSystem.h>
#include <Engine/Render/RenderSystem.h>
#include <Engine/Render/VertexBuffer.h>
#include <Engine/Render/IndexBuffer.h>
#include <Engine/Render/VertexShader.h>
#include <Engine/Render/PixelShader.h>
#include <Engine/Render/InputLayout.h>
#include <Engine/Render/Topology.h>
#include <Engine/Render/ConstantBuffer.h>
#include <Engine/Scene/Camera.h>

#include <utility>
#include <algorithm>

using namespace Howlite;

struct TickEvent : public TEvent<TickEvent> {};
struct DrawEvent : public TEvent<DrawEvent> {};

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
	window.Init(1280, 720, "Some window", Window::Style::Close | Window::Style::Title);
	window.SetCursor(false);

	const float width = static_cast<float>(window.GetWidth());
	const float height = static_cast<float>(window.GetHeight());

	PerspectiveCamera camera;
	camera.SetPosition({ 0.0f, 0.0f, -5.0f });
	camera.SetFieldOfView(75.0f);
	camera.SetNearPlane(0.1f);
	camera.SetFarPlane(100.0f);
	camera.SetAspectRatio(width / height);
	
	std::shared_ptr<VertexBuffer> vertexBuffer = nullptr;
	std::shared_ptr<IndexBuffer> indexBuffer = nullptr;
	std::shared_ptr<VertexShader> vertexShader = nullptr;
	std::shared_ptr<PixelShader> pixelShader = nullptr;
	std::shared_ptr<InputLayout> inputLayout = nullptr;
	std::shared_ptr<Topology> topology = nullptr;
	
	struct Vertex {
		DirectX::XMFLOAT3 mPosition;
		DirectX::XMFLOAT3 mColor;

		inline static std::vector<D3D11_INPUT_ELEMENT_DESC> GetLayoutDescription() noexcept
		{
			return {
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			};
		}
	};

	std::vector<Vertex> vertexList = {
		{{-0.5f,  0.5f, -0.5f}, {0.5f, 0.0f, 0.0f}},
		{{ 0.5f,  0.5f, -0.5f}, {0.0f, 0.5f, 0.0f}},
		{{ 0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 0.5f}},
		{{-0.5f,  0.5f,  0.5f}, {0.5f, 0.5f, 0.0f}},
		{{-0.5f, -0.5f, -0.5f}, {0.0f, 0.5f, 0.5f}},
		{{ 0.5f, -0.5f, -0.5f}, {0.5f, 0.5f, 0.5f}},
		{{ 0.5f, -0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}},
		{{-0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 0.0f}},
	};

	std::vector<uint32_t> indexList = {
		3,1,0, 2,1,3,
		0,5,4, 1,5,0,
		3,4,7, 0,4,3,
		1,6,5, 2,6,1,
		2,7,6, 3,7,2,
		6,4,5, 7,4,6,
	};

	RenderSystem* renderSystem = Engine::GetInstance()->GetSystemManager()->GetSystem<RenderSystem>();
	Device* device = renderSystem->GetDevice();

	vertexBuffer = std::make_shared<VertexBuffer>(device, vertexList);
	indexBuffer = std::make_shared<IndexBuffer>(device, indexList);

	auto vertexShaderByteCode = renderSystem->CompileShader(L"test_vs.hlsl", "main", "vs_5_0");
	auto pixelShaderByteCode = renderSystem->CompileShader(L"test_ps.hlsl", "main", "ps_5_0");
	HL_ASSERT(vertexShaderByteCode, "Failed to compile vertex shader");
	HL_ASSERT(pixelShaderByteCode, "Failed to compile pixel shader");
	vertexShader = std::make_shared<VertexShader>(device, vertexShaderByteCode);
	pixelShader = std::make_shared<PixelShader>(device, pixelShaderByteCode);

	inputLayout = std::make_shared<InputLayout>(device, Vertex::GetLayoutDescription(), vertexShaderByteCode);
	topology = std::make_shared<Topology>(device, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	struct alignas(16) TransformBuffer {
		DirectX::XMMATRIX mWorldMatrix;
		DirectX::XMMATRIX mViewMatrix;
		DirectX::XMMATRIX mProjectionMatrix;
	};

	TransformBuffer transformBuffer;
	transformBuffer.mWorldMatrix = DirectX::XMMatrixTranspose(DirectX::XMMatrixIdentity());
	transformBuffer.mViewMatrix = DirectX::XMMatrixTranspose(camera.GetViewMatrix());
	transformBuffer.mProjectionMatrix = DirectX::XMMatrixTranspose(camera.GetProjectionMatrix());

	std::shared_ptr<TConstantBuffer<TransformBuffer, ShaderStage::Vertex, 0>> constantBuffer = nullptr;

	constantBuffer = std::make_shared<TConstantBuffer<TransformBuffer, ShaderStage::Vertex, 0>>(device, transformBuffer);

	std::vector<std::shared_ptr<IBindable>> renderable;
	renderable.push_back(vertexBuffer);
	renderable.push_back(indexBuffer);
	renderable.push_back(vertexShader);
	renderable.push_back(pixelShader);
	renderable.push_back(inputLayout);
	renderable.push_back(topology);
	renderable.push_back(constantBuffer);

	engine.GetEventManager()->Subscribe<MouseRawInputEvent>("mouse_raw_input_event", [&camera, &window](const MouseRawInputEvent* InEvent)
	{
		using namespace DirectX;

		static float yaw = 0.0f;
		static float pitch = 0.0f;

		InputSystem* inputSystem = Engine::GetInstance()->GetSystemManager()->GetSystem<InputSystem>();

		if (inputSystem->GetMouse().IsRightButtonPressed())
		{
			const float width = static_cast<float>(window.GetWidth());
			const float height = static_cast<float>(window.GetHeight());

			const float x = std::min(width / 1000.0f, 2.4f); // max = 2.4f
			const float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;
			
			const float y = std::min(height / 1000.0f, 2.4f); // max = 2.4f
			const float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

			yaw = yaw + InEvent->mDeltaX * xFactor;
			pitch = pitch + InEvent->mDeltaY * yFactor;
		
			camera.SetRotation(XMQuaternionRotationRollPitchYaw(-XMConvertToRadians(pitch), -XMConvertToRadians(yaw), 0.0f));
		}
	});

	engine.GetEventManager()->Subscribe<WindowCloseEvent>("window_close_event", [&isRunning](const WindowCloseEvent*)
	{
		isRunning = false;
	});

	engine.GetEventManager()->Subscribe<TickEvent>("tick_event", [&constantBuffer, &transformBuffer, &camera, &isRunning](const TickEvent*)
	{
		static float elapsedTime = 0.0f;
		static float deltaTime = 0.0f;
		static ULONGLONG timeStart = 0;
		static ULONGLONG timeLast = 0;
		ULONGLONG timeCur = ::GetTickCount64();
		if (timeStart == 0)
		{
			timeStart = timeCur;
		}
		elapsedTime = (timeCur - timeStart) / 1000.0f;
		deltaTime = (timeCur - timeLast) / 1000.0f;
		timeLast = timeCur;

		InputSystem* inputSystem = Engine::GetInstance()->GetSystemManager()->GetSystem<InputSystem>();
		RenderSystem* renderSystem = Engine::GetInstance()->GetSystemManager()->GetSystem<RenderSystem>();

		if (inputSystem->GetKeyboard().IsKeyPressed(VK_ESCAPE))
		{
			isRunning = false;
		}

		if (inputSystem->GetMouse().IsRightButtonPressed())
		{
			DirectX::XMFLOAT3 translation = { 0.0f, 0.0f, 0.0f };

			const float speed = 10.0f * deltaTime;

			if (inputSystem->GetKeyboard().IsKeyPressed('W'))
			{
				translation = { 0.0f, 0.0f, speed };
			}
			else if (inputSystem->GetKeyboard().IsKeyPressed('S'))
			{
				translation = { 0.0f, 0.0f, -speed };
			}

			if (inputSystem->GetKeyboard().IsKeyPressed('D'))
			{
				translation = { -speed, 0.0f, 0.0f };
			}
			else if (inputSystem->GetKeyboard().IsKeyPressed('A'))
			{
				translation = { speed, 0.0f, 0.0f };
			}

			DirectX::XMStoreFloat3(&translation, DirectX::XMVector3Transform(DirectX::XMLoadFloat3(&translation), DirectX::XMMatrixRotationQuaternion(camera.GetRotation())));

			DirectX::XMFLOAT3 position = camera.GetPosition();

			position.x += translation.x;
			position.y += translation.y;
			position.z += translation.z;

			camera.SetPosition(position);
		}

		transformBuffer.mProjectionMatrix = DirectX::XMMatrixTranspose(camera.GetProjectionMatrix());
		transformBuffer.mViewMatrix = DirectX::XMMatrixTranspose(camera.GetViewMatrix());
		transformBuffer.mWorldMatrix = DirectX::XMMatrixTranspose(DirectX::XMMatrixRotationY(elapsedTime) * DirectX::XMMatrixRotationX(elapsedTime));

		constantBuffer->Update(renderSystem->GetDevice(), transformBuffer);
	});

	engine.GetEventManager()->Subscribe<DrawEvent>("draw_event", [&window, &renderable, &indexBuffer](const DrawEvent*)
	{
		RenderSystem* renderSystem = Engine::GetInstance()->GetSystemManager()->GetSystem<RenderSystem>();

		D3D11_VIEWPORT viewport;
		ZeroMemory(&viewport, sizeof(viewport));
		viewport.Width = static_cast<float>(window.GetWidth());
		viewport.Height = static_cast<float>(window.GetHeight());
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;

		renderSystem->BeginFrame(window.GetHandle(), viewport, { 0.1f, 0.1f, 0.1f, 1.0f });

		for (auto& bindable : renderable)
		{
			bindable->Bind(renderSystem->GetDevice());
		}

		renderSystem->DrawIndexed(indexBuffer->GetIndexCount());
		renderSystem->EndFrame(true);
	});

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