#pragma once

namespace Howlite {

	enum class EShaderType
	{
		Phong_VS,
		Phong_PS,
		Solid_VS,
		Solid_PS
	};

	class HShaderTable {
	public:
		/**
		 * Get Instance
		 * @return 
		 */
		static HShaderTable& GetInstance();

		/**
		 * Get Shader Path From Table
		 * @param Type 
		 * @return 
		 */
		const std::wstring GetShaderPath(EShaderType Type) const;

		/**
		 * Overloaded operator[] Get Shader Path From Table
		 * @param Type
		 * @return
		 */
		const std::wstring operator[](EShaderType Type) const;

	private:
		HShaderTable();
		~HShaderTable();

		std::unordered_map<EShaderType, std::wstring> mTable;

	};

	

}
