
#include "imgui_menu.h"

namespace imgui_menu {

	using LangID = Translation::LangID;

	template<typename Fn>
	void Wnd::Create(const char* name, Fn fn, bool onTop, ImGuiWindowFlags flags, float fontScale) noexcept
	{
		bool oldIsShow{ isShow };

		if (isShow && ImGui::Begin(name, &isShow, flags)) {

			ImGui::SetWindowFontScale(fontScale);

			fn();

			ImGui::End();

			//if (onTop) { 
			//	ImGui::SetWindowFocus(name);
			//}
		}

		if (oldIsShow && !isShow)
			OnClosed();
	}

	void Wnd::EnableCursor(bool enable) noexcept
	{
		static bool bEnable{};

		if (bEnable != enable)
			::ShowCursor(enable);

		bEnable = enable;

		const auto control = RE::ControlMap::GetSingleton();

		if (control)
			control->ignoreKeyboardMouse = bEnable;

		isCursorEnabled = bEnable;
	}

	void Menu::SetLocation(const std::uint32_t idInterior, const char* nameInterior, const std::uint32_t idExterior, const char* nameExterior, const bool isInterior) noexcept
	{
		if (idInterior == location.uInterior && location.uExterior == idExterior)
			return;

		location.uInterior = idInterior;
		location.sInterior = nameInterior;
		location.uExterior = idExterior;
		location.sExterior = nameExterior;
		location.isInterior = isInterior;

		json_preset::Preset::GetSingleton().Get(GetLocationID(), isInterior);
	}

	void Menu::Show() noexcept 
	{
		if (Settings::Ini::GetSingleton().IsEnabledLog()) {

			EnableCursor(!IsCursorEnabled());

			if (IsCursorEnabled()) {
				Wnd::Show(true);
			}
		}
		else {

			Wnd::Show();

			EnableCursor(isShow);
		}
	}

	void Menu::OnKeyUp(std::uint16_t vkCode, bool isExtended) noexcept
	{
		//if (isExtended) {
			vkExtended = 0;
		//}
	}

	void Menu::OnKeyDown(std::uint16_t vkCode, bool isExtended, bool repeated) noexcept
	{
		if (repeated)
			return;

		auto& settings = Settings::Ini::GetSingleton();

		if (!waitForKeyOpen && !waitForKeyStop) {

			if (isExtended) {
				vkExtended = vkCode;
			}
			else if (!isExtended) {

				if (vkCode == settings.GetKey() && vkExtended == settings.GetExtendedKey())
					Show();
				
				if (vkCode == settings.GetKeyStop() && vkExtended == settings.GetExtendedKeyStop()) {

					auto& shadow = Shadow::Boost::GetSingleton();

					if (shadow.IsRunning()) {
						shadow.Pause();
					}
					else {
						shadow.Run();
					}
				}
			}
		} 
		else {

			if (isExtended) {
				vkExtended = vkCode;	
			}
			else {

				if (waitForKeyOpen) {

					settings.SetKey(vkCode);
					settings.SetExtendedKey(vkExtended);

					waitForKeyOpen = false;
				}

				if (waitForKeyStop) {

					settings.SetKeyStop(vkCode);
					settings.SetExtendedKeyStop(vkExtended);

					waitForKeyStop = false;
				}
			}
		}
	}

	void Menu::SetMenuStyle() noexcept
	{
		switch (Settings::Ini::GetSingleton().GetMenuStyle()) {

		case Settings::MenuStyle::Classic:

			ImGui::StyleColorsClassic();

			break;

		case Settings::MenuStyle::Light:

			ImGui::StyleColorsLight();

			break;

		case Settings::MenuStyle::Dark:

			ImGui::StyleColorsDark();

			break;
		}
	}

	bool Menu::Init(HWND hWnd, ID3D11Device* dev, ID3D11DeviceContext* device_context) noexcept
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;

		Translation::Language::GetSingleton().SetGlyph(io);

		SetMenuStyle();

		bool ret = ImGui_ImplWin32_Init(hWnd);
		ret |= ImGui_ImplDX11_Init(dev, device_context);

		if (Settings::Ini::GetSingleton().IsEnabledLog())
			isShow = true;

		return ret;
	}

	void Menu::Render() noexcept
	{
		if (!isShow)
			return;

		auto& settings = Settings::Ini::GetSingleton();

		auto drawData = ImGui::GetDrawData();

		if (drawData)
			ImGui_ImplDX11_RenderDrawData(drawData);

		auto& shadow = Shadow::Boost::GetSingleton();
		auto& lang = Translation::Language::GetSingleton();

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();

		ImGui::NewFrame();

		ImGuiWindowFlags flags = IsCursorEnabled() ? ImGuiWindowFlags_::ImGuiWindowFlags_MenuBar : 0;

		if (settings.IsTransparent()) {

			flags |= ImGuiWindowFlags_::ImGuiWindowFlags_NoTitleBar;
			flags |= ImGuiWindowFlags_::ImGuiWindowFlags_NoBackground;
		}

		float scale{ settings.GetFontScale() };

		Create(lang[LangID::CaptionWnd], [&] {
			
			if (IsCursorEnabled() && ImGui::BeginMenuBar()) {

				if (ImGui::BeginMenu(lang[LangID::MenuFile])) {

					if (ImGui::MenuItem(lang[LangID::MenuSettings])) {
						wndSettings.Show(true);
					}

					ImGui::Separator();

					if (ImGui::MenuItem(lang[LangID::MenuExit])) {
						Show();
					}

					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu(lang[LangID::MenuPreset])) {

					if (ImGui::MenuItem(lang[LangID::MenuEditor])) {
						wndEditor.Show(true);
					}

					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu(lang[LangID::MenuInfo])) {

					if (ImGui::MenuItem(lang[LangID::MenuHelp])) {
						wndHelp.Show(true);
					}

					if (ImGui::MenuItem(lang[LangID::MenuAbout])) {
						wndAbout.Show(true);
					}

					ImGui::EndMenu();
				}

				ImGui::EndMenuBar();
			}

			ImGui::Text(lang[LangID::DynValues]);

			ImGui::Text(lang[LangID::AvgMs], shadow.GetMs());
			//ImGui::Text(lang[LangID::DynValue], shadow.GetDynamicvalue());
			ImGui::Text(lang[LangID::ShadowDist], shadow.GetDirShadowDistance());
			ImGui::Text(lang[LangID::ObjectDist], shadow.GetLODFadeOutMultObjects());
			ImGui::Text(lang[LangID::ItemsDist], shadow.GetLODFadeOutMultItems());
			ImGui::Text(lang[LangID::ActorsDist], shadow.GetLODFadeOutMultActors());
			ImGui::Text(lang[LangID::GrassDist], shadow.GetGrassDistance());
			ImGui::Text(lang[LangID::BlockLvl2], shadow.GetBlockLevel2());
			ImGui::Text(lang[LangID::BlockLvl1], shadow.GetBlockLevel1());
			ImGui::Text(lang[LangID::BlockLvl0], shadow.GetBlockLevel0());

			ImGui::Separator();

			ImGui::Text(lang[LangID::StatValues]);

			ImGui::Text(lang[LangID::GodraysQuality], shadow.GetGrQuality());
			ImGui::Text(lang[LangID::GodraysGrid], shadow.GetGrGrid());
			ImGui::Text(lang[LangID::GodraysScale], shadow.GetGrScale());
			ImGui::Text(lang[LangID::GodraysCascade], shadow.GetGrCascade());

		}, 
			false,
			flags,
			scale);
		
		wndEditor.Create(lang[LangID::CaptEditor], [&] {

			if (ImGui::CollapsingHeader(lang[LangID::CurrentLocationHdr])) {

				auto& preset = json_preset::Preset::GetSingleton();

				ImGui::Text(lang[LangID::IsInterior], location.isInterior);
				ImGui::Text(lang[LangID::HasPreset], preset.HasPreset());
				ImGui::Text(lang[LangID::InteriorId], location.uInterior);
				ImGui::Text(lang[LangID::ExteriorId], location.uExterior);
				ImGui::Text(lang[LangID::InteriorName], location.sInterior.c_str());
				ImGui::Text(lang[LangID::ExteriorName], location.sExterior.c_str());

				if (ImGui::Button(lang[LangID::AddToPreset])) {
					preset.Add(GetLocationID(), location.isInterior);
				}

				ImGui::SameLine();

				if (ImGui::Button(lang[LangID::RemoveFromPreset])) {
					preset.Remove(GetLocationID(), location.isInterior);
				}

				ImGui::SameLine();

				if (ImGui::Button(lang[LangID::SavePreset]))
					preset.Save();
				}

			if (ImGui::CollapsingHeader(lang[LangID::MainHdr])) {

				float fpsTarget{ settings.GetFpsTarget() };

				if (ImGui::SliderFloat(lang[LangID::FpsTarget], &fpsTarget, 1.0f, 200.0f)) {

					settings.SetFpsTarget(fpsTarget);
					shadow.SetupFps();
				}

				int fpsDelay{ static_cast<int>(settings.GetFpsDelay()) };

				if (ImGui::SliderInt(lang[LangID::FpsDelay], &fpsDelay, 1, 200)) {
					settings.SetFpsDelay(static_cast<float>(fpsDelay));
				}

				float msTolerance{ settings.GetMsTolerance() };

				if (ImGui::SliderFloat(lang[LangID::MsTolerance], &msTolerance, 0.0f, 1000.0f)) {

					settings.SetMsTolerance(msTolerance);
					shadow.SetupFps();
				}
			}

			if (ImGui::CollapsingHeader(lang[LangID::ShadowHdr])) {

				bool enableShadow{ settings.GetShadowEnabled() };

				if (ImGui::Checkbox(lang[LangID::EnableShadow], &enableShadow)) {

					settings.SetShadowEnabled(enableShadow);
					shadow.EnableShadow();
				}

				float shadowFactor{ settings.GetShadowFactor() };

				if (ImGui::SliderFloat(lang[LangID::ShadowFactor], &shadowFactor, 0.1f, 100.0f)) {
					settings.SetShadowFactor(shadowFactor);
				}

				float minDistance{ settings.GetMinDistance() };

				if (ImGui::SliderFloat(lang[LangID::MinShadow], &minDistance, 100.0f, settings.GetMaxDistance())) {
					settings.SetMinDistance(minDistance);
				}

				float maxDistance{ settings.GetMaxDistance() };

				if (ImGui::SliderFloat(lang[LangID::MaxShadow], &maxDistance, settings.GetMinDistance(), 30000.0f)) {
					settings.SetMaxDistance(maxDistance);
				}
			}

			if (ImGui::CollapsingHeader(lang[LangID::LodHdr])) {

				bool enableLod{ settings.GetLodEnabled() };

				if (ImGui::Checkbox(lang[LangID::EnableLod], &enableLod)) {

					settings.SetLodEnabled(enableLod);
					shadow.EnableLod();
				}

				float lodFactor{ settings.GetLodFactor() };

				if (ImGui::SliderFloat(lang[LangID::LodFactor], &lodFactor, 0.1f, 100.0f)) {
					settings.SetLodFactor(lodFactor);
				}

				float minObjects{ settings.GetMinObjects() };

				if (ImGui::SliderFloat(lang[LangID::MinObjects], &minObjects, 1.0f, settings.GetMaxObjects())) {
					settings.SetMinObjects(minObjects);
				}

				float maxObjects{ settings.GetMaxObjects() };

				if (ImGui::SliderFloat(lang[LangID::MaxObjects], &maxObjects, settings.GetMinObjects(), 100.0f)) {
					settings.SetMaxObjects(maxObjects);
				}

				float minItems{ settings.GetMinItems() };

				if (ImGui::SliderFloat(lang[LangID::MinItems], &minItems, 1.0f, settings.GetMaxItems())) {
					settings.SetMinItems(minItems);
				}

				float maxItems{ settings.GetMaxItems() };

				if (ImGui::SliderFloat(lang[LangID::MaxItems], &maxItems, settings.GetMinItems(), 100.0f)) {
					settings.SetMaxItems(maxItems);
				}

				float minActors{ settings.GetMinActors() };

				if (ImGui::SliderFloat(lang[LangID::MinActors], &minActors, 1.0f, settings.GetMaxActors())) {
					settings.SetMinActors(minActors);
				}

				float maxActors{ settings.GetMaxActors() };

				if (ImGui::SliderFloat(lang[LangID::MaxActors], &maxActors, settings.GetMinActors(), 100.0f)) {
					settings.SetMaxActors(maxActors);
				}
			}

			if (ImGui::CollapsingHeader(lang[LangID::TerrainManagerHdr])) {

				bool blockEnable{ settings.GetBlockEnabled() };

				if (ImGui::Checkbox(lang[LangID::EnableBlockLevels], &blockEnable)) {

					settings.SetBlockEnabled(blockEnable);
					shadow.EnableBlock();
				}

				static std::vector<const char*> items = { lang[LangID::ComboLevel0], lang[LangID::ComboLevel1], lang[LangID::ComboLevel2], lang[LangID::ComboLevel3] };
				
				static int index{};

				ImGui::Combo(lang[LangID::Level], &index, items.data(), items.size());

				Settings::BlockLevel blockLevel = settings.GetBlockLevel(index);

				float distBlockLevel2{ blockLevel.fDistBlockLevel2 };

				if (ImGui::SliderFloat(lang[LangID::BlockLevel2], &distBlockLevel2, 1.0f, 200000.0f)) {
					blockLevel.fDistBlockLevel2 = distBlockLevel2;
				}
				
				float distBlockLevel1{ blockLevel.fDistBlockLevel1 };

				if (ImGui::SliderFloat(lang[LangID::BlockLevel1], &distBlockLevel1, 1.0f, blockLevel.fDistBlockLevel2)) {
					blockLevel.fDistBlockLevel1 = distBlockLevel1;
				}

				float distBlockLevel0{ blockLevel.fDistBlockLevel0 };

				if (ImGui::SliderFloat(lang[LangID::BlockLevel0], &distBlockLevel0, 0.0f, blockLevel.fDistBlockLevel1)) {
					blockLevel.fDistBlockLevel0 = distBlockLevel0;
				}

				settings.SetBlockLevel(index, blockLevel);
			}

			if (ImGui::CollapsingHeader(lang[LangID::GrassHdr])) {

				bool grassEnable{ settings.GetGrassEnabled() };

				if (ImGui::Checkbox(lang[LangID::EnableGrass], &grassEnable)) {

					settings.SetGrassEnabled(grassEnable);
					shadow.EnableGrass();
				}

				float grassFactor{ settings.GetGrassFactor() };

				if (ImGui::SliderFloat(lang[LangID::GrassFactor], &grassFactor, 0.1f, 100.0f)) {
					settings.SetGrassFactor(grassFactor);
				}

				float minGrass{ settings.GetMinGrass() };

				if (ImGui::SliderFloat(lang[LangID::MinGrass], &minGrass, 100.0f, settings.GetMaxGrass())) {
					settings.SetMinGrass(minGrass);
				}

				float maxGrass{ settings.GetMaxGrass() };

				if (ImGui::SliderFloat(lang[LangID::MaxGrass], &maxGrass, settings.GetMinGrass(), 30000.0f)) {
					settings.SetMaxGrass(maxGrass);
				}
			}

			if (ImGui::CollapsingHeader(lang[LangID::GodRaysHdr])) {

				bool godRaysEnable{ settings.GetGodRaysEnabled() };

				if (ImGui::Checkbox(lang[LangID::EnableGodRays], &godRaysEnable)) {

					settings.SetGodRaysEnabled(godRaysEnable);
					shadow.EnableGodRays();
				}

				std::int32_t grQuality{ settings.GetGrQuality() };
				std::int32_t grGrid{ settings.GetGrGrid() };
				float grScale{ settings.GetGrScale() };
				std::int32_t grCascade{ settings.GetGrCascade() };

				bool ret = ImGui::SliderInt(lang[LangID::GrQuality], &grQuality, 0, 3);
				ret |= ImGui::SliderInt(lang[LangID::GrGrid], &grGrid, 0, 256);
				ret |= ImGui::SliderFloat(lang[LangID::GrScale], &grScale, 0.1f, 2.0f);
				ret |= ImGui::SliderInt(lang[LangID::GrCascade], &grCascade, 1, 64);

				if (ret) {

					settings.SetGrQuality(grQuality);
					settings.SetGrGrid(grGrid);
					settings.SetGrScale(grScale);
					settings.SetGrCascade(grCascade);
					shadow.SetGodRays();
				}
			}

			if (ImGui::Button(lang[LangID::Load])) {

				settings.ReadSettings(true);

				shadow.ReloadStuff();
			}

			ImGui::SameLine();

			if (ImGui::Button(lang[LangID::Save])) {

				settings.WriteSettings(true);

				json_preset::Preset::GetSingleton().InitDefaultValues();
			}

			ImGui::SameLine();

			if (ImGui::Button(lang[LangID::Default])) {

				settings.SetDefault(true);

				shadow.ReloadStuff();
			}
		},
			true,
			ImGuiWindowFlags_::ImGuiWindowFlags_NoSavedSettings,
			scale);

		wndSettings.Create(lang[LangID::Settings], [&] {

			ImGui::Text(lang[LangID::Hotkeys]);

			ImGui::Text(lang[LangID::OpenMenu]);

			ImGui::SameLine();

			if (ImGui::Button(HotKey::GetNameKey(waitForKeyOpen, settings.GetKey(), settings.GetExtendedKey()).c_str())) {
				waitForKeyOpen = true;
			}

			ImGui::Text(lang[LangID::RunPausePlugin]);

			ImGui::SameLine();

			if (ImGui::Button(HotKey::GetNameKey(waitForKeyStop, settings.GetKeyStop(), settings.GetExtendedKeyStop()).c_str())) {
				waitForKeyStop = true;
			}

			bool bLog{ settings.IsEnabledLog() };

			if (ImGui::Checkbox(lang[LangID::EnableLog], &bLog)) {
				settings.EnableLog(bLog);
			}

			bool bTransparent{ settings.IsTransparent() };

			if (ImGui::Checkbox(lang[LangID::Transparent], &bTransparent)) {
				settings.EnableTransparent(bTransparent);
			}

			bool bPauseInMenu{ settings.IsPauseInMenu() };

			if (ImGui::Checkbox(lang[LangID::StopInMenu], &bPauseInMenu)) {
				settings.EnablePauseInMenu(bPauseInMenu);
			}

			static float fontScale{ settings.GetFontScale() };

			ImGui::SliderFloat(lang[LangID::FontScale], &fontScale, 1.0f, 10.0f);

			static std::vector<const char*> items = { lang[LangID::Classic], lang[LangID::Light], lang[LangID::Dark] };

			static int currItem{ static_cast<int>(settings.GetMenuStyle()) };

			if (ImGui::Combo(lang[LangID::MenuStyles], &currItem, items.data(), items.size())) {
				settings.SetMenuStyle(static_cast<Settings::MenuStyle>(currItem));
			}

			if (ImGui::Button(lang[LangID::Cancel])) {
				wndSettings.Show(false);
			}

			ImGui::SameLine();

			if (ImGui::Button(lang[LangID::SaveClose])) {

				settings.SetFontScale(fontScale);

				SetMenuStyle();

				if (!settings.IsPauseInMenu()) {
					shadow.Run();
				}

				settings.WriteSettings();

				wndSettings.Show(false);
			}
		}, 
			true, 
			ImGuiWindowFlags_::ImGuiWindowFlags_NoSavedSettings,
			scale);

		wndHelp.Create(lang[LangID::HelpHdr], [&] {

			ImGui::Text(lang[LangID::HelpText]);
		},
			true,
			ImGuiWindowFlags_::ImGuiWindowFlags_NoSavedSettings,
			scale);

		wndAbout.Create(lang[LangID::About], [&] {

			ImGui::Text(Version::Fullname.data());
			ImGui::Text(Version::SVersion.data());

			ImGui::Separator();

			ImGui::Text(lang[LangID::CreditsHdr]);
			ImGui::Text("ImGui by ocornut");
			ImGui::Text("SimpleIni by brofield");
			ImGui::Text("Jsoncpp");
			ImGui::Text("Special thanks to my ex patrons MasonJ and Cadman013 for their support");
			ImGui::Text("F4SE team for creating the script extender");
			ImGui::Text("Bethesda for creating Fallout 4");
			ImGui::Text("Fudgyduff for CommonLibF4");
			ImGui::Text("Thanks to all those who with their ideas and suggestions\nhelp me to improve this mod.");
		}, 
			true,
			ImGuiWindowFlags_::ImGuiWindowFlags_NoSavedSettings,
			scale);

		ImGui::Render();
	}

	Menu Menu::instance;
}