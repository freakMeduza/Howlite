#pragma once

#include "Bindable.h"
#include "Drawable.h"
#include "ConstantBuffer.h"

namespace Howlite {

	class HTransformBuffer : public HBindable {
	public:
		HTransformBuffer(HGraphicSystem& GraphicSystem, const HDrawable& Parent, UINT Slot = 0);
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
		};

		static ScopedPointer<typename HVertexConstantBuffer<TransformBuffer>> mBuffer;

	};

}