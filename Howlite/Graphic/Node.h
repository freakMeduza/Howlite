#pragma once

#include "Mesh.h"

namespace Howlite {

	class HNode {
	public:
		HNode(const std::string& Name, const std::vector<SharedPointer<HMesh>>& Meshes, const DirectX::XMMATRIX& Transform);
		~HNode();

		HNode(HNode&&) = delete;
		HNode(const HNode&) = delete;

		HNode& operator=(HNode&&) = delete;
		HNode& operator=(const HNode&) = delete;

		/**
		 * Add Child Node
		 * @param Child 
		 */
		void AddChild(SharedPointer<HNode> Child);

		/**
		 * Remove Child Node
		 * @param Child 
		 */
		void RemoveChild(SharedPointer<HNode> Child);

		/**
		 * Get Children Nodes
		 * @return 
		 */
		std::vector<SharedPointer<HNode>> GetChildren() const noexcept;

		/**
		 * Get Node Name
		 * @return 
		 */
		std::string GetName() const noexcept;

		/**
		 * Draw Node
		 * @param GraphicSystem 
		 * @param ParentTransform 
		 */
		void Draw(class HGraphicSystem& GraphicSystem, const DirectX::XMMATRIX ParentTransform) const;

	private:
		std::string mName;
		
		DirectX::XMMATRIX mTransform;
		
		std::vector<SharedPointer<HNode>> mChildren;
		std::vector<SharedPointer<HMesh>> mMeshes;

	};

}