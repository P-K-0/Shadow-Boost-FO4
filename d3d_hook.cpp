
#include "d3d_hook.h"

namespace Hook {

	bool D3D::Register() noexcept
	{
		HookImportFunc("d3d11.dll", "D3D11CreateDeviceAndSwapChain", oldFuncs.d3dCreateDevice, (std::uintptr_t)D3D11CreateDeviceAndSwapChainHook);

		HookImportFunc("User32.dll", "ClipCursor", oldFuncs.clipCursor, (std::uintptr_t)ClipCursorHook);

		return true;
	}

	void D3D::EnableCursor(bool enable)
	{
		static bool e{};

		if (e != enable)
			::ShowCursor(enable);

		e = enable;
	}

	BOOL __stdcall D3D::ClipCursorHook(RECT* lpRect)
	{
		if (isShow)
			*lpRect = oldRect;

		return oldFuncs.clipCursor(lpRect);
	}

	LRESULT __stdcall D3D::WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch (msg) {

		case WM_KEYDOWN:

			{
				bool isPressed = (lParam & 0x40000000) != 0x0;

				if (!isPressed && wParam == VK_F12) {

					isShow = !isShow;

					EnableCursor(isShow);

					auto input = (*g_inputDeviceMgr);

					if (input)
						input->pollingEnabled = !isShow;
				}
			}

			break;
		}

		if (isShow && ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
			return true;

		return oldFuncs.wndProc(hWnd, msg, wParam, lParam);
	}

