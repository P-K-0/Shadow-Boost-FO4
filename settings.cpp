
#include "settings.h"

#include "f4se_plugin.h"
#include "std_libraries.h"
#include "translation.h"
#include "SimpleIni.h"

namespace Settings {

	enum class Section {

		Main,
		Lod,
		Grass,
		TerrainManager,
		TerrainManagerLevel1,
		TerrainManagerLevel2,
		TerrainManagerLevel3,
		GodRays,
		Log
	};

	const std::vector<std::string> Sections{
		"Main",
		"Lod",
		"Grass",
		"TerrainManager",
		"TerrainManager:Level1",
		"TerrainManager:Level2",
		"TerrainManager:Level3",
		"GodRays",
		"Log"
	};

	template<typename T, typename Func = std::function<T (const std::string&)>>
	inline T GetValue(CSimpleIni& ini, const Section& section, const std::string& key, T def, Func fn)
	{
		return fn(ini.GetValue(Sections[static_cast<int>(section)].c_str(), key.c_str(), std::to_string(def).c_str()));
	}

	template<typename T>
	inline SI_Error SetValue(CSimpleIni& ini, const Section& section, const std::string& key, T value)
	{
		return ini.SetValue(Sections[static_cast<int>(section)].c_str(), key.c_str(), std::to_string(value).c_str());
	}

	inline SI_Error SetValue(CSimpleIni& ini, const Section& section, const std::string& key, std::string& value)
	{
		return ini.SetValue(Sections[static_cast<int>(section)].c_str(), key.c_str(), value.c_str());
	}

	inline float stof(const std::string& str) { return std::stof(str); }

	inline int stoi(const std::string& str) { return std::stoi(str); }

	inline bool stob(const std::string& str) { return std::stoi(str); }

	inline std::uint32_t stou(const std::string& str) { return std::stoul(str); }

	inline std::uint16_t stoi16(const std::string& str) { return static_cast<std::uint16_t>(stou(str)); }

	inline float GetValue(CSimpleIni& ini, const Section& section, const std::string& key, float def)
	{
		return GetValue<>(ini, section, key, def, stof);
	}

	inline bool GetValue(CSimpleIni& ini, const Section& section, const std::string& key, bool def)
	{
		return GetValue<>(ini, section, key, def, stob);
	}

	inline int GetValue(CSimpleIni& ini, const Section& section, const std::string& key, int def)
	{
		return GetValue<>(ini, section, key, def, stoi);
	}

	inline std::uint32_t GetValue(CSimpleIni& ini, const Section& section, const std::string& key, std::uint32_t def)
	{
		return GetValue<>(ini, section, key, def, stou);
	}

	inline std::uint16_t GetValue(CSimpleIni& ini, const Section& section, const std::string& key, std::uint16_t def)
	{
		return GetValue<>(ini, section, key, def, stoi16);
	}

	inline std::string GetValue(CSimpleIni& ini, const Section& section, const std::string& key, const char* def)
	{
		return ini.GetValue(Sections[static_cast<int>(section)].c_str(), key.c_str(), def);
	}

	void Ini::ParseString(const std::string& str, std::vector<std::string>& vec) noexcept
	{
		vec.clear();

		std::string tmp{ str };

		std::size_t found{};

		while (!tmp.empty()) {

			found = tmp.find(";");

			if (found != std::string::npos) {

				vec.push_back(tmp.substr(0, found));
				tmp = tmp.substr(found + 1);
			}
			else {

				vec.push_back(tmp);
				tmp = "";
			}
		}

		if (!vec.empty())
			std::sort(vec.begin(), vec.end());

		//for (auto& v : vec)
		//	logger::info("{}", v.c_str());
	}

	const bool Ini::IsMenuOpen() noexcept
	{
		auto ui = RE::UI::GetSingleton();

		if (!ui)
			return false;

		bool ret{};
		
		std::for_each(vBlacklistMenu.begin(), vBlacklistMenu.end(), [&](const std::string& menu) { ret |= ui->GetMenuOpen(menu.c_str()); });

		return ret;
	}

	void Ini::SetDefault(bool isEditor) noexcept
	{
		if (!isEditor) {

			iKey = DefaultKey;
			iExtendedKey = DefaultExtendedKey;

			iKeyStop = DefaultKeyStop;
			iExtendedKeyStop = DefaultExtendedKeyStop;

			fFontScale = DefaultFontScale;

			bLog = DefaultLog;

			bTransparent = DefaultTransparent;
			bPauseInMenu = DefaultPauseInMenu;

			sBlackListMenu = DefaultBlackListMenu;

			mnuStyle = MenuStyle::Dark;
		}

		m_values.fFpsTarget = DefaultFpsTarget;
		m_values.fFpsDelay = DefaultFpsDelay;
		m_values.fMsTolerance = DefaultMsTolerance;

		m_values.grQuality = DefaultGrQuality;
		m_values.grGrid = DefaultGrGrid;
		m_values.grScale = DefaultGrScale;
		m_values.grCascade = DefaultGrCascade;

		m_values.bShadowEnable = DefaultShadow;
		m_values.bGrassEnable = DefaultGrass;
		m_values.bLodEnable = DefaultLod;
		m_values.bBlockEnable = DefaultBlock;
		m_values.bGodRays = DefaultGodRays;

		values.fMinDistance = DefaultMinDistance;
		values.fMaxDistance = DefaultMaxDistance;

		values.fShadowFactor = DefaultShadowFactor;

		values.fMinDistObjects = DefaultMinDistObjects;
		values.fMaxDistObjects = DefaultMaxDistObjects;

		values.fMinDistItems = DefaultMinDistItems;
		values.fMaxDistItems = DefaultMaxDistItems;

		values.fMinDistActors = DefaultMinDistActors;
		values.fMaxDistActors = DefaultMaxDistActors;

		values.fLodFactor = DefaultLodFactor;

		values.fMinDistGrass = DefaultMinDistGrass;
		values.fMaxDistGrass = DefaultMaxDistGrass;

		values.fGrassFactor = DefaultGrassFactor;

		for (int index = 0; index < MaxBlockLevels; index++) {
		
			auto& blocklevel = values.blockLevel[index];

			blocklevel.fDistBlockLevel2 = DistBlockLevel2[index];
			blocklevel.fDistBlockLevel1 = DistBlockLevel1[index];
			blocklevel.fDistBlockLevel0 = DistBlockLevel0[index];
		}

		ParseString(sBlackListMenu, vBlacklistMenu);
	}

