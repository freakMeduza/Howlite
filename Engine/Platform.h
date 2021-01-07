#pragma once

#ifdef _WIN64
#else
	#error platform doesn't supported'
#endif

#define WIN32_LEAN_AND_MEAN
#define NOGDICAPMASKS
#define NOMENUS
#define NOICONS
#define NORASTEROPS
#define OEMRESOURCE
#define NOATOM
#define NOCLIPBOARD
#define NOCOLOR
#define NOCTLMGR
#define NODRAWTEXT
#define NOKERNEL
#define NONLS
#define NOMEMMGR
#define NOMETAFILE
#define NOOPENFILE
#define NOSCROLL
#define NOSERVICE
#define NOSOUND
#define NOTEXTMETRIC
#define NOWH
#define NOCOMM
#define NOKANJI
#define NOHELP
#define NOPROFILER
#define NODEFERWINDOWPOS
#define NOMCX
#define NORPC
#define NOPROXYSTUB
#define NOIMAGE
#define NOTAPE
#define NOMINMAX

// move to pch
#include <Windows.h>
#include <wrl.h>
#include <cstdio>
#include <cstdint>
#include <limits>
#include <typeinfo>
#include <variant>
#include <functional>
#include <memory>
#include <string>
#include <sstream>
#include <list>
#include <array>
#include <vector>
#include <unordered_map>
#include <ctime>
#include <bitset>

// Direct3D 
#include <d3d11.h>
#include <dxgidebug.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

namespace Howlite {

	static constexpr size_t HL_INVALID_ID = std::numeric_limits<size_t>::max();

	using EventTypeId = size_t;
	using EntityId = size_t;
	using EntityTypeId = size_t;
	using ComponentId = size_t;
	using ComponentTypeId = size_t;
	using SystemTypeId = size_t;

	enum class LogCategory : uint8_t {
		Debug,
		Info,
		Warn,
		Error
	};

	inline static void SetConsoleColor(const LogCategory InLogCategory) noexcept
	{
		HANDLE hConsole = ::GetStdHandle(STD_OUTPUT_HANDLE);
		
		WORD color = 0;
		switch (InLogCategory)
		{
			case LogCategory::Debug:
				color = 8;
				break;
			case LogCategory::Info:
				color = 10;
				break;
			case LogCategory::Warn:
				color = 14;
				break;
			case LogCategory::Error:
				color = 12;
				break;
		};

		::SetConsoleTextAttribute(hConsole, color);
	}

	inline static std::string FormatLogMessage(const char* Format) noexcept {
		std::stringstream stream;
		struct tm newtime;
		time_t now = time(NULL);
		localtime_s(&newtime, &now);
		char buffer[100];
		strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", &newtime);
		stream << buffer << " [Howlite]: " << Format;
		return stream.str();
	}
}

#ifdef _DEBUG
	#define HL_DEBUG(Format, ...) { Howlite::SetConsoleColor(Howlite::LogCategory::Debug); printf(Howlite::FormatLogMessage(Format).c_str(), __VA_ARGS__); }
#else
	#define HL_DEBUG(Format, ...) 
#endif

#define HL_INFO(Format, ...)  { Howlite::SetConsoleColor(Howlite::LogCategory::Info); printf(Howlite::FormatLogMessage(Format).c_str(), __VA_ARGS__); }
#define HL_WARN(Format, ...)  { Howlite::SetConsoleColor(Howlite::LogCategory::Warn); printf(Howlite::FormatLogMessage(Format).c_str(), __VA_ARGS__); }
#define HL_ERROR(Format, ...) { Howlite::SetConsoleColor(Howlite::LogCategory::Error); printf(Howlite::FormatLogMessage(Format).c_str(), __VA_ARGS__); }

#ifdef _DEBUG
	#define HL_ASSERT(Condition, Message) \
	{ \
		if (!(Condition)) { \
			::MessageBox(nullptr, Message, "Assertion Failed!", MB_OK | MB_ICONERROR); \
			__debugbreak(); \
		} \
	}
#else
	#define HL_ASSERT(Condition, Message)
#endif

