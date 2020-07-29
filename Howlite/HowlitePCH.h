#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <set>
#include <string>
#include <tuple>
#include <array>
#include <memory>
#include <sstream>
#include <optional>
#include <exception>
#include <functional>
#include <unordered_map>

#include "resource.h"

#include <wrl.h>
#include <d3d11.h>
#include <dxgidebug.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "D3DCompiler.lib")
#pragma comment(lib, "legacy_stdio_definitions.lib")

#ifdef _DEBUG
#include <DXErr.h>
#pragma comment(lib, "DxErr.lib")
#endif

#include <DirectXMath.h>

#include "Engine/Exception.h"
#include "Common/Tick.h"
#include "Renderer/DXGIInfoQueue.h"
#include "Renderer/UI/Imgui/imgui.h"
#include "Renderer/UI/Imgui/imgui_impl_dx11.h"
#include "Renderer/UI/Imgui/imgui_impl_win32.h"

template<typename Type> using ScopedPointer = std::unique_ptr<Type>;
template<typename Type> using SharedPointer = std::shared_ptr<Type>;

using HID = size_t;

inline static HID CreateObjectID(const std::string& Value) 
{
	return std::hash<std::string>{}(Value);
}

template<typename Type, typename ... Arg>
constexpr ScopedPointer<Type> CreateScopedPointer(Arg&& ... Args)
{
	return std::make_unique<Type>(std::forward<Arg>(Args)...);
}

template<typename Type, typename ... Arg>
constexpr SharedPointer<Type> CreateSharedPointer(Arg&& ... Args)
{
	return std::make_shared<Type>(std::forward<Arg>(Args)...);
}

#define H_UNUSED(x) \
static_cast<void>((x));

#define H_ERROR(x) \
throw Howlite::HException{ __FILE__, __LINE__, (HRESULT)x, Howlite::EExceptionType::Platform }; 

#define H_ASSERT(x, ErrorMessage) \
{ \
if(!(x)) \
{ \
throw Howlite::HException{ __FILE__, __LINE__, ErrorMessage }; \
} \
}

#ifdef _DEBUG
#define H_DXGI_INFOQUEUE(GraphicSystem) \
Howlite::HDXGIInfoQueue& DXGIInfoQueue = GetDXGIInfoQueue(GraphicSystem);

#define H_DX_SAFECALL(x) \
{ \
DXGIInfoQueue.ClearMessageQueue(); \
HRESULT hresult = (x); \
if(FAILED(hresult)) \
{ \
std::string message; \
for(const std::string& line : DXGIInfoQueue.GetMessageQueue()) \
{ \
message += line; \
} \
throw Howlite::HException{ __FILE__, __LINE__, hresult, Howlite::EExceptionType::Graphic, message }; \
} \
}

#else
#define H_DXGI_INFOQUEUE(GraphicSystem)

#define H_DX_SAFECALL(x) \
{ \
HRESULT hresult = (x); \
if(FAILED(hresult)) \
{ \
throw Howlite::HException{ __FILE__, __LINE__, hresult, Howlite::EExceptionType::Graphic }; \
} \
}

#endif

#define H_TYPENAME(x) #x

#define H_DECLARE_EVENT_TYPE(x) \
static inline EHEventType GetStaticEventType() noexcept { return x; } \
inline EHEventType GetEventType() const noexcept override { return GetStaticEventType(); } \
inline const char* GetEventName() const noexcept override { return #x; } 

#define H_BIND_EVENT_CALLBACK(x) \
std::bind(&x, this, std::placeholders::_1)

