#pragma once

#include <d3d11.h>

#include "imgui.h"
#include "IMGUI/imgui_impl_dx11.h"
#include "IMGUI/imgui_impl_win32.h"

#include "hook.h"
#include "shadow.h"

namespace Hook {

	class D3D {

		using CreateDeviceAndSwapChain = decltype(&D3D11CreateDeviceAndSwapChain);
		using ClearState = HRESULT (__stdcall*)(ID3D11DeviceContext*);
		using Present = HRESULT(__stdcall*)(IDXGISwapChain*, UINT, UINT);

	public:

		static [[nodiscard]] bool Register();

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

		static CreateDeviceAndSwapChain d3dCreateDevice;
		static ClearState d3dClear;
		static Present d3dPresent;
	};
}
