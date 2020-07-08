#include "HowlitePCH.h"
#include "UISystem.h"

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace Howlite {

	HUISystem::HUISystem(HWND hWnd, ID3D11Device* Device, ID3D11DeviceContext* Context)
	{
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::StyleColorsClassic();

        ImGuiIO& io = ImGui::GetIO(); 

        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;      
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;          
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;        

        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        ImGui_ImplWin32_Init(hWnd);
        ImGui_ImplDX11_Init(Device, Context);
	}

	HUISystem::~HUISystem()
	{
        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
	}

	void HUISystem::BeginFrame()
	{
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
	}

	void HUISystem::EndFrame()
	{
        ImGui::Render();
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }
	}

    void HUISystem::SetIsEnabled(bool IsEnabled) noexcept
    {
        mIsEnabled = IsEnabled;
    }

    bool HUISystem::IsEnabled() const noexcept
    {
        return mIsEnabled;
    }

    LRESULT HUISystem::HandleMessage(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
    {
        return ImGui_ImplWin32_WndProcHandler(hWnd, Msg, wParam, lParam);
    }

}