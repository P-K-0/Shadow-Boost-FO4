#pragma once

#include "pch.h"

namespace Translation {

	static const char* TranslationsDir = "Data\\F4SE\\Plugins\\Translations\\";
	static const char* TranslationFile = "Language.txt";

	static const char* DefaultLang = "en";

	enum class LangID : std::uint32_t {

		CaptionWnd = 0,
		StatusHdr,
		AvgMs,
		DynValue,
		ShadowDist,
		ObjectDist,
		ItemsDist,
		ActorsDist,
		GrassDist,
		BlockLvl2,
		BlockLvl1,
		BlockLvl0,
		GodraysQuality,
		GodraysGrid,
		GodraysScale,
		GodraysCascade,
		CurrentLocationHdr,
		IsInterior,
		HasPreset,
		InteriorId,
		ExteriorId,
		InteriorName,
		ExteriorName,
		AddToPreset,
		RemoveFromPreset,
		SavePreset,
		HelpHdr,
		HelpText,
		MainHdr,
		FpsTarget,
		FpsDelay,
		MsTolerance,
		ShadowHdr,
		EnableShadow,
		ShadowFactor,
		MinShadow,
		MaxShadow,
		LodHdr,
		EnableLod,
		LodFactor,
		MinObjects,
		MaxObjects,
		MinItems,
		MaxItems,
		MinActors,
		MaxActors,
		TerrainManagerHdr,
		EnableBlockLevels,
		BlockFactor,
		MinBlockLevel2,
		MaxBlockLevel2,
		MinBlockLevel1,
		MaxBlockLevel1,
		MinBlockLevel0,
		MaxBlockLevel0,
		GrassHdr,
		EnableGrass,
		GrassFactor,
		MinGrass,
		MaxGrass,
		GodRaysHdr,
		EnableGodRays,
		GrQuality,
		GrGrid,
		GrScale,
		GrCascade,
		OthersHdr,
		HotKey,
		EnableLog,
		Transparent,
		StopInMenu,
		CreditsHdr,
		Load,
		Save,
		Default,
		WaitForNewKey,
		Total
	};

	enum class LangGlyph : std::uint32_t {

		Default,
		Korean,
		Japanese,
		ChineseFull,
		ChineseSimplifiedCommon,
		Cyrillic,
		Thai,
		Vietnamese
	};

	class Language {

	public:

		static Language& GetInstance() { return instance; }

		void Load() noexcept;

		[[nodiscard]] const char* operator[](const LangID id) noexcept { return vec[static_cast<std::uint32_t>(id)].c_str(); }

		[[nodiscard]] const ImWchar* SelectGlyph(ImGuiIO& io, const LangGlyph& glyph) noexcept;
		[[nodiscard]] void SetGlyph(ImGuiIO& io) noexcept;

	private:

		Language() {}
		~Language() {}

		Language(const Language&) = delete;
		Language(Language&&) = delete;

		Language& operator=(const Language&) = delete;
		Language& operator=(Language&&) = delete;

		[[nodiscard]] std::wstring GetFallout4ini() noexcept;
		[[nodiscard]] const std::string& GetFallout4lang() noexcept;

		std::string sLang{ DefaultLang };

		std::vector<std::string> vec{
			"Shadow Boost FO4 by PK0",
			"Status",
			"Avg ms frames : %.3f",
			"Dynamic Value : %.3f",
			"Shadow Dist : %.3f",
			"Objects Dist : %.3f",
			"Items Dist : %.3f",
			"Actors Dist : %.3f",
			"Grass Dist : %.3f",
			"BlockLevel 2 Dist : %.3f",
			"BlockLevel 1 Dist : %.3f",
			"BlockLevel 0 Dist : %.3f",
			"God rays Quality : %i",
			"God rays Grid : %i",
			"God rays Scale : %.3f",
			"God rays Cascade : %i",
			"Current Location",
			"Is Interior : %i",
			"Has Preset : %i",
			"Interior Id : %.8X",
			"Exterior Id : %.8X",
			"Interior name : %s",
			"Exterior name : %s",
			"Add to Preset",
			"Remove from Preset",
			"Save Preset",
			"Help",
			"It is highly recommended to configure all options,\nbecause the default options may not be compatible\nwith all systems, so better check the ini files of\nthe game to get a precise idea of which settings\nneed to be changed.",
			"Main",
			"Fps Target",
			"Fps Delay",
			"Ms Tolerance",
			"Shadow",
			"Enable Shadow",
			"Shadow Factor",
			"Min Shadow",
			"Max Shadow",
			"Lod",
			"Enable Lod",
			"Lod Factor",
			"Min Objects",
			"Max Objects",
			"Min Items",
			"Max Items",
			"Min Actors",
			"Max Actors",
			"Terrain Manager",
			"Enable Block Levels",
			"Block Factor",
			"Min Block Level 2",
			"Max Block Level 2",
			"Min Block Level 1",
			"Max Block Level 1", 
			"Min Block Level 0",
			"Max Block Level 0", 
			"Grass",
			"Enable Grass",
			"Grass Factor",
			"Min Grass",
			"Max Grass",
			"God Rays",
			"Enable God Rays",
			"Quality",
			"Grid",
			"Scale",
			"Cascade",
			"Others",
			"Hotkey : ",
			"Enable Log (if activated it means that\nthis window always remains active)",
			"Make it transparent",
			"Pause when a menu is open",
			"Thanks and credits",
			"Load",
			"Save",
			"Default",
			"Waiting for a new key",
			"unused"
		};

		const std::vector<std::pair<LangGlyph, std::string>> vecLang {
			{ LangGlyph::Default, "en" },
			{ LangGlyph::Default, "de" },
			{ LangGlyph::Default, "es" },
			{ LangGlyph::Default, "esmx" },
			{ LangGlyph::Default, "fr" },
			{ LangGlyph::Default, "it" },
			{ LangGlyph::Default, "ptbr"},
			{ LangGlyph::Japanese, "ja" },
			{ LangGlyph::Korean, "ko" },
			{ LangGlyph::ChineseSimplifiedCommon, "cn" },
			{ LangGlyph::Cyrillic, "ru" },
			{ LangGlyph::Cyrillic, "pl"}
		};

		static Language instance;
	};
}
