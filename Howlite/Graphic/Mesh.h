#pragma once

#include "Drawable.h"

namespace Howlite {

	class HMesh : public HDrawable {
	public:
		HMesh(HGraphicSystem& GraphicSystem, const std::vector<SharedPointer<HBindable>>& Bindables);
		~HMesh();

		HMesh(HMesh&&) = delete;
		HMesh(const HMesh&) = delete;

		HMesh& operator=(HMesh&&) = delete;
		HMesh& operator=(const HMesh&) = delete;

		// Drawable interface
		DirectX::XMMATRIX GetTransform() const noexcept override;

		/**
		 * Draw Mesh
		 * @param GraphicSystem 
		 * @param ParentTransform 
		 */
		void Draw(HGraphicSystem& GraphicSystem, const DirectX::XMMATRIX& ParentTransform) const;

	private:
		mutable DirectX::XMMATRIX mTransform;

	};

}
