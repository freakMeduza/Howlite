#include "HowlitePCH.h"
#include "DXGIInfoQueue.h"

namespace Howlite {

	HDXGIInfoQueue::HDXGIInfoQueue()
	{
		typedef HRESULT(WINAPI* DXGIGetDebugInterface)(REFIID, void**);

		const auto hModDxgiDebug = LoadLibraryEx("dxgidebug.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);

		if(hModDxgiDebug == nullptr)
		{
			H_ERROR(::GetLastError())
		}

		const auto DxgiGetDebugInterface = reinterpret_cast<DXGIGetDebugInterface>(reinterpret_cast<void*>(GetProcAddress(hModDxgiDebug, "DXGIGetDebugInterface")));

		if (DxgiGetDebugInterface == nullptr)
		{
			H_ERROR(::GetLastError())
		}

		DxgiGetDebugInterface(__uuidof(IDXGIInfoQueue), &mInfoQueue);
	}

	HDXGIInfoQueue::~HDXGIInfoQueue()
	{

	}

	void HDXGIInfoQueue::ClearMessageQueue()
	{
		mNext = mInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
	}

	std::vector<std::string> HDXGIInfoQueue::GetMessageQueue()
	{
		std::vector<std::string> messages;

		const auto end = mInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
		
		for(auto i = mNext; i < end; ++i)
		{
			SIZE_T length;
			mInfoQueue->GetMessage(DXGI_DEBUG_ALL, i, nullptr, &length);
			std::vector<uint8_t> buffer(length);
			auto message = reinterpret_cast<DXGI_INFO_QUEUE_MESSAGE*>(buffer.data());
			mInfoQueue->GetMessage(DXGI_DEBUG_ALL, i, message, &length);
			messages.emplace_back(message->pDescription);
		}

		return messages;
	}

}