#include "HowlitePCH.h"
#include "TransformBuffer.h"
#include "Engine/Engine.h"
#include "Engine/Window.h"
#include "Renderer/GraphicSystem.h"
#include "Renderer/Camera.h"

namespace Howlite {

	ScopedPointer<HVertexConstantBuffer<HTransformBuffer::TransformBuffer>> HTransformBuffer::mBuffer = nullptr;

	HTransformBuffer::HTransformBuffer(HGraphicSystem& GraphicSystem, const HDrawable& Parent, UINT Slot) :
		mParent{ Parent }
	{
		if(!mBuffer)
		{
			mBuffer = CreateScopedPointer<HVertexConstantBuffer<TransformBuffer>>(GraphicSystem, Slot);
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
		const XMMATRIX modelMatrix = mParent.GetTransform();
		const XMMATRIX viewMatrix = camera.GetViewMatrix();
		const XMMATRIX projectionMatrix = camera.GetProjectionMatrix();
		mBuffer->Update(GraphicSystem, TransformBuffer{ XMMatrixTranspose(modelMatrix), XMMatrixTranspose(viewMatrix), XMMatrixTranspose(projectionMatrix) });
		mBuffer->Bind(GraphicSystem);
	}

}