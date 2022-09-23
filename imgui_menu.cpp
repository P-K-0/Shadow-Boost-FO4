
#include "imgui_menu.h"

#include "shadow.h"
#include "settings.h"
#include "json_preset.h"
#include "translation.h"

namespace imgui_menu {

	using LangID = Translation::LangID;

	void Menu::SetLocation(const std::uint32_t idInterior, const char* nameInterior, const std::uint32_t idExterior, const char* nameExterior, const bool isInterior) noexcept
	{
		if (idInterior == location.uInterior && location.uExterior == idExterior)
			return;

		location.uInterior = idInterior;
		location.sInterior = nameInterior;
		location.uExterior = idExterior;
		location.sExterior = nameExterior;
		location.isInterior = isInterior;

		json_preset::Preset::GetInstance().Get(GetLocationID(), isInterior);
	}

	bool Menu::Init(HWND hWnd, ID3D11Device* dev, ID3D11DeviceContext* device_context) noexcept
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;

		Translation::Language::GetInstance().SetGlyph(io);

		ImGui::StyleColorsDark();

		bool ret = ImGui_ImplWin32_Init(hWnd);
		ret |= ImGui_ImplDX11_Init(dev, device_context);

		if (ret) {

			device = dev;
			context = device_context;
		}

		return ret;
	}

	void Menu::Clear() noexcept
	{
		auto& settings = Settings::Ini::GetInstance();

		if (!settings.IsEnabledLog() && !isShow)
			return;

		auto& shadow = Shadow::Boost::GetInstance();
		auto& lang = Translation::Language::GetInstance();

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();

		ImGui::NewFrame();

		flags = 0;

		if (settings.IsTransparent()) {

			flags |= ImGuiWindowFlags_::ImGuiWindowFlags_NoTitleBar;
			flags |= ImGuiWindowFlags_::ImGuiWindowFlags_NoBackground;
		}

		if (ImGui::Begin(lang[LangID::CaptionWnd], nullptr, flags) && shadow) {

			if (ImGui::CollapsingHeader(lang[LangID::StatusHdr])) {

				ImGui::Text(lang[LangID::AvgMs], shadow.GetMs());
				ImGui::Text(lang[LangID::DynValue], shadow.GetDynamicvalue());
				ImGui::Text(lang[LangID::ShadowDist], shadow.GetDirShadowDistance());
				ImGui::Text(lang[LangID::ObjectDist], shadow.GetLODFadeOutMultObjects());
				ImGui::Text(lang[LangID::ItemsDist], shadow.GetLODFadeOutMultItems());
				ImGui::Text(lang[LangID::ActorsDist], shadow.GetLODFadeOutMultActors());
				ImGui::Text(lang[LangID::GrassDist], shadow.GetGrassDistance());
				ImGui::Text(lang[LangID::BlockLvl2], shadow.GetBlockLevel2());
				ImGui::Text(lang[LangID::BlockLvl1], shadow.GetBlockLevel1());
				ImGui::Text(lang[LangID::BlockLvl0], shadow.GetBlockLevel0());
				ImGui::Text(lang[LangID::GodraysQuality], shadow.GetGrQuality());
				ImGui::Text(lang[LangID::GodraysGrid], shadow.GetGrGrid());
				ImGui::Text(lang[LangID::GodraysScale], shadow.GetGrScale());
				ImGui::Text(lang[LangID::GodraysCascade], shadow.GetGrCascade());
			}

			if (ImGui::CollapsingHeader(lang[LangID::CurrentLocationHdr])) {

				auto& preset = json_preset::Preset::GetInstance();

				ImGui::Text(lang[LangID::IsInterior], location.isInterior);
				ImGui::Text(lang[LangID::HasPreset], preset.HasPreset());
				ImGui::Text(lang[LangID::InteriorId], location.uInterior);
				ImGui::Text(lang[LangID::ExteriorId], location.uExterior);
				ImGui::Text(lang[LangID::InteriorName], location.sInterior.c_str());
				ImGui::Text(lang[LangID::ExteriorName], location.sExterior.c_str());

				if (ImGui::Button(lang[LangID::AddToPreset]))
					preset.Add(GetLocationID(), location.isInterior);

				ImGui::SameLine();

				if (ImGui::Button(lang[LangID::RemoveFromPreset]))
					preset.Remove(GetLocationID(), location.isInterior);

				ImGui::SameLine();

				if (ImGui::Button(lang[LangID::SavePreset]))
					preset.Save();
			}

			if (ImGui::CollapsingHeader(lang[LangID::HelpHdr])) {

				ImGui::Text(lang[LangID::HelpText]);
			}

			if (ImGui::CollapsingHeader(lang[LangID::MainHdr])) {

				if (ImGui::SliderFloat(lang[LangID::FpsTarget], &settings.GetFpsTarget(), 1.0f, 200.0f))
					shadow.SetupFps();

				int value{ static_cast<int>(settings.GetFpsDelay()) };

				if (ImGui::SliderInt(lang[LangID::FpsDelay], &value, 1, 200))
					settings.GetFpsDelay() = static_cast<float>(value);

				if (ImGui::SliderFloat(lang[LangID::MsTolerance], &settings.GetMsTolerance(), 0.0f, 1000.0f))
					shadow.SetupFps();
			}

			if (ImGui::CollapsingHeader(lang[LangID::ShadowHdr])) {

				if (ImGui::Checkbox(lang[LangID::EnableShadow], &settings.GetShadowEnabled()))
					shadow.EnableShadow();

				ImGui::SliderFloat(lang[LangID::ShadowFactor], &settings.GetShadowFactor(), 0.1f, 100.0f);

				ImGui::SliderFloat(lang[LangID::MinShadow], &settings.GetMinDistance(), 100.0f, settings.GetMaxDistance());
				ImGui::SliderFloat(lang[LangID::MaxShadow], &settings.GetMaxDistance(), settings.GetMinDistance(), 30000.0f);
			}

			if (ImGui::CollapsingHeader(lang[LangID::LodHdr])) {

				if (ImGui::Checkbox(lang[LangID::EnableLod], &settings.GetLodEnabled()))
					shadow.EnableLod();

				ImGui::SliderFloat(lang[LangID::LodFactor], &settings.GetLodFactor(), 0.1f, 100.0f);

				ImGui::SliderFloat(lang[LangID::MinObjects], &settings.GetMinObjects(), 1.0f, settings.GetMaxObjects());
				ImGui::SliderFloat(lang[LangID::MaxObjects], &settings.GetMaxObjects(), settings.GetMinObjects(), 100.0f);

				ImGui::SliderFloat(lang[LangID::MinItems], &settings.GetMinItems(), 1.0f, settings.GetMaxItems());
				ImGui::SliderFloat(lang[LangID::MaxItems], &settings.GetMaxItems(), settings.GetMinItems(), 100.0f);

				ImGui::SliderFloat(lang[LangID::MinActors], &settings.GetMinActors(), 1.0f, settings.GetMaxActors());
				ImGui::SliderFloat(lang[LangID::MaxActors], &settings.GetMaxActors(), settings.GetMinActors(), 100.0f);
			}

			if (ImGui::CollapsingHeader(lang[LangID::TerrainManagerHdr])) {

				if (ImGui::Checkbox(lang[LangID::EnableBlockLevels], &settings.GetBlockEnabled()))
					shadow.EnableBlock();

				ImGui::SliderFloat(lang[LangID::BlockFactor], &settings.GetBlockFactor(), 0.1f, 100.0f);

				ImGui::SliderFloat(lang[LangID::MinBlockLevel2], &settings.GetMinBlockLevel2(), 1.0f, settings.GetMaxBlockLevel2());
				ImGui::SliderFloat(lang[LangID::MaxBlockLevel2], &settings.GetMaxBlockLevel2(), settings.GetMinBlockLevel2(), 100000.0f);

				ImGui::SliderFloat(lang[LangID::MinBlockLevel1], &settings.GetMinBlockLevel1(), 1.0f, settings.GetMaxBlockLevel1());
				ImGui::SliderFloat(lang[LangID::MaxBlockLevel1], &settings.GetMaxBlockLevel1(), settings.GetMinBlockLevel1(), 100000.0f);

				ImGui::SliderFloat(lang[LangID::MinBlockLevel0], &settings.GetMinBlockLevel0(), 1.0f, settings.GetMaxBlockLevel0());
				ImGui::SliderFloat(lang[LangID::MaxBlockLevel0], &settings.GetMaxBlockLevel0(), settings.GetMinBlockLevel0(), 100000.0f);
			}

			if (ImGui::CollapsingHeader(lang[LangID::GrassHdr])) {

				if (ImGui::Checkbox(lang[LangID::EnableGrass], &settings.GetGrassEnabled()))
					shadow.EnableGrass();

				ImGui::SliderFloat(lang[LangID::GrassFactor], &settings.GetGrassFactor(), 0.1f, 100.0f);

				ImGui::SliderFloat(lang[LangID::MinGrass], &settings.GetMinGrass(), 100.0f, settings.GetMaxGrass());
				ImGui::SliderFloat(lang[LangID::MaxGrass], &settings.GetMaxGrass(), settings.GetMinGrass(), 30000.0f);
			}

			if (ImGui::CollapsingHeader(lang[LangID::GodRaysHdr])) {

				if (ImGui::Checkbox(lang[LangID::EnableGodRays], &settings.GetGodRaysEnabled()))
					shadow.EnableGodRays();

				bool ret = ImGui::SliderInt(lang[LangID::GrQuality], &settings.GetGrQuality(), 0, 3);
				ret |= ImGui::SliderInt(lang[LangID::GrGrid], &settings.GetGrGrid(), 0, 256);
				ret |= ImGui::SliderFloat(lang[LangID::GrScale], &settings.GetGrScale(), 0.1f, 2.0f);
				ret |= ImGui::SliderInt(lang[LangID::GrCascade], &settings.GetGrCascade(), 1, 64);

				if (ret)
					shadow.SetGodRays();
			}

			if (ImGui::CollapsingHeader(lang[LangID::OthersHdr])) {

				ImGui::Text(lang[LangID::Hotkey]);

				ImGui::SameLine();

				if (ImGui::Button(settings.GetHotkey().c_str())) {

					settings.SetNameKey(true);

					waitForKey = true;
				}

				ImGui::NewLine();

				ImGui::Checkbox(lang[LangID::EnableLog], &settings.IsEnabledLog());

				ImGui::Checkbox(lang[LangID::Transparent], &settings.IsTransparent());

				if (ImGui::Checkbox(lang[LangID::StopInMenu], &settings.IsPauseInMenu())) {

					if (!settings.IsPauseInMenu())
						shadow.Run();
				}
			}

			if (ImGui::CollapsingHeader(lang[LangID::CreditsHdr])) {

				ImGui::Text("ImGui by ocornut");
				ImGui::Text("SimpleIni by brofield");
				ImGui::Text("Jsoncpp");
				ImGui::Text("Special thanks to my patrons MasonJ and Brac for their support");
				ImGui::Text("F4SE team for creating the script extender");
				ImGui::Text("Bethesda for creating Fallout 4");
				ImGui::Text("Fudgyduff for CommonLibF4");
				ImGui::Text("Thanks to all those who with their ideas and suggestions\nhelp me to improve this mod.");
			}

			if (ImGui::Button(lang[LangID::Load])) {

				settings.ReadSettings();

				shadow.ReloadStuff();
			}

			ImGui::SameLine();

			if (ImGui::Button(lang[LangID::Save])) {

				settings.WriteSettings();

				json_preset::Preset::GetInstance().InitDefaultValues();
			}

			ImGui::SameLine();

			if (ImGui::Button(lang[LangID::Default])) {

				settings.SetDefault();

				shadow.ReloadStuff();
			}

			ImGui::End();
		}

		ImGui::Render();
	}

	void Menu::Present() noexcept
	{
		if (!Settings::Ini::GetInstance().IsEnabledLog() && !isShow)
			return;

		auto drawData = ImGui::GetDrawData();

		if (drawData)
			ImGui_ImplDX11_RenderDrawData(drawData);
	}

	Menu Menu::instance;
}