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
		
		/**
		 * Get Obect ID
		 * @return 
		 */
		inline virtual HID GetObjectID() const 
		{
			H_ASSERT(false, "Failed to get object ID.");
			return CreateObjectID("");
		}

	protected:
		static ID3D11Device& GetDevice(HGraphicSystem& GraphicSystem) noexcept;
		static ID3D11DeviceContext& GetContext(HGraphicSystem& GraphicSystem) noexcept;
		static HDXGIInfoQueue& GetDXGIInfoQueue(HGraphicSystem& GraphicSystem);

	};

}