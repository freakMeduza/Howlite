#pragma once

#include "Node.h"

namespace Howlite {

	class HModel {
	public:
		HModel(HGraphicSystem& GraphicSystem, const std::string& File);
		~HModel();

		HModel(HModel&&) = delete;
		HModel(const HModel&) = delete;

		HModel& operator=(HModel&&) = delete;
		HModel& operator=(const HModel&) = delete;

		/**
		 * Draw Model
		 * @param GraphicSystem 
		 */
		void Draw(HGraphicSystem& GraphicSystem) const;

	private:
		SharedPointer<HMesh> ParseMesh(HGraphicSystem& GraphicSystem, const aiMesh& Mesh);
		SharedPointer<HNode> ParseNode(const aiNode& Node);

		std::vector<SharedPointer<HMesh>> mMeshes;
		
		SharedPointer<HNode> mRootNode{ nullptr };
	};


}
