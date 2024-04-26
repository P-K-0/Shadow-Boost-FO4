
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

	ATOM __stdcall D3D::RegisterClassExHook(const WNDCLASSEXA* wnd)
	{
		WNDCLASSEX tmp = *wnd;

		oldFuncs.wndProc = wnd->lpfnWndProc;
		tmp.lpfnWndProc = WndProcHandler;

		return oldFuncs.regCls(&tmp);
	}

	BOOL __stdcall D3D::ClipCursorHook(RECT* lpRect)
	{
		if (imgui_menu::Menu::GetSingleton().IsShow())
			*lpRect = oldRect;

		return oldFuncs.clipCursor(lpRect);
	}

	LRESULT __stdcall D3D::WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		auto& menu = imgui_menu::Menu::GetSingleton();

		switch (msg) {

		case WM_ACTIVATE:

			if (LOWORD(wParam) == WA_INACTIVE) {				
				Shadow::Boost::GetSingleton().Pause();
			}
			else {
				Shadow::Boost::GetSingleton().Run();
			}

			break;

		case WM_SYSKEYUP:
		case WM_KEYUP:
		case WM_SYSKEYDOWN:
		case WM_KEYDOWN:

			{
				std::uint16_t vkCode = LOWORD(wParam);
				std::uint16_t keyFlags = HIWORD(lParam);
				bool isExtended{};

				switch (vkCode) {

				case VK_CONTROL:
				case VK_SHIFT:
				case VK_MENU:

					isExtended = true;

					break;
				}

				bool repeated = (keyFlags & KF_REPEAT) == KF_REPEAT;  
				bool released = (keyFlags & KF_UP) == KF_UP;

				//bool isExtended = (keyFlags & KF_EXTENDED) == KF_EXTENDED;

				if (released) {
					menu.OnKeyUp(vkCode, isExtended);
				}
				else {
					menu.OnKeyDown(vkCode, isExtended, repeated);
				}
			}

			break;
		}

		if (menu.IsCursorEnabled() && ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
			return true;

		return oldFuncs.wndProc(hWnd, msg, wParam, lParam);
	}

	HRESULT __stdcall D3D::ClearStateHook(ID3D11DeviceContext* This)
	{
		Shadow::Boost::GetSingleton()();

		return oldFuncs.d3dClear(This);
	}

	HRESULT __stdcall D3D::PresentHook(IDXGISwapChain* This, UINT SyncInterval, UINT Flags)
	{
		imgui_menu::Menu::GetSingleton().Render();

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

		imgui_menu::Menu::GetSingleton().Init(window, (*ppDevice), (*ppImmediateContext));

		return ret;
	}

	D3D::OldFuncs D3D::oldFuncs;

	RECT D3D::oldRect{};
}