	void Ini::ReadSettings(bool isEditor) noexcept
	{
		dirLog = "Data\\F4SE\\Plugins\\";
		dirLog += Version::Project;
		dirLog += ".ini";

		SetDefault(isEditor);

		CSimpleIni ini;

		ini.SetUnicode();
		SI_Error error = ini.LoadFile(dirLog.c_str());

		if (error != 0)
			return;

		if (!isEditor) {

			iKey = GetValue(ini, Section::Main, "iKey", DefaultKey);
			iExtendedKey = GetValue(ini, Section::Main, "iExtendedKey", DefaultExtendedKey);

			iKeyStop = GetValue(ini, Section::Main, "iKeyStop", DefaultKeyStop);
			iExtendedKeyStop = GetValue(ini, Section::Main, "iExtendedKeyStop", DefaultExtendedKeyStop);

			fFontScale = GetValue(ini, Section::Main, "fFontScale", DefaultFontScale);

			bLog = GetValue(ini, Section::Log, "bLog", DefaultLog);
			bTransparent = GetValue(ini, Section::Log, "bTransparent", DefaultTransparent);
			bPauseInMenu = GetValue(ini, Section::Log, "bPauseInMenu", DefaultPauseInMenu);

			sBlackListMenu = GetValue(ini, Section::Main, "sBlackListMenu", DefaultBlackListMenu);

			mnuStyle = static_cast<MenuStyle>(GetValue(ini, Section::Main, "Style", static_cast<int>(MenuStyle::Dark)));
		}

		m_values.fFpsTarget = GetValue(ini, Section::Main, "fFpsTarget", DefaultFpsTarget);
		m_values.fFpsDelay = GetValue(ini, Section::Main, "fFpsDelay", DefaultFpsDelay);
		m_values.fMsTolerance = GetValue(ini, Section::Main, "fMsTolerance", DefaultMsTolerance);
		m_values.bShadowEnable = GetValue(ini, Section::Main, "bEnable", DefaultShadow);

		m_values.grQuality = GetValue(ini, Section::GodRays, "iQuality", DefaultGrQuality);
		m_values.grGrid = GetValue(ini, Section::GodRays, "iGrid", DefaultGrGrid);
		m_values.grScale = GetValue(ini, Section::GodRays, "fScale", DefaultGrScale);
		m_values.grCascade = GetValue(ini, Section::GodRays, "iCascade", DefaultGrCascade);
		m_values.bGodRays = GetValue(ini, Section::GodRays, "bEnable", DefaultGodRays);

		values.fMinDistance = GetValue(ini, Section::Main, "fMinDistance", DefaultMinDistance);
		values.fMaxDistance = GetValue(ini, Section::Main, "fMaxDistance", DefaultMaxDistance);

		values.fShadowFactor = GetValue(ini, Section::Main, "fDynamicValueFactor", DefaultShadowFactor);

		values.fMinDistObjects = GetValue(ini, Section::Lod, "fLODFadeOutMultObjectsMin", DefaultMinDistObjects);
		values.fMaxDistObjects = GetValue(ini, Section::Lod, "fLODFadeOutMultObjectsMax", DefaultMaxDistObjects);

		values.fMinDistItems = GetValue(ini, Section::Lod, "fLODFadeOutMultItemsMin", DefaultMinDistItems);
		values.fMaxDistItems = GetValue(ini, Section::Lod, "fLODFadeOutMultItemsMax", DefaultMaxDistItems);

		values.fMinDistActors = GetValue(ini, Section::Lod, "fLODFadeOutMultActorsMin", DefaultMinDistActors);
		values.fMaxDistActors = GetValue(ini, Section::Lod, "fLODFadeOutMultActorsMax", DefaultMaxDistActors);

		values.fLodFactor = GetValue(ini, Section::Lod, "fDynamicValueFactor", DefaultLodFactor);

		m_values.bLodEnable = GetValue(ini, Section::Lod, "bEnable", DefaultLod);

		values.fMinDistGrass = GetValue(ini, Section::Grass, "fGrassStartFadeDistanceMin", DefaultMinDistGrass);
		values.fMaxDistGrass = GetValue(ini, Section::Grass, "fGrassStartFadeDistanceMax", DefaultMaxDistGrass);
		
		values.fGrassFactor = GetValue(ini, Section::Grass, "fDynamicValueFactor", DefaultGrassFactor);

		m_values.bGrassEnable = GetValue(ini, Section::Grass, "bEnable", DefaultGrass);

		for (int index = 0; index < MaxBlockLevels; index++) {

			Section section = static_cast<Section>(static_cast<int>(Section::TerrainManager) + index);

			auto& blocklevel = values.blockLevel[index];

			blocklevel.fDistBlockLevel2 = GetValue(ini, section, "fBlockLevel2Distance", DistBlockLevel2[index]);
			blocklevel.fDistBlockLevel1 = GetValue(ini, section, "fBlockLevel1Distance", DistBlockLevel1[index]);
			blocklevel.fDistBlockLevel0 = GetValue(ini, section, "fBlockLevel0Distance", DistBlockLevel0[index]);
		}

		m_values.bBlockEnable = GetValue(ini, Section::TerrainManager, "bEnable", DefaultBlock);

		ParseString(sBlackListMenu, vBlacklistMenu);
	}

