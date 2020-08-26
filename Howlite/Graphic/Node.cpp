#include "HowlitePCH.h"
#include "Node.h"

namespace Howlite {

	HNode::HNode(const std::string& Name, const std::vector<SharedPointer<HMesh>>& Meshes, const DirectX::XMMATRIX& Transform) :
		mName{ Name }, mTransform{ Transform }, mMeshes{ Meshes }
	{

	}

	HNode::~HNode()
	{

	}

	void HNode::AddChild(SharedPointer<HNode> Child)
	{
		H_ASSERT(Child != nullptr, "Failed to add child node 'Child != nullptr'.")
		mChildren.push_back(Child);
	}

	void HNode::RemoveChild(SharedPointer<HNode> Child)
	{
		H_ASSERT(Child != nullptr, "Failed to remove child node 'Child != nullptr'.")
		auto it = std::find(mChildren.begin(), mChildren.end(), Child);
		if(it != mChildren.end())
		{
			mChildren.erase(it);
		}
	}

	std::vector<SharedPointer<HNode>> HNode::GetChildren() const noexcept
	{
		return mChildren;
	}

	std::string HNode::GetName() const noexcept
	{
		return mName;
	}

	void HNode::Draw(HGraphicSystem& GraphicSystem, const DirectX::XMMATRIX ParentTransform) const
	{
		const DirectX::XMMATRIX transform = mTransform * ParentTransform;
		for (const auto& mesh : mMeshes)
		{
			mesh->Draw(GraphicSystem, transform);
		}
		for(const auto& child : mChildren)
		{
			child->Draw(GraphicSystem, transform);
		}
	}

}
