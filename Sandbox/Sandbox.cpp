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
#include <Engine/Render/SwapChain.h>
#include <Engine/Scene/Component/CameraComponent.h>
#include <Engine/Scene/Component/TransformComponent.h>

#include <utility>
#include <algorithm>

using namespace Howlite;

struct DrawEvent : public TEvent<DrawEvent> {};

struct TickEvent : public TEvent<TickEvent> 
{
	TickEvent(const float InDeltaTime) : mDeltaTime{ InDeltaTime }
	{

	}

	float mDeltaTime = 0.0f;
};

class Camera : public TEntity<Camera> {
public:
	HL_CLASS_DEFAULT(Camera);

	void SetupCamera(const float InWidth, const float InHeight)
	{
		mCameraComponent = Engine::GetInstance()->GetComponentManager()->AddComponent<CameraComponent>(GetEntityId());
		mTransformComponent = Engine::GetInstance()->GetComponentManager()->AddComponent<TransformComponent>(GetEntityId());
		HL_ASSERT(mCameraComponent, "Failed to add camera component");
		HL_ASSERT(mTransformComponent, "Failed to add transform component");
		mCameraComponent->SetPerspective(45.0f, InWidth / InHeight, 0.1f, 100.0f);
		mTransformComponent->TranslateZ(3.0f);
		mPrevMousePosition = Engine::GetInstance()->GetSystemManager()->GetSystem<InputSystem>()->GetMouse().GetPosition();
	}

	void OnMouseRawInput(const MouseRawInputEvent* InEvent)
	{
		InputSystem* inputSystem = Engine::GetInstance()->GetSystemManager()->GetSystem<InputSystem>();

		if (inputSystem->GetKeyboard().IsKeyPressed(VK_LMENU)) // left alt
		{
			const auto& mousePosition = inputSystem->GetMouse().GetPosition();
			
			const Mouse::Position& offset = { mousePosition.first - mPrevMousePosition.first, mousePosition.second - mPrevMousePosition.second };

			if (inputSystem->GetMouse().IsLeftButtonPressed())
			{
				mPitch += InEvent->mDeltaY * mSensitive;
				mYaw += InEvent->mDeltaX * mSensitive;
				mPitch = std::clamp(mPitch, 0.99f * -DirectX::XM_PIDIV2, 0.99f * DirectX::XM_PIDIV2);
				mYaw = std::clamp(mYaw, 0.99f * -DirectX::XM_PIDIV2, 0.99f * DirectX::XM_PIDIV2);
				const auto& rotation = DirectX::XMQuaternionRotationRollPitchYaw(-mPitch, mYaw, 0.0f);
				mTransformComponent->SetRotation(rotation);
			}
			else if (inputSystem->GetMouse().IsRightButtonPressed())
			{
				float radius = mTransformComponent->GetTranslation().z;
				radius += offset.second * mSensitive;
				if (radius <= 1.5f)
				{
					radius = 1.5f;
				}
				mTransformComponent->SetTranslation({ 0.0f, 0.0f, radius });
			}
			mPrevMousePosition = mousePosition;
		}
	}

	const DirectX::XMMATRIX& GetProjectionMatrix() const
	{
		return mCameraComponent->GetProjectionMatrix();
	}

	[[nodiscard]] const DirectX::XMMATRIX GetViewMatrix() const
	{
		const auto& translation = mTransformComponent->GetTranslation();
		const auto& rotation = mTransformComponent->GetRotation();
		const auto& position = DirectX::XMVector3Transform(DirectX::XMLoadFloat3(&translation), DirectX::XMMatrixRotationQuaternion(rotation));
		return DirectX::XMMatrixLookAtLH(position, DirectX::XMVectorZero(), DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
	}

private:
	CameraComponent* mCameraComponent = nullptr;
	TransformComponent* mTransformComponent = nullptr;
	Mouse::Position mPrevMousePosition = { 0, 0 };
	float mSensitive = 0.015f;
	float mPitch = 0.0f;
	float mYaw = 0.0f;
};

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	HL_UNUSED(hInstance);
	HL_UNUSED(hPrevInstance);
	HL_UNUSED(lpCmdLine);
	HL_UNUSED(nShowCmd);

	Engine engine;
	Window window;
	Camera* camera;
	
	bool isRunning = true;
	
	engine.Init();
	window.Init(1280, 720, "Howlite - Sandbox", Window::Style::Close | Window::Style::Title);

	const float width = static_cast<float>(window.GetWidth());
	const float height = static_cast<float>(window.GetHeight());
	
	camera = engine.GetEntityManager()->CreateEntity<Camera>();
	camera->SetupCamera(width, height);

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

	auto vertexShaderByteCode = renderSystem->ReadShader(L"..\\Shaders\\test_VS.cso");
	auto pixelShaderByteCode = renderSystem->ReadShader(L"..\\Shaders\\test_PS.cso");
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
	transformBuffer.mViewMatrix = DirectX::XMMatrixTranspose(camera->GetViewMatrix());
	transformBuffer.mProjectionMatrix = DirectX::XMMatrixTranspose(camera->GetProjectionMatrix());

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

	engine.GetEventManager()->Subscribe<MouseRawInputEvent>("mouse_raw_input_event", [&window, &camera](const MouseRawInputEvent* InEvent)
	{
		camera->OnMouseRawInput(InEvent);
	});

	engine.GetEventManager()->Subscribe<WindowCloseEvent>("window_close_event", [&isRunning](const WindowCloseEvent*)
	{
		isRunning = false;
	});

	engine.GetEventManager()->Subscribe<TickEvent>("tick_event", [&constantBuffer, &transformBuffer, &isRunning, &window, &camera](const TickEvent* InEvent)
	{
		InputSystem* inputSystem = Engine::GetInstance()->GetSystemManager()->GetSystem<InputSystem>();
		RenderSystem* renderSystem = Engine::GetInstance()->GetSystemManager()->GetSystem<RenderSystem>();

		if (inputSystem->GetKeyboard().IsKeyPressed(VK_ESCAPE))
		{
			isRunning = false;
		}

		transformBuffer.mWorldMatrix = DirectX::XMMatrixTranspose(DirectX::XMMatrixIdentity());
		transformBuffer.mViewMatrix = DirectX::XMMatrixTranspose(camera->GetViewMatrix());
		transformBuffer.mProjectionMatrix = DirectX::XMMatrixTranspose(camera->GetProjectionMatrix());
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

		engine.GetEventManager()->Post<TickEvent>(deltaTime);
		engine.GetEventManager()->Post<DrawEvent>();
		engine.GetEventManager()->Notify();
	}

	window.Terminate();
	engine.Terminate();

	return 0;
}