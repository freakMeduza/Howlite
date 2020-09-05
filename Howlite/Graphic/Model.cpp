#include "HowlitePCH.h"
#include "Model.h"
#include "Engine/Engine.h"
#include "Engine/Window.h"
#include "UI/UISystem.h"
#include "Common/Buffer.h"
#include "Common/Image.h"
#include "BindableCommon.h"
#include "ShaderTable.h"

namespace Howlite {

	HModel::HModel(HGraphicSystem& GraphicSystem, const std::string& File)
	{
		mMaterialData.Color = HColor::Aquamarine.GetColorFloat3();
		mMaterialData.SpecularIntensity = 30.0f;
		mMaterialData.SpecularPower = 0.2f;
		mMaterialData.DiffuseMapEnabled = FALSE;
		mMaterialData.NormalMapEnabled = FALSE;
		mBuffer = CreateSharedPointer<HPixelConstantBuffer<HMaterialData>>(GraphicSystem, mMaterialData, EConstantBufferSlot::Material);

		Assimp::Importer importer;
		
		const aiScene* scene = importer.ReadFile(File, 
												 aiProcess_Triangulate |
												 aiProcess_JoinIdenticalVertices |
												 aiProcess_ConvertToLeftHanded |
												 aiProcess_GenNormals);

		H_ASSERT(scene != nullptr, "Failed to load model 'aiScene* scene == nullptr'.");
		
		for(auto i = 0u; i < scene->mNumMeshes; ++i)
		{
			mMeshes.push_back(ParseMesh(GraphicSystem, *scene->mMeshes[i], scene->mMaterials));
		}
	
		mRootNode = ParseNode(*scene->mRootNode);
	}

	HModel::~HModel()
	{

	}

	void HModel::Draw(HGraphicSystem& GraphicSystem) const
	{
		mRootNode->Draw(GraphicSystem, DirectX::XMMatrixIdentity());
	}

	void HModel::Bind(HGraphicSystem& GraphicSystem) const
	{
		mBuffer->Bind(GraphicSystem);
		mBuffer->Update(GraphicSystem, mMaterialData);
	}

	void HModel::DrawUIWindow(bool* IsOpen) noexcept
	{
		const ImGuiWindowFlags flags = ImGuiWindowFlags_AlwaysAutoResize;

		static bool diffuseEnabled = mMaterialData.DiffuseMapEnabled == TRUE ? true : false;
		static bool normalEnabled = mMaterialData.NormalMapEnabled == TRUE ? true : false;
		
		if (!ImGui::Begin("Material", IsOpen, flags))
		{
			ImGui::End();
			return;
		}

		ImGui::Text("Appearance");
		ImGui::ColorEdit3("Material Color", &mMaterialData.Color.x);
		ImGui::SliderFloat("Material Specular Intencity", &mMaterialData.SpecularIntensity, 0.1f, 200.0f, "%.2f", 2.0f);
		ImGui::SliderFloat("Material Specular Power", &mMaterialData.SpecularPower, 0.1f, 200.0f, "%.2f", 2.0f);
		ImGui::Checkbox("Diffuse Mapping Enabled", &diffuseEnabled);
		ImGui::Checkbox("Normal Mapping Enabled", &normalEnabled);
		ImGui::Separator();
		ImGui::End();

		mMaterialData.DiffuseMapEnabled = diffuseEnabled ? TRUE : FALSE;
		mMaterialData.NormalMapEnabled = normalEnabled ? TRUE : FALSE;
	}

	SharedPointer<HMesh> HModel::ParseMesh(HGraphicSystem& GraphicSystem, const aiMesh& Mesh, const aiMaterial* const* Materials)
	{
		HBuffer buffer{
			{
				EAttributeType::Position3D,
				EAttributeType::Normal3D,
				EAttributeType::UV2D
			}
		};
		
		for(auto i = 0u; i < Mesh.mNumVertices; ++i)
		{
			const DirectX::XMFLOAT3 position = *reinterpret_cast<DirectX::XMFLOAT3*>(&Mesh.mVertices[i]);
			const DirectX::XMFLOAT3 normal = *reinterpret_cast<DirectX::XMFLOAT3*>(&Mesh.mNormals[i]);
			const DirectX::XMFLOAT2 uv = *reinterpret_cast<DirectX::XMFLOAT2*>(&Mesh.mTextureCoords[0][i]);
			buffer.EmplaceBack(position, normal, uv);
		}

		const size_t indexCount = static_cast<size_t>(Mesh.mNumFaces) * 3;

		std::vector<uint32_t> indices;

		indices.reserve(indexCount);

		for(auto i = 0u; i < Mesh.mNumFaces; ++i)
		{
			const aiFace& face = Mesh.mFaces[i];
			H_ASSERT(face.mNumIndices == 3u, "Failed to load model. Indices count mismatch 'face->mNumIndices != 3u'.")
			if(face.mNumIndices == 3)
			{
				indices.push_back(face.mIndices[0]);
				indices.push_back(face.mIndices[1]);
				indices.push_back(face.mIndices[2]);
			}
		}

		std::vector<SharedPointer<HBindable>> bindables;

		bool diffuseMapEnabled = false;
		bool normalMapEnabled = false;

		if(Mesh.mMaterialIndex >= 0)
		{
			const aiMaterial& material = *Materials[Mesh.mMaterialIndex];

			aiString filename;

			const std::string modelPath = "Assets\\monster\\";

			if (material.GetTexture(aiTextureType_DIFFUSE, 0, &filename) == AI_SUCCESS)
			{
				std::string path = modelPath;
				path.append(filename.C_Str());
				bindables.push_back(CreateSharedPointer<HTexture>(GraphicSystem, HImage::LoadFromFile(path), ETextureSlot::Diffuse));
				diffuseMapEnabled = true;
			}
			if (material.GetTexture(aiTextureType_NORMALS, 0, &filename) == AI_SUCCESS)
			{
				std::string path = modelPath;
				path.append(filename.C_Str());
				bindables.push_back(CreateSharedPointer<HTexture>(GraphicSystem, HImage::LoadFromFile(path), ETextureSlot::Normal));
				normalMapEnabled = true;
			}

			bindables.push_back(CreateSharedPointer<HSampler>(GraphicSystem));
		}

		bindables.push_back(CreateSharedPointer<HVertexBuffer>(GraphicSystem, buffer));
		bindables.push_back(CreateSharedPointer<HIndexBuffer>(GraphicSystem, indices));

		SharedPointer<HVertexShader> vertexShader = CreateSharedPointer<HVertexShader>(GraphicSystem, HShaderTable::GetInstance()[EShaderType::Phong_VS]);
		
		bindables.push_back(vertexShader);
		bindables.push_back(CreateSharedPointer<HPixelShader>(GraphicSystem, HShaderTable::GetInstance()[EShaderType::Phong_PS]));
		bindables.push_back(CreateSharedPointer<HInputLayout>(GraphicSystem, buffer.GetLayout(), vertexShader->GetShaderBytecode()));
		bindables.push_back(CreateSharedPointer<HTopology>(GraphicSystem, D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
		
		mMaterialData.DiffuseMapEnabled = diffuseMapEnabled ? TRUE : FALSE;
		mMaterialData.NormalMapEnabled = normalMapEnabled ? TRUE : FALSE;

		bindables.emplace_back(mBuffer);

		return CreateSharedPointer<HMesh>(GraphicSystem, bindables);
	}

	SharedPointer<HNode> HModel::ParseNode(const aiNode& Node)
	{
		const size_t meshCount = static_cast<size_t>(Node.mNumMeshes);

		std::vector<SharedPointer<HMesh>> meshes;

		meshes.reserve(meshCount);

		for(auto i = 0u; i < meshCount; ++i)
		{
			const uint32_t index = Node.mMeshes[i];
			meshes.push_back(mMeshes[index]);
		}

		const DirectX::XMMATRIX transform = DirectX::XMMatrixTranspose(DirectX::XMLoadFloat4x4(reinterpret_cast<const DirectX::XMFLOAT4X4*>(&Node.mTransformation)));

		SharedPointer<HNode> node = CreateSharedPointer<HNode>(Node.mName.C_Str(), meshes, transform);

		for(auto i = 0u; i < Node.mNumChildren; ++i)
		{
			node->AddChild(ParseNode(*Node.mChildren[i]));
		}

		return node;
	}

}