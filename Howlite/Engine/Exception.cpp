#include "HowlitePCH.h"
#include "Exception.h"

namespace {
	static std::string GetDefaultErrorMessage(const std::string& File, int Line)
	{
		std::stringstream ss;

		ss << std::endl
			<< File
			<< ":"
			<< Line
			<< std::endl;

		return ss.str();
	}

	static std::string GetPlatformErrorMessage(HRESULT ErrorCode) 
	{
		char* buffer = nullptr;

		const DWORD length = FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			nullptr,
			ErrorCode,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			reinterpret_cast<LPSTR>(&buffer),
			0,
			nullptr
		);

		if (length == 0)
		{
			return "Unknown error code.";
		}

		std::string errorMessage = buffer;
		
		LocalFree(buffer);
		
		return errorMessage;
	}

	static std::string GetGraphicErrorMessage(HRESULT ErrorCode)
	{
		std::stringstream ss;
	#ifdef _DEBUG
		ss << std::endl
			<< DXGetErrorDescription(ErrorCode)
			<< std::endl
			<< DXGetErrorString(ErrorCode);
	#else
		H_UNUSED(ErrorCode)
	#endif
		return ss.str();
	}
}

namespace Howlite {

	HException::HException(const std::string& File, int Line) :
		mFile{ File }, mLine{ Line }
	{
		mErrorDescription = GetDefaultErrorMessage(mFile, mLine);
	}

	HException::HException(const std::string& File, int Line, const std::string& ErrorString) :
		mFile{ File }, mLine{ Line }
	{
		mErrorDescription = GetDefaultErrorMessage(mFile, mLine);

		std::stringstream ss;

		ss << std::endl
			<< ErrorString
			<< std::endl;

		mErrorDescription += ss.str();
	}

	HException::HException(const std::string& File, int Line, HRESULT ErrorCode, EExceptionType ExceptionType, const std::string& ErrorString) :
		mFile{ File }, mLine{ Line }
	{
		mErrorDescription = GetDefaultErrorMessage(mFile, mLine);

		switch (ExceptionType)
		{
			case EExceptionType::Platform:
			{
				mErrorDescription += GetPlatformErrorMessage(ErrorCode);
				break;
			}
			case EExceptionType::Graphic:
			{
				mErrorDescription += GetGraphicErrorMessage(ErrorCode);
				break;
			}
		}

		std::stringstream ss;

		ss << std::endl
			<< ErrorString
			<< std::endl;

		mErrorDescription += ss.str();
	}

	const char* HException::what() const noexcept
	{
		return mErrorDescription.data();
	}

	std::string HException::GetFile() const noexcept
	{
		return mFile;
	}

	int HException::GetLine() const noexcept
	{
		return mLine;
	}

	HRESULT HException::GetErrorCode() const noexcept
	{
		return mErrorCode;
	}

}