#pragma once

#include "IBindable.h"

namespace Howlite {

	class IndexBuffer : public IBindable {
	public:
		HL_DISABLE_COPY(IndexBuffer);
		// Inherited via IBindable
		virtual void Bind(Device* InDevice) override;
		
		IndexBuffer(class Device* InDevice, const std::vector<uint32_t>& InIndexList);

		inline size_t GetIndexCount() const noexcept { return mIndexCount; }

	private:
		size_t mIndexCount = 0;
		Microsoft::WRL::ComPtr<ID3D11Buffer> mBuffer = nullptr;
	};

}