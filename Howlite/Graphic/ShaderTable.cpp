#include "HowlitePCH.h"
#include "ShaderTable.h"
#include "Common/String.h"

namespace Howlite {

	HShaderTable& HShaderTable::GetInstance()
	{
		static HShaderTable table;
		return table;
	}

	const std::wstring HShaderTable::GetShaderPath(EShaderType Type) const
	{
		auto it = mTable.find(Type);
		if(it != mTable.end())
		{
			return it->second;
		}
		return {};
	}

	const std::wstring HShaderTable::operator[](EShaderType Type) const
	{
		return GetShaderPath(Type);
	}

	HShaderTable::HShaderTable()
	{
		mTable.insert({ EShaderType::Phong_VS, HString::ToWideString("PhongVS.cso") });
		mTable.insert({ EShaderType::Phong_PS, HString::ToWideString("PhongPS.cso") });
		mTable.insert({ EShaderType::Solid_VS, HString::ToWideString("SolidVS.cso") });
		mTable.insert({ EShaderType::Solid_PS, HString::ToWideString("SolidPS.cso") });
	}

	HShaderTable::~HShaderTable()
	{

	}

}