#pragma once

#include "Bindable.h"
#include "Drawable.h"
#include "ConstantBuffer.h"

namespace Howlite {

	class HTransformBuffer : public HBindable {
	public:
		HTransformBuffer(HGraphicSystem& GraphicSystem, const HDrawable& Parent, EConstantBufferSlot Slot);
		~HTransformBuffer() override;

		HTransformBuffer(HTransformBuffer&&) = delete;
		HTransformBuffer(const HTransformBuffer&) = delete;

		HTransformBuffer& operator=(HTransformBuffer&&) = delete;
		HTransformBuffer& operator=(const HTransformBuffer&) = delete;

		// Bindable interface
		void Bind(HGraphicSystem& GraphicSystem) const noexcept;

	private:
		const HDrawable& mParent;

		struct TransformBuffer
		{
			DirectX::XMMATRIX ModelMatrix;
			DirectX::XMMATRIX ViewMatrix;
			DirectX::XMMATRIX ProjectionMatrix;
			alignas(16) DirectX::XMFLOAT3 CameraPosition;
		};

		static ScopedPointer<typename HVertexConstantBuffer<TransformBuffer>> mBufferVS;
		static ScopedPointer<typename HPixelConstantBuffer<TransformBuffer>> mBufferPS;

	};

}