#include "HowlitePCH.h"
#include "Engine/Engine.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	H_UNUSED(hPrevInstance)
	H_UNUSED(nShowCmd)

	try {
		auto engine = CreateScopedPointer<Howlite::HEngine>(hInstance, lpCmdLine);

		return engine->Run();
	}
	catch(const std::exception& Exception)
	{
		::MessageBox(nullptr, Exception.what(), "Error!", MB_ICONERROR | MB_OK);
	}

	return -1;
}