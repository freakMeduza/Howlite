#pragma once

#include "Node.h"
#include "ConstantBuffer.h"

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

		/**
		 * Bind Model
		 * @param GraphicSystem 
		 */
		void Bind(HGraphicSystem& GraphicSystem) const;

		/**
		 * Draw UI Window
		 * @param IsOpen
		 * @return
		 */
		void DrawUIWindow(bool* IsOpen) noexcept;

	private:
		SharedPointer<HMesh> ParseMesh(HGraphicSystem& GraphicSystem, const aiMesh& Mesh, const aiMaterial* const* Materials);
		SharedPointer<HNode> ParseNode(const aiNode& Node);

		std::vector<SharedPointer<HMesh>> mMeshes;
		
		SharedPointer<HNode> mRootNode{ nullptr };

		HMaterialData mMaterialData;
		SharedPointer<HPixelConstantBuffer<HMaterialData>> mBuffer{ nullptr };
	};


}
