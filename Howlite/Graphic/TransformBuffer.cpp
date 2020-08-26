#include "HowlitePCH.h"
#include "TransformBuffer.h"
#include "Engine/Engine.h"
#include "Engine/Window.h"
#include "Graphic/GraphicSystem.h"
#include "Graphic/Camera.h"

namespace Howlite {

	ScopedPointer<HVertexConstantBuffer<HTransformBuffer::TransformBuffer>> HTransformBuffer::mBufferVS = nullptr;
	ScopedPointer<HPixelConstantBuffer<HTransformBuffer::TransformBuffer>> HTransformBuffer::mBufferPS = nullptr;

	HTransformBuffer::HTransformBuffer(HGraphicSystem& GraphicSystem, const HDrawable& Parent, EConstantBufferSlot Slot) :
		mParent{ Parent }
	{
		if(!mBufferVS)
		{
			mBufferVS = CreateScopedPointer<HVertexConstantBuffer<TransformBuffer>>(GraphicSystem, Slot);
		}
		if (!mBufferPS)
		{
			mBufferPS = CreateScopedPointer<HPixelConstantBuffer<TransformBuffer>>(GraphicSystem, Slot);
		}
	}

	HTransformBuffer::~HTransformBuffer()
	{

	}

	void HTransformBuffer::Bind(HGraphicSystem& GraphicSystem) const noexcept
	{
		using namespace DirectX;
		const HWindow& window = HEngine::GetInstance().GetWindowInstance();
		const HCamera& camera = HEngine::GetInstance().GetCameraInstance();
		const float width = static_cast<float>(window.GetWidth());
		const float height = static_cast<float>(window.GetHeight());
		const XMMATRIX& modelMatrix = mParent.GetTransform();
		const XMMATRIX& viewMatrix = camera.GetViewMatrix();
		const XMMATRIX& projectionMatrix = camera.GetProjectionMatrix();
		const XMFLOAT3& position = camera.GetPosition();
		mBufferPS->Update(GraphicSystem, TransformBuffer{ XMMatrixTranspose(modelMatrix), XMMatrixTranspose(viewMatrix), XMMatrixTranspose(projectionMatrix), position });
		mBufferPS->Bind(GraphicSystem);
		mBufferVS->Update(GraphicSystem, TransformBuffer{ XMMatrixTranspose(modelMatrix), XMMatrixTranspose(viewMatrix), XMMatrixTranspose(projectionMatrix), position });
		mBufferVS->Bind(GraphicSystem);
	}

}