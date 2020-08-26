#include "HowlitePCH.h"
#include "Mesh.h"
#include "BindableCommon.h"

namespace Howlite {

	HMesh::HMesh(HGraphicSystem& GraphicSystem, const std::vector<SharedPointer<HBindable>>& Bindables)
	{
		AddBind(CreateSharedPointer<HTopology>(GraphicSystem, D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
		AddBind(CreateSharedPointer<HTransformBuffer>(GraphicSystem, *this, EConstantBufferSlot::Transform));

		for(const SharedPointer<HBindable>& bind : Bindables)
		{
			AddBind(bind);
		}

		mTransform = DirectX::XMMatrixIdentity();
	}

	HMesh::~HMesh()
	{

	}

	DirectX::XMMATRIX HMesh::GetTransform() const noexcept
	{
		return mTransform;
	}

	void HMesh::Draw(HGraphicSystem& GraphicSystem, const DirectX::XMMATRIX& ParentTransform) const
	{
		mTransform = ParentTransform;
		HDrawable::Draw(GraphicSystem);
	}

}