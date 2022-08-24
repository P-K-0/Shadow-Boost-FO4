#pragma once

#include <d3d11.h>

#include "imgui.h"
#include "IMGUI/imgui_impl_dx11.h"
#include "IMGUI/imgui_impl_win32.h"

#include "hook.h"
#include "shadow.h"
#include "settings.h"

namespace Hook {

	class D3D {

		using CreateDeviceAndSwapChain = decltype(&D3D11CreateDeviceAndSwapChain);
		using ClearState = HRESULT (__stdcall*)(ID3D11DeviceContext*);
		using Present = HRESULT(__stdcall*)(IDXGISwapChain*, UINT, UINT);
		using ClipCur = decltype(&ClipCursor);

	public:

		static [[nodiscard]] bool Register() noexcept;

	private:

		D3D() {}
		~D3D() {}

		D3D(const D3D&) = delete;
		D3D(D3D&&) = delete;

		D3D& operator=(const D3D&) = delete;
		D3D& operator=(D3D&&) = delete;

		static HRESULT __stdcall ClearStateHook(ID3D11DeviceContext*);
		static HRESULT __stdcall PresentHook(IDXGISwapChain*, UINT, UINT);
		static HRESULT __stdcall D3D11CreateDeviceAndSwapChainHook(IDXGIAdapter*, D3D_DRIVER_TYPE, HMODULE, UINT, const D3D_FEATURE_LEVEL*, UINT, UINT, const DXGI_SWAP_CHAIN_DESC*, IDXGISwapChain**, ID3D11Device**, D3D_FEATURE_LEVEL*, ID3D11DeviceContext**);
		static LRESULT __stdcall WndProcHandler(HWND, UINT, WPARAM, LPARAM);

		static BOOL __stdcall ClipCursorHook(RECT*);
		static void EnableCursor(bool enable);

		struct OldFuncs {

			CreateDeviceAndSwapChain d3dCreateDevice;
			ClearState d3dClear;
			Present d3dPresent;
			ClipCur clipCursor;
			WNDPROC wndProc;
		};

		static OldFuncs oldFuncs;

		static RECT oldRect;
		static bool isShow;
	};
}
