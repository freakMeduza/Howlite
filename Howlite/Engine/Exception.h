#pragma once

namespace Howlite {

	enum class EExceptionType 
	{
		Platform,
		Graphic
	};

	class HException : public std::exception {
	public:
		HException(const std::string& File, int Line);
		HException(const std::string& File, int Line, const std::string& ErrorString);
		HException(const std::string& File, int Line, HRESULT ErrorCode, EExceptionType ExceptionType, const std::string& ErrorString = {});

		~HException() override = default;

		// std::exception interface
		virtual const char* what() const noexcept override;

		/**
		 * Get File
		 * @return 
		 */
		std::string GetFile() const noexcept;

		/**
		 * Get Line
		 * @return 
		 */
		int GetLine() const noexcept;

		/**
		 * Get Error Code
		 * @return 
		 */
		HRESULT GetErrorCode() const noexcept;

	private:
		int mLine{ -1 };

		HRESULT mErrorCode{ NOERROR };
		
		std::string mFile;
		std::string mErrorDescription;

	};

}

