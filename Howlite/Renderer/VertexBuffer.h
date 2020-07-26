#pragma once

#include "Bindable.h"
#include "Common/Buffer.h"

namespace Howlite {

	class HVertexBuffer : public HBindable {
	public:
		HVertexBuffer(HGraphicSystem& GraphicSystem, const HBuffer& Buffer);
		~HVertexBuffer() override;

		HVertexBuffer(HVertexBuffer&&) = delete;
		HVertexBuffer(const HVertexBuffer&) = delete;

		HVertexBuffer& operator=(HVertexBuffer&&) = delete;
		HVertexBuffer& operator=(const HVertexBuffer&) = delete;

		// Bindable interface
		void Bind(HGraphicSystem& GraphicSystem) const noexcept;

	private:
		UINT mStride{ 0u };
		Microsoft::WRL::ComPtr<ID3D11Buffer> mBuffer;
		
	};

}