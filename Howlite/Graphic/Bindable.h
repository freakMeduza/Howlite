#pragma once

#include "GraphicSystem.h"

namespace Howlite {

	class HBindable {
	public:
		virtual ~HBindable() = default;

		/**
		 * Bind Object
		 * @param GraphicSystem 
		 * @return 
		 */
		virtual void Bind(HGraphicSystem& GraphicSystem) const noexcept = 0;

	protected:
		static ID3D11Device& GetDevice(HGraphicSystem& GraphicSystem) noexcept;
		static ID3D11DeviceContext& GetContext(HGraphicSystem& GraphicSystem) noexcept;
		static HDXGIInfoQueue& GetDXGIInfoQueue(HGraphicSystem& GraphicSystem);

	};

}