#pragma once

#include "Bindable.h"

namespace Howlite {

	class HIndexBuffer : public HBindable {
	public:
		HIndexBuffer(HGraphicSystem& GraphicSystem, const std::vector<uint32_t>& Indices);
		~HIndexBuffer() override;

		HIndexBuffer(HIndexBuffer&&) = delete;
		HIndexBuffer(const HIndexBuffer&) = delete;

		HIndexBuffer& operator=(HIndexBuffer&&) = delete;
		HIndexBuffer& operator=(const HIndexBuffer&) = delete;

		// Bindable interface
		void Bind(HGraphicSystem& GraphicSystem) const noexcept;

		/**
		 * Get Indices Count
		 * @return 
		 */
		UINT GetIndicesCount() const noexcept;

	private:
		UINT mCount{ 0u };
		Microsoft::WRL::ComPtr<ID3D11Buffer> mBuffer;

	};

}
