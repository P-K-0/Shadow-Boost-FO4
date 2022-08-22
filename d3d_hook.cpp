
#include "d3d_hook.h"

namespace Hook {

	//WNDPROC wndProc{};

	//LRESULT __stdcall WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	//{
	//	return DefWindowProc(hWnd, msg, wParam, lParam);
	//}

	bool D3D::Register()
	{
		HookImportFunc("d3d11.dll", "D3D11CreateDeviceAndSwapChain", d3dCreateDevice, (std::uintptr_t)D3D11CreateDeviceAndSwapChainHook);

		return true;
	}

	HRESULT __stdcall D3D::ClearStateHook(ID3D11DeviceContext* This)
	{
		auto& instance = Shadow::Boost::GetInstance();

		instance();

		auto& settings = Settings::Ini::GetInstance();

		if (settings.IsEnabledLog()) {

			ImGui_ImplDX11_NewFrame();
			ImGui_ImplWin32_NewFrame();

			ImGui::NewFrame();

			ImGui::SetNextWindowPos(ImVec2{ settings.GetLogPosX(), settings.GetLogPosY() });

			if (ImGui::Begin("Shadow Boost FO4 by PK0", nullptr)) {

				ImGui::Text("Avg ms frames : %.3f", instance.GetMs());
				ImGui::Text("Distance : %.3f", instance.GetDirShadowDistance());
				ImGui::Text("Dynamic Value : %.3f", instance.GetDynamicvalue());
				ImGui::Text("Min : %.3f", instance.GetMin());
				ImGui::Text("Max : %.3f", instance.GetMax());

				ImGui::End();
			}

			ImGui::Render();
		}

		return d3dClear(This);
	}

	HRESULT __stdcall D3D::PresentHook(IDXGISwapChain* This, UINT SyncInterval, UINT Flags)
	{
		if (Settings::Ini::GetInstance().IsEnabledLog())
			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		return d3dPresent(This, SyncInterval, Flags);
	}

	HRESULT __stdcall D3D::D3D11CreateDeviceAndSwapChainHook(IDXGIAdapter* pAdapter,
		D3D_DRIVER_TYPE DriverType,
		HMODULE Software, UINT Flags,
		const D3D_FEATURE_LEVEL* pFeatureLevels,
		UINT FeatureLevels,
		UINT SDKVersion,
		const DXGI_SWAP_CHAIN_DESC* pSwapChainDesc,
		IDXGISwapChain** ppSwapChain,
		ID3D11Device** ppDevice,
		D3D_FEATURE_LEVEL* pFeatureLevel,
		ID3D11DeviceContext** ppImmediateContext)
	{
		HRESULT ret = d3dCreateDevice(pAdapter, DriverType, Software, Flags, pFeatureLevels, FeatureLevels, SDKVersion, pSwapChainDesc, ppSwapChain, ppDevice, pFeatureLevel, ppImmediateContext);

		if (FAILED(ret))
			return ret;

		std::uintptr_t* vtbl = (std::uintptr_t*)(*ppImmediateContext);
		vtbl = (std::uintptr_t*)vtbl[0];

		HookFunc(vtbl, 110, (std::uintptr_t)ClearStateHook, (std::uintptr_t*)&d3dClear);

		vtbl = (std::uintptr_t*)(*ppSwapChain);
		vtbl = (std::uintptr_t*)vtbl[0];

		HookFunc(vtbl, 8, (std::uintptr_t)PresentHook, (std::uintptr_t*)&d3dPresent);

		auto window = ::GetActiveWindow();

		//wndProc = (WNDPROC)GetWindowLongPtr(window, GWLP_WNDPROC);
		//SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProcHandler);

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;

		ImGui::StyleColorsDark();

		ImGui_ImplWin32_Init(window);
		ImGui_ImplDX11_Init((*ppDevice), (*ppImmediateContext));

		return ret;
	}

	D3D::CreateDeviceAndSwapChain D3D::d3dCreateDevice;
	D3D::ClearState D3D::d3dClear;
	D3D::Present D3D::d3dPresent;
}