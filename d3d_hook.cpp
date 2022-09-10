
#include "d3d_hook.h"

#include "hook.h"
#include "shadow.h"
#include "settings.h"
#include "imgui_menu.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace Hook {

	bool D3D::Register() noexcept
	{
		HookFunc(REL::ID(254484), oldFuncs.d3dCreateDevice, (std::uintptr_t)D3D11CreateDeviceAndSwapChainHook);
		HookFunc(REL::ID(641385), oldFuncs.clipCursor, (std::uintptr_t)ClipCursorHook);
		HookFunc(REL::ID(514993), oldFuncs.regCls, (std::uintptr_t)RegisterClassExHook);

		return true;
	}

	void D3D::EnableCursor(bool enable)
	{
		static bool e{};

		if (e != enable)
			::ShowCursor(enable);

		e = enable;
	}

	ATOM __stdcall D3D::RegisterClassExHook(const WNDCLASSEXA* wnd)
	{
		WNDCLASSEX tmp = *wnd;

		oldFuncs.wndProc = wnd->lpfnWndProc;
		tmp.lpfnWndProc = WndProcHandler;

		return oldFuncs.regCls(&tmp);
	}

	BOOL __stdcall D3D::ClipCursorHook(RECT* lpRect)
	{
		if (imgui_menu::Menu::GetInstance().IsShow())
			*lpRect = oldRect;

		return oldFuncs.clipCursor(lpRect);
	}

	LRESULT __stdcall D3D::WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		auto& menu = imgui_menu::Menu::GetInstance();

		switch (msg) {

		case WM_KEYDOWN:

			{
				bool isPressed = (lParam & 0x40000000) != 0x0;
				auto& settings = Settings::Ini::GetInstance();

				if (!isPressed && !menu.WaitForKey() && wParam == settings.GetKey()) {

					menu.Show();

					EnableCursor(menu.IsShow());

					const auto input = RE::BSInputDeviceManager::GetSingleton();

					if (input)
						input->pollingEnabled = !menu.IsShow();
				}

				if (!isPressed && menu.WaitForKey()) {

					settings.GetKey() = static_cast<std::int32_t>(wParam);
					settings.SetNameKey();

					menu.DisableWaitForKey();
				}	
			}

			break;
		}

		if (menu.IsShow() && ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
			return true;

		return oldFuncs.wndProc(hWnd, msg, wParam, lParam);
	}

	HRESULT __stdcall D3D::ClearStateHook(ID3D11DeviceContext* This)
	{
		Shadow::Boost::GetInstance()();

		imgui_menu::Menu::GetInstance().Clear();

		return oldFuncs.d3dClear(This);
	}

	HRESULT __stdcall D3D::PresentHook(IDXGISwapChain* This, UINT SyncInterval, UINT Flags)
	{
		imgui_menu::Menu::GetInstance().Present();

		return oldFuncs.d3dPresent(This, SyncInterval, Flags);
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
		HRESULT ret = oldFuncs.d3dCreateDevice(pAdapter, DriverType, Software, Flags, pFeatureLevels, FeatureLevels, SDKVersion, pSwapChainDesc, ppSwapChain, ppDevice, pFeatureLevel, ppImmediateContext);

		if (FAILED(ret))
			return ret;

		std::uintptr_t* vtbl = (std::uintptr_t*)(*ppImmediateContext);
		vtbl = (std::uintptr_t*)vtbl[0];

		HookFunc(vtbl, 110, (std::uintptr_t)ClearStateHook, (std::uintptr_t*)&oldFuncs.d3dClear);

		vtbl = (std::uintptr_t*)(*ppSwapChain);
		vtbl = (std::uintptr_t*)vtbl[0];

		HookFunc(vtbl, 8, (std::uintptr_t)PresentHook, (std::uintptr_t*)&oldFuncs.d3dPresent);

		auto window = ::GetActiveWindow();

		::GetWindowRect(window, &oldRect);

		imgui_menu::Menu::GetInstance().Init(window, (*ppDevice), (*ppImmediateContext));

		return ret;
	}

	D3D::OldFuncs D3D::oldFuncs;

	RECT D3D::oldRect{};
}
