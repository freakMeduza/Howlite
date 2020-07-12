#include "HowlitePCH.h"
#include "Bindable.h"

namespace Howlite {

	ID3D11Device& HBindable::GetDevice(HGraphicSystem& GraphicSystem) noexcept
	{
		return *GraphicSystem.mDevice.Get();
	}

	ID3D11DeviceContext& HBindable::GetContext(HGraphicSystem& GraphicSystem) noexcept
	{
		return *GraphicSystem.mContext.Get();
	}

	HDXGIInfoQueue& HBindable::GetDXGIInfoQueue(HGraphicSystem& GraphicSystem)
	{
	#if _DEBUG
		H_ASSERT(GraphicSystem.mDXGIInfoQueue != nullptr, "Failed to get context 'GraphicSystem.mDXGIInfoQueue == nullptr'.")
		return *GraphicSystem.mDXGIInfoQueue;
	#else
		H_UNUSED(GraphicSystem)
		H_ASSERT(false, "Unable to get dxgi info queue in release configuration.")
	#endif
	}

}