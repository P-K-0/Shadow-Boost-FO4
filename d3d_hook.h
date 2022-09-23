#pragma once

#include "pch.h"

#include <d3d11.h>

namespace Hook {

	class D3D {

		using CreateDeviceAndSwapChain = decltype(&D3D11CreateDeviceAndSwapChain);
		using ClearState = HRESULT (__stdcall*)(ID3D11DeviceContext*);
		using Present = HRESULT(__stdcall*)(IDXGISwapChain*, UINT, UINT);
		using ClipCur = decltype(&ClipCursor);
		using RegisterClsEx = decltype(&RegisterClassEx);

	public:

		[[nodiscard]] static bool Register() noexcept;

	private:

		D3D() noexcept {}
		~D3D() noexcept {}

		D3D(const D3D&) = delete;
		D3D(D3D&&) = delete;

		D3D& operator=(const D3D&) = delete;
		D3D& operator=(D3D&&) = delete;

		static HRESULT __stdcall ClearStateHook(ID3D11DeviceContext*);
		static HRESULT __stdcall PresentHook(IDXGISwapChain*, UINT, UINT);
		static HRESULT __stdcall D3D11CreateDeviceAndSwapChainHook(IDXGIAdapter*, D3D_DRIVER_TYPE, HMODULE, UINT, const D3D_FEATURE_LEVEL*, UINT, UINT, const DXGI_SWAP_CHAIN_DESC*, IDXGISwapChain**, ID3D11Device**, D3D_FEATURE_LEVEL*, ID3D11DeviceContext**);
		static LRESULT __stdcall WndProcHandler(HWND, UINT, WPARAM, LPARAM);
		static ATOM __stdcall RegisterClassExHook(const WNDCLASSEXA*);
		static BOOL __stdcall ClipCursorHook(RECT*);

		static void EnableCursor(bool enable) noexcept;

		struct OldFuncs {

			CreateDeviceAndSwapChain d3dCreateDevice;
			ClearState d3dClear;
			Present d3dPresent;
			ClipCur clipCursor;
			RegisterClsEx regCls;
			WNDPROC wndProc;
		};

		static OldFuncs oldFuncs;

		static RECT oldRect;
	};
}
