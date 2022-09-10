
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
		GodRays,
		Log
	};

	const std::vector<std::string> Sections{
		"Main",
		"Lod",
		"Grass",
		"TerrainManager",
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

	inline float stof(const std::string& str) { return std::stof(str); }

	inline int stoi(const std::string& str) { return std::stoi(str); }

	inline bool stob(const std::string& str) { return std::stoi(str); }

	inline std::uint32_t stou(const std::string& str) { return std::stoul(str); }

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

	inline std::string GetValue(CSimpleIni& ini, const Section& section, const std::string& key, const char* def)
	{
		return ini.GetValue(Sections[static_cast<int>(section)].c_str(), key.c_str(), def);
	}

	void Ini::SetNameKey(bool wait) noexcept
	{
		if (wait) {

			nameKey = Translation::Language::GetInstance()[Translation::LangID::WaitForNewKey];

			return;
		}

		char nKey[255]{};

		std::uint32_t scanCode = MapVirtualKey(iKey, MAPVK_VK_TO_VSC);
		scanCode <<= 16;

		GetKeyNameText(scanCode, nKey, sizeof(nKey));

		nameKey = nKey;
	}

	void Ini::ParseString(const std::string& str, std::vector<std::string>& vec) noexcept
	{
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
	}

	void Ini::SetDefault() noexcept
	{
		iKey = DefaultKey;

		m_values.fFpsTarget = DefaultFpsTarget;
		m_values.fFpsDelay = DefaultFpsDelay;
		m_values.fMsTolerance = DefaultMsTolerance;

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

		values.fMinDistBlockLevel2 = DefaultMinDistBlockLevel2;
		values.fMaxDistBlockLevel2 = DefaultMaxDistBlockLevel2;

		values.fMinDistBlockLevel1 = DefaultMinDistBlockLevel1;
		values.fMaxDistBlockLevel1 = DefaultMaxDistBlockLevel1;

		values.fMinDistBlockLevel0 = DefaultMinDistBlockLevel0;
		values.fMaxDistBlockLevel0 = DefaultMaxDistBlockLevel0;

		values.fBlockFactor = DefaultBlockLevelFactor;

		m_values.grQuality = DefaultGrQuality;
		m_values.grGrid = DefaultGrGrid;
		m_values.grScale = DefaultGrScale;
		m_values.grCascade = DefaultGrCascade;

		m_values.bShadowEnable = DefaultShadow;
		m_values.bGrassEnable = DefaultGrass;
		m_values.bLodEnable = DefaultLod;
		m_values.bBlockEnable = DefaultBlock;
		m_values.bGodRays = DefaultGodRays;

		bLog = DefaultLog;
		bTransparent = DefaultTransparent;
		bPauseInMenu = DefaultPauseInMenu;

		SetNameKey();
	}

	void Ini::ReadSettings() noexcept
	{
		dirLog = "Data\\F4SE\\Plugins\\";
		dirLog += Version::Project;
		dirLog += ".ini";

		SetDefault();

		CSimpleIni ini;

		ini.SetUnicode();
		SI_Error error = ini.LoadFile(dirLog.c_str());

		if (error != 0)
			return;

		iKey = GetValue(ini, Section::Main, "iKey", DefaultKey);

		m_values.fFpsTarget = GetValue(ini, Section::Main, "fFpsTarget", DefaultFpsTarget);
		m_values.fFpsDelay = GetValue(ini, Section::Main, "fFpsDelay", DefaultFpsDelay);
		m_values.fMsTolerance = GetValue(ini, Section::Main, "fMsTolerance", DefaultMsTolerance);

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

		values.fMinDistGrass = GetValue(ini, Section::Grass, "fGrassStartFadeDistanceMin", DefaultMinDistGrass);
		values.fMaxDistGrass = GetValue(ini, Section::Grass, "fGrassStartFadeDistanceMax", DefaultMaxDistGrass);
		
		values.fGrassFactor = GetValue(ini, Section::Grass, "fDynamicValueFactor", DefaultGrassFactor);

		values.fMinDistBlockLevel2 = GetValue(ini, Section::TerrainManager, "fBlockLevel2DistanceMin", DefaultMinDistBlockLevel2);
		values.fMaxDistBlockLevel2 = GetValue(ini, Section::TerrainManager, "fBlockLevel2DistanceMax", DefaultMaxDistBlockLevel2);

		values.fMinDistBlockLevel1 = GetValue(ini, Section::TerrainManager, "fBlockLevel1DistanceMin", DefaultMinDistBlockLevel1);
		values.fMaxDistBlockLevel1 = GetValue(ini, Section::TerrainManager, "fBlockLevel1DistanceMax", DefaultMaxDistBlockLevel1);

		values.fMinDistBlockLevel0 = GetValue(ini, Section::TerrainManager, "fBlockLevel0DistanceMin", DefaultMinDistBlockLevel0);
		values.fMaxDistBlockLevel0 = GetValue(ini, Section::TerrainManager, "fBlockLevel0DistanceMax", DefaultMaxDistBlockLevel0);

		values.fBlockFactor = GetValue(ini, Section::TerrainManager, "fDynamicValueFactor", DefaultBlockLevelFactor);

		m_values.grQuality = GetValue(ini, Section::GodRays, "iQuality", DefaultGrQuality);
		m_values.grGrid = GetValue(ini, Section::GodRays, "iGrid", DefaultGrGrid);
		m_values.grScale = GetValue(ini, Section::GodRays, "fScale", DefaultGrScale);
		m_values.grCascade = GetValue(ini, Section::GodRays, "iCascade", DefaultGrCascade);

		m_values.bShadowEnable = GetValue(ini, Section::Main, "bEnable", DefaultShadow);
		m_values.bGrassEnable = GetValue(ini, Section::Grass, "bEnable", DefaultGrass);
		m_values.bLodEnable = GetValue(ini, Section::Lod, "bEnable", DefaultLod);
		m_values.bBlockEnable = GetValue(ini, Section::TerrainManager, "bEnable", DefaultBlock);
		m_values.bGodRays = GetValue(ini, Section::GodRays, "bEnable", DefaultGodRays);

		bLog = GetValue(ini, Section::Log, "bLog", DefaultLog);
		bTransparent = GetValue(ini, Section::Log, "bTransparent", DefaultTransparent);
		bPauseInMenu = GetValue(ini, Section::Log, "bPauseInMenu", DefaultPauseInMenu);

		ParseString(GetValue(ini, Section::Main, "sBlackListMenu", DefaultBlackListMenus), blacklistMenus);

		SetNameKey();
	}

	bool Ini::WriteSettings() noexcept
	{
		CSimpleIni ini;

		ini.SetUnicode();
		SI_Error error = ini.LoadFile(dirLog.c_str());

		if (error != 0)
			return false;

		SetValue(ini, Section::Main, "iKey", iKey);

		SetValue(ini, Section::Main, "fFpsTarget", m_values.fFpsTarget);
		SetValue(ini, Section::Main, "fFpsDelay", m_values.fFpsDelay);
		SetValue(ini, Section::Main, "fMsTolerance", m_values.fMsTolerance);

		SetValue(ini, Section::Main, "fMinDistance", values.fMinDistance);
		SetValue(ini, Section::Main, "fMaxDistance", values.fMaxDistance);

		SetValue(ini, Section::Main, "fDynamicValueFactor", values.fShadowFactor);

		SetValue(ini, Section::Lod, "fLODFadeOutMultObjectsMin", values.fMinDistObjects);
		SetValue(ini, Section::Lod, "fLODFadeOutMultObjectsMax", values.fMaxDistObjects);

		SetValue(ini, Section::Lod, "fLODFadeOutMultItemsMin", values.fMinDistItems);
		SetValue(ini, Section::Lod, "fLODFadeOutMultItemsMax", values.fMaxDistItems);

		SetValue(ini, Section::Lod, "fLODFadeOutMultActorsMin", values.fMinDistActors);
		SetValue(ini, Section::Lod, "fLODFadeOutMultActorsMax", values.fMaxDistActors);

		SetValue(ini, Section::Lod, "fDynamicValueFactor", values.fLodFactor);

		SetValue(ini, Section::Grass, "fGrassStartFadeDistanceMin", values.fMinDistGrass);
		SetValue(ini, Section::Grass, "fGrassStartFadeDistanceMax", values.fMaxDistGrass);

		SetValue(ini, Section::Grass, "fDynamicValueFactor", values.fGrassFactor);

		SetValue(ini, Section::TerrainManager, "fBlockLevel2DistanceMin", values.fMinDistBlockLevel2);
		SetValue(ini, Section::TerrainManager, "fBlockLevel2DistanceMax", values.fMaxDistBlockLevel2);

		SetValue(ini, Section::TerrainManager, "fBlockLevel1DistanceMin", values.fMinDistBlockLevel1);
		SetValue(ini, Section::TerrainManager, "fBlockLevel1DistanceMax", values.fMaxDistBlockLevel1);

		SetValue(ini, Section::TerrainManager, "fBlockLevel0DistanceMin", values.fMinDistBlockLevel0);
		SetValue(ini, Section::TerrainManager, "fBlockLevel0DistanceMax", values.fMaxDistBlockLevel0);

		SetValue(ini, Section::TerrainManager, "fDynamicValueFactor", values.fBlockFactor);

		SetValue(ini, Section::GodRays, "iQuality", m_values.grQuality);
		SetValue(ini, Section::GodRays, "iGrid", m_values.grGrid);
		SetValue(ini, Section::GodRays, "fScale", m_values.grScale);
		SetValue(ini, Section::GodRays, "iCascade", m_values.grCascade);

		SetValue(ini, Section::Main, "bEnable", m_values.bShadowEnable);
		SetValue(ini, Section::Lod, "bEnable", m_values.bLodEnable);
		SetValue(ini, Section::Grass, "bEnable", m_values.bGrassEnable);
		SetValue(ini, Section::TerrainManager, "bEnable", m_values.bBlockEnable);
		SetValue(ini, Section::GodRays, "bEnable", m_values.bGodRays);

		SetValue(ini, Section::Log, "bLog", bLog);
		SetValue(ini, Section::Log, "bTransparent", bTransparent);
		SetValue(ini, Section::Log, "bPauseInMenu", bPauseInMenu);

		return ini.SaveFile(dirLog.c_str()) == 0;		
	}

	Ini Ini::instance;
}