	bool Ini::WriteSettings(bool isEditor) noexcept
	{
		CSimpleIni ini;

		ini.SetUnicode();
		ini.LoadFile(dirLog.c_str());

		//SI_Error error = ini.LoadFile(dirLog.c_str());
		//if (error != 0)
		//	return false;

		if (!isEditor) {

			SetValue(ini, Section::Main, "iKey", iKey);
			SetValue(ini, Section::Main, "iExtendedKey", iExtendedKey);

			SetValue(ini, Section::Main, "iKeyStop", iKeyStop);
			SetValue(ini, Section::Main, "iExtendedKeyStop", iExtendedKeyStop);

			SetValue(ini, Section::Main, "fFontScale", fFontScale);

			SetValue(ini, Section::Log, "bLog", bLog);
			SetValue(ini, Section::Log, "bTransparent", bTransparent);
			SetValue(ini, Section::Log, "bPauseInMenu", bPauseInMenu);

			SetValue(ini, Section::Main, "Style", static_cast<int>(mnuStyle));
		}

		SetValue(ini, Section::Main, "fFpsTarget", m_values.fFpsTarget);
		SetValue(ini, Section::Main, "fFpsDelay", m_values.fFpsDelay);
		SetValue(ini, Section::Main, "fMsTolerance", m_values.fMsTolerance);

		SetValue(ini, Section::Main, "fMinDistance", values.fMinDistance);
		SetValue(ini, Section::Main, "fMaxDistance", values.fMaxDistance);

		SetValue(ini, Section::Main, "sBlackListMenu", sBlackListMenu);

		SetValue(ini, Section::Main, "fDynamicValueFactor", values.fShadowFactor);

		SetValue(ini, Section::Main, "bEnable", m_values.bShadowEnable);

		SetValue(ini, Section::GodRays, "iQuality", m_values.grQuality);
		SetValue(ini, Section::GodRays, "iGrid", m_values.grGrid);
		SetValue(ini, Section::GodRays, "fScale", m_values.grScale);
		SetValue(ini, Section::GodRays, "iCascade", m_values.grCascade);
		SetValue(ini, Section::GodRays, "bEnable", m_values.bGodRays);

		SetValue(ini, Section::Lod, "fLODFadeOutMultObjectsMin", values.fMinDistObjects);
		SetValue(ini, Section::Lod, "fLODFadeOutMultObjectsMax", values.fMaxDistObjects);

		SetValue(ini, Section::Lod, "fLODFadeOutMultItemsMin", values.fMinDistItems);
		SetValue(ini, Section::Lod, "fLODFadeOutMultItemsMax", values.fMaxDistItems);

		SetValue(ini, Section::Lod, "fLODFadeOutMultActorsMin", values.fMinDistActors);
		SetValue(ini, Section::Lod, "fLODFadeOutMultActorsMax", values.fMaxDistActors);

		SetValue(ini, Section::Lod, "fDynamicValueFactor", values.fLodFactor);

		SetValue(ini, Section::Lod, "bEnable", m_values.bLodEnable);

		SetValue(ini, Section::Grass, "fGrassStartFadeDistanceMin", values.fMinDistGrass);
		SetValue(ini, Section::Grass, "fGrassStartFadeDistanceMax", values.fMaxDistGrass);

		SetValue(ini, Section::Grass, "fDynamicValueFactor", values.fGrassFactor);

		SetValue(ini, Section::Grass, "bEnable", m_values.bGrassEnable);

		for (int index = 0; index < MaxBlockLevels; index++) {

			Section section = static_cast<Section>(static_cast<int>(Section::TerrainManager) + index);

			auto& blocklevel = values.blockLevel[index];

			SetValue(ini, section, "fBlockLevel2Distance", blocklevel.fDistBlockLevel2);
			SetValue(ini, section, "fBlockLevel1Distance", blocklevel.fDistBlockLevel1);
			SetValue(ini, section, "fBlockLevel0Distance", blocklevel.fDistBlockLevel0);
		}

		SetValue(ini, Section::TerrainManager, "bEnable", m_values.bBlockEnable);

		return ini.SaveFile(dirLog.c_str()) == 0;		
	}

	Ini Ini::instance;
}