	HRESULT __stdcall D3D::ClearStateHook(ID3D11DeviceContext* This)
	{
		auto& settings = Settings::Ini::GetInstance();

		if (settings.IsEnabledLog() || isShow) {

			auto& instance = Shadow::Boost::GetInstance();

			ImGui_ImplDX11_NewFrame();
			ImGui_ImplWin32_NewFrame();

			ImGui::NewFrame();

			if (ImGui::Begin("Shadow Boost FO4 by PK0", nullptr) && instance) {

				if (ImGui::CollapsingHeader("Status")) {

					//ImGui::Text("Fps : %.3f", instance.GetFps());
					ImGui::Text("Avg ms frames : %.3f", instance.GetMs());
					ImGui::Text("Dynamic Value : %.3f", instance.GetDynamicvalue());
					ImGui::Text("Shadow Dist : %.3f", instance.GetDirShadowDistance());
					ImGui::Text("Objects Dist : %.3f", instance.GetLODFadeOutMultObjects());
					ImGui::Text("Items Dist : %.3f", instance.GetLODFadeOutMultItems());
					ImGui::Text("Actors Dist : %.3f", instance.GetLODFadeOutMultActors());
					ImGui::Text("Grass Dist : %.3f", instance.GetGrassDistance());
					ImGui::Text("BlockLevel 2 Dist : %.3f", instance.GetBlockLevel2());
					ImGui::Text("BlockLevel 1 Dist : %.3f", instance.GetBlockLevel1());
					ImGui::Text("BlockLevel 0 Dist : %.3f", instance.GetBlockLevel0());
					ImGui::Text("God rays Quality : %i", instance.GetGrQuality());
					ImGui::Text("God rays Grid : %i", instance.GetGrGrid());
					ImGui::Text("God rays Scale : %.3f", instance.GetGrScale());
					ImGui::Text("God rays Cascade : %i", instance.GetGrCascade());
				}

				if (ImGui::CollapsingHeader("Help")) {

					ImGui::Text("It is highly recommended to configure all options,\n"
								"because the default options may not be compatible\n"
								"with all systems, so better check the ini files of\n"
								"the game to get a precise idea of which settings\n"
								"need to be changed.");
					ImGui::Text("To disable a feature just set the minimum and maximum\n"
								"equal for example(shadow min = 3000.0 and shadow max = 3000.0).");
				}

				if (ImGui::CollapsingHeader("Main")) {

					ImGui::SliderFloat(" Fps Target", &settings.GetFpsTarget(), 1.0f, 200.0f);

					int value{ static_cast<int>(settings.GetFpsDelay()) };

					ImGui::SliderInt(" Fps Delay", &value, 1.0f, 200.0f);

					settings.GetFpsDelay() = static_cast<float>(value);
				}

				if (ImGui::CollapsingHeader("Shadow")) {

					ImGui::SliderFloat(" Shadow Factor", &settings.GetShadowFactor(), 0.1f, 100.0f);
					
					ImGui::SliderFloat(" Min Shadow", &settings.GetMinDistance(), 100.0f, settings.GetMaxDistance());
					ImGui::SliderFloat(" Max Shadow", &settings.GetMaxDistance(), settings.GetMinDistance(), 30000.0f);
				}

				if (ImGui::CollapsingHeader("Lod")) {

					ImGui::SliderFloat(" Lod Factor", &settings.GetLodFactor(), 0.1f, 100.0f);
					
					ImGui::SliderFloat(" Min Objects", &settings.GetMinObjects(), 1.0f, settings.GetMaxObjects());
					ImGui::SliderFloat(" Max Objects", &settings.GetMaxObjects(), settings.GetMinObjects(), 100.0f);

					ImGui::SliderFloat(" Min Items", &settings.GetMinItems(), 1.0f, settings.GetMaxItems());
					ImGui::SliderFloat(" Max Items", &settings.GetMaxItems(), settings.GetMinItems(), 100.0f);

					ImGui::SliderFloat(" Min Actors", &settings.GetMinActors(), 1.0f, settings.GetMaxActors());
					ImGui::SliderFloat(" Max Actors", &settings.GetMaxActors(), settings.GetMinActors(), 100.0f);
				}

				if (ImGui::CollapsingHeader("Terrain Manager")) {

					ImGui::SliderFloat(" Block Factor", &settings.GetBlockFactor(), 0.1f, 100.0f);

					ImGui::SliderFloat(" Min Block Level 2", &settings.GetMinBlockLevel2(), 1.0f, settings.GetMaxBlockLevel2());
					ImGui::SliderFloat(" Max Block Level 2", &settings.GetMaxBlockLevel2(), settings.GetMinBlockLevel2(), 100000.0f);

					ImGui::SliderFloat(" Min Block Level 1", &settings.GetMinBlockLevel1(), 1.0f, settings.GetMaxBlockLevel1());
					ImGui::SliderFloat(" Max Block Level 1", &settings.GetMaxBlockLevel1(), settings.GetMinBlockLevel1(), 100000.0f);

					ImGui::SliderFloat(" Min Block Level 0", &settings.GetMinBlockLevel0(), 1.0f, settings.GetMaxBlockLevel0());
					ImGui::SliderFloat(" Max Block Level 0", &settings.GetMaxBlockLevel0(), settings.GetMinBlockLevel0(), 100000.0f);
				}

				if (ImGui::CollapsingHeader("Grass")) {

					ImGui::SliderFloat(" Grass Factor", &settings.GetGrassFactor(), 0.1f, 100.0f);

					ImGui::SliderFloat(" Min Grass", &settings.GetMinGrass(), 100.0f, settings.GetMaxGrass());
					ImGui::SliderFloat(" Max Grass", &settings.GetMaxGrass(), settings.GetMinGrass(), 30000.0f);
				}

				if (ImGui::CollapsingHeader("God Rays")) {

					ImGui::SliderInt(" Quality", &settings.GetGrQuality(), 0, 3);
					ImGui::SliderInt(" Grid", &settings.GetGrGrid(), 1, 32);
					ImGui::SliderFloat(" Scale", &settings.GetGrScale(), 0.1f, 2.0f);
					ImGui::SliderInt(" Cascade", &settings.GetGrCascade(), 1, 32);

					instance.SetGodRays();
				}

				if (ImGui::CollapsingHeader("Others")) {

					ImGui::Checkbox("Enable Log (if activated it means that\nthis window always remains active)", &settings.IsEnabledLog());
				}

				if (ImGui::CollapsingHeader("Thanks and credits")) {

					ImGui::Text("ImGui by ocornut");
					ImGui::Text("SimpleIni by brofield");
					ImGui::Text("Special thanks to my patron MasonJ for his support");
					ImGui::Text("F4SE team for creating the script extender");
					ImGui::Text("Bethesda for creating Fallout 4");
					ImGui::Text("Thanks to all those who with their ideas and suggestions\nhelp me to improve this mod.");
				}

				if (ImGui::Button("Save"))
					settings.WriteSettings();

				ImGui::End();
			}

			ImGui::Render();
		}

		return oldFuncs.d3dClear(This);
	}

	HRESULT __stdcall D3D::PresentHook(IDXGISwapChain* This, UINT SyncInterval, UINT Flags)
	{
		Shadow::Boost::GetInstance()();

		if (Settings::Ini::GetInstance().IsEnabledLog() || isShow) {

			auto drawData = ImGui::GetDrawData();

			if (drawData)
				ImGui_ImplDX11_RenderDrawData(drawData);
		}

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

		oldFuncs.wndProc = (WNDPROC)GetWindowLongPtr(window, GWLP_WNDPROC);
		SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProcHandler);

		::GetWindowRect(window, &oldRect);

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;

		ImGui::StyleColorsDark();

		ImGui_ImplWin32_Init(window);
		ImGui_ImplDX11_Init((*ppDevice), (*ppImmediateContext));

		return ret;
	}

	D3D::OldFuncs D3D::oldFuncs;

	RECT D3D::oldRect{};
	bool D3D::isShow{};
}