#define HL_UNUSED(Parameter) { static_cast<void>((Parameter)); }

#define HL_CLASS_STATIC(ClassName) \
	ClassName() = delete; \
	~ClassName() = delete; \
	ClassName(ClassName&&) = delete; \
	ClassName& operator=(ClassName&&) = delete; \
	ClassName(const ClassName&) = delete; \
	ClassName& operator=(const ClassName&) = delete; \

#define HL_CLASS_DEFAULT(ClassName) \
	ClassName() = default; \
	~ClassName() = default; \
	ClassName(ClassName&&) = default; \
	ClassName& operator=(ClassName&&) = default; \
	ClassName(const ClassName&) = default; \
	ClassName& operator=(const ClassName&) = default; \

#define HL_CLASS_DISABLE_COPY(ClassName) \
	ClassName() = default; \
	virtual ~ClassName() = default; \
	ClassName(ClassName&&) = default; \
	ClassName& operator=(ClassName&&) = default; \
	ClassName(const ClassName&) = delete; \
	ClassName& operator=(const ClassName&) = delete; \

#define HL_CLASS_DISABLE_MOVE(ClassName) \
	ClassName() = default; \
	virtual ~ClassName() = default; \
	ClassName(ClassName&&) = delete; \
	ClassName& operator=(ClassName&&) = delete; \
	ClassName(const ClassName&) = default; \
	ClassName& operator=(const ClassName&) = default; \

#define HL_CLASS_DISABLE_COPY_AND_MOVE(ClassName) \
	ClassName() = default; \
	virtual ~ClassName() = default; \
	ClassName(ClassName&&) = delete; \
	ClassName& operator=(ClassName&&) = delete; \
	ClassName(const ClassName&) = default; \
	ClassName& operator=(const ClassName&) = default; \

#define HL_INTERFACE_STATIC(ClassName) \
	ClassName() = delete; \
	virtual ~ClassName() = delete; \
	ClassName(ClassName&&) = delete; \
	ClassName& operator=(ClassName&&) = delete; \
	ClassName(const ClassName&) = delete; \
	ClassName& operator=(const ClassName&) = delete; \

#define HL_INTERFACE_DEFAULT(ClassName) \
	ClassName() = default; \
	virtual ~ClassName() = default; \
	ClassName(ClassName&&) = default; \
	ClassName& operator=(ClassName&&) = default; \
	ClassName(const ClassName&) = default; \
	ClassName& operator=(const ClassName&) = default; \

#define HL_INTERFACE_DISABLE_COPY(ClassName) \
	ClassName() = default; \
	virtual ~ClassName() = default; \
	ClassName(ClassName&&) = default; \
	ClassName& operator=(ClassName&&) = default; \
	ClassName(const ClassName&) = delete; \
	ClassName& operator=(const ClassName&) = delete; \

#define HL_INTERFACE_DISABLE_MOVE(ClassName) \
	ClassName() = default; \
	virtual ~ClassName() = default; \
	ClassName(ClassName&&) = delete; \
	ClassName& operator=(ClassName&&) = delete; \
	ClassName(const ClassName&) = default; \
	ClassName& operator=(const ClassName&) = default; \

#define HL_INTERFACE_DISABLE_COPY_AND_MOVE(ClassName) \
	ClassName() = default; \
	virtual ~ClassName() = default; \
	ClassName(ClassName&&) = delete; \
	ClassName& operator=(ClassName&&) = delete; \
	ClassName(const ClassName&) = default; \
	ClassName& operator=(const ClassName&) = default; \

#define HL_DISABLE_COPY(ClassName) \
	ClassName(const ClassName&) = delete; \
	ClassName& operator=(const ClassName&) = delete; \

#define HL_DISABLE_MOVE(ClassName) \
	ClassName(ClassName&&) = delete; \
	ClassName& operator=(ClassName&&) = delete; \

#define HL_DISABLE_COPY_AND_MOVE(ClassName) \
	ClassName(const ClassName&) = delete; \
	ClassName& operator=(const ClassName&) = delete; \
	ClassName(ClassName&&) = delete; \
	ClassName& operator=(ClassName&&) = delete; \




