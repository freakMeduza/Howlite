#include "HowlitePCH.h"
#include "TransformBuffer.h"

#include "Engine/Engine.h"
#include "Engine/Window.h"

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
		const float width = static_cast<float>(window.GetWidth());
		const float height = static_cast<float>(window.GetHeight());
		const XMMATRIX modelMatrix = mParent.GetTransform();
		const XMMATRIX viewMatrix = XMMatrixLookAtLH(XMVectorSet(0.0f, 0.0f, -5.0f, 0.0f),
													 XMVectorZero(),
													 XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
		const XMMATRIX projectionMatrix = XMMatrixPerspectiveLH(1.0f, height / width, 0.5f, 100.0f);

		const TransformBuffer buffer{
			XMMatrixTranspose(modelMatrix),
			XMMatrixTranspose(viewMatrix),
			XMMatrixTranspose(projectionMatrix),
		};

		mBuffer->Update(GraphicSystem, buffer);
		mBuffer->Bind(GraphicSystem);
	}

}