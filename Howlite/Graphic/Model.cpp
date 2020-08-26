#include "HowlitePCH.h"
#include "Model.h"
#include "Engine/Engine.h"
#include "Engine/Window.h"
#include "UI/UISystem.h"
#include "Common/Buffer.h"
#include "BindableCommon.h"
#include "Shader/ShaderTable.h"

namespace Howlite {

	HModel::HModel(HGraphicSystem& GraphicSystem, const std::string& File)
	{
		Assimp::Importer importer;
		
		const aiScene* scene = importer.ReadFile(File, 
												 aiProcess_Triangulate |
												 aiProcess_JoinIdenticalVertices |
												 aiProcess_ConvertToLeftHanded |
												 aiProcess_GenNormals);

		H_ASSERT(scene != nullptr, "Failed to load model 'aiScene* scene == nullptr'.");
		
		for(auto i = 0u; i < scene->mNumMeshes; ++i)
		{
			mMeshes.push_back(ParseMesh(GraphicSystem, *scene->mMeshes[i]));
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

	SharedPointer<HMesh> HModel::ParseMesh(HGraphicSystem& GraphicSystem, const aiMesh& Mesh)
	{
		HBuffer buffer{ {EAttributeType::Position3D, EAttributeType::Normal3D } };
		
		for(auto i = 0u; i < Mesh.mNumVertices; ++i)
		{
			const DirectX::XMFLOAT3 position = *reinterpret_cast<DirectX::XMFLOAT3*>(&Mesh.mVertices[i]);
			const DirectX::XMFLOAT3 normal = *reinterpret_cast<DirectX::XMFLOAT3*>(&Mesh.mNormals[i]);
			buffer.EmplaceBack(position, normal);
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

		bindables.push_back(CreateSharedPointer<HVertexBuffer>(GraphicSystem, buffer));
		bindables.push_back(CreateSharedPointer<HIndexBuffer>(GraphicSystem, indices));

		SharedPointer<HVertexShader> vertexShader = CreateSharedPointer<HVertexShader>(GraphicSystem, HShaderTable::GetInstance()[EShaderType::Phong_VS]);
		
		bindables.push_back(vertexShader);
		bindables.push_back(CreateSharedPointer<HPixelShader>(GraphicSystem, HShaderTable::GetInstance()[EShaderType::Phong_PS]));
		bindables.push_back(CreateSharedPointer<HInputLayout>(GraphicSystem, buffer.GetLayout(), vertexShader->GetShaderBytecode()));
		bindables.push_back(CreateSharedPointer<HTopology>(GraphicSystem, D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
		
		struct MaterialBuffer
		{
			DirectX::XMFLOAT3 Color;
			float SpecularIntensity;
			float SpecularPower;
			float padding[3];
		};
		
		MaterialBuffer material;
		material.Color             = HColor::DodgerBlue.GetColorFloat3();
		material.SpecularIntensity = 30.0f;
		material.SpecularPower     = 0.6f;

		bindables.emplace_back(CreateSharedPointer<HPixelConstantBuffer<MaterialBuffer>>(GraphicSystem, material, EConstantBufferSlot::Material));

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