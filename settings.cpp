
#include "settings.h"

namespace Settings {

	enum class Section {

		Main,
		Lod,
		Grass,
		Light,
		TerrainManager,
		GodRays,
		Log
	};

	const std::vector<std::string> Sections{
		"Main",
		"Lod",
		"Grass",
		"Light",
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

	void Ini::ReadSettings() noexcept
	{
		dirLog = "Data\\F4SE\\Plugins\\";
		dirLog += f4se::NamePlugin;
		dirLog += ".ini";

		CSimpleIni ini;

		ini.SetUnicode();
		SI_Error error = ini.LoadFile(dirLog.c_str());

		if (error != 0)
			return;

		iKey = GetValue(ini, Section::Main, "iKey", VK_F12);

		fFpsTarget = GetValue(ini, Section::Main, "fFpsTarget", DefaultFpsTarget);
		fFpsDelay = GetValue(ini, Section::Main, "fFpsDelay", DefaultFpsDelay);

		fMinDistance = GetValue(ini, Section::Main, "fMinDistance", DefaultMinDistance);
		fMaxDistance = GetValue(ini, Section::Main, "fMaxDistance", DefaultMaxDistance);

		fShadowFactor = GetValue(ini, Section::Main, "fDynamicValueFactor", DefaultShadowFactor);

		fMinDistObjects = GetValue(ini, Section::Lod, "fLODFadeOutMultObjectsMin", DefaultMinDistObjects);
		fMaxDistObjects = GetValue(ini, Section::Lod, "fLODFadeOutMultObjectsMax", DefaultMaxDistObjects);

		fMinDistItems = GetValue(ini, Section::Lod, "fLODFadeOutMultItemsMin", DefaultMinDistItems);
		fMaxDistItems = GetValue(ini, Section::Lod, "fLODFadeOutMultItemsMax", DefaultMaxDistItems);

		fMinDistActors = GetValue(ini, Section::Lod, "fLODFadeOutMultActorsMin", DefaultMinDistActors);
		fMaxDistActors = GetValue(ini, Section::Lod, "fLODFadeOutMultActorsMax", DefaultMaxDistActors);

		fLodFactor = GetValue(ini, Section::Lod, "fDynamicValueFactor", DefaultLodFactor);

		fMinDistGrass = GetValue(ini, Section::Grass, "fGrassStartFadeDistanceMin", DefaultMinDistGrass);
		fMaxDistGrass = GetValue(ini, Section::Grass, "fGrassStartFadeDistanceMax", DefaultMaxDistGrass);
		
		fGrassFactor = GetValue(ini, Section::Grass, "fDynamicValueFactor", DefaultGrassFactor);

		fMinDistBlockLevel2 = GetValue(ini, Section::TerrainManager, "fBlockLevel2DistanceMin", DefaultMinDistBlockLevel2);
		fMaxDistBlockLevel2 = GetValue(ini, Section::TerrainManager, "fBlockLevel2DistanceMax", DefaultMaxDistBlockLevel1);

		fMinDistBlockLevel1 = GetValue(ini, Section::TerrainManager, "fBlockLevel1DistanceMin", DefaultMinDistBlockLevel1);
		fMaxDistBlockLevel1 = GetValue(ini, Section::TerrainManager, "fBlockLevel1DistanceMax", DefaultMaxDistBlockLevel1);

		fMinDistBlockLevel0 = GetValue(ini, Section::TerrainManager, "fBlockLevel0DistanceMin", DefaultMinDistBlockLevel0);
		fMaxDistBlockLevel0 = GetValue(ini, Section::TerrainManager, "fBlockLevel0DistanceMax", DefaultMaxDistBlockLevel0);

		fBlockFactor = GetValue(ini, Section::TerrainManager, "fDynamicValueFactor", DefaultBlockLevelFactor);

		grQuality = GetValue(ini, Section::GodRays, "iQuality", DefaultGrQuality);
		grGrid = GetValue(ini, Section::GodRays, "iGrid", DefaultGrGrid);
		grScale = GetValue(ini, Section::GodRays, "fScale", DefaultGrScale);
		grCascade = GetValue(ini, Section::GodRays, "iCascade", DefaultGrCascade);

		bLog = GetValue(ini, Section::Log, "bLog", DefaultLog);
	}

	void Ini::WriteSettings() noexcept
	{
		CSimpleIni ini;

		ini.SetUnicode();
		SI_Error error = ini.LoadFile(dirLog.c_str());

		if (error != 0)
			return;

		SetValue(ini, Section::Main, "iKey", iKey);

		SetValue(ini, Section::Main, "fFpsTarget", fFpsTarget);
		SetValue(ini, Section::Main, "fFpsDelay", fFpsDelay);

		SetValue(ini, Section::Main, "fMinDistance", fMinDistance);
		SetValue(ini, Section::Main, "fMaxDistance", fMaxDistance);

		SetValue(ini, Section::Main, "fDynamicValueFactor", fShadowFactor);

		SetValue(ini, Section::Lod, "fLODFadeOutMultObjectsMin", fMinDistObjects);
		SetValue(ini, Section::Lod, "fLODFadeOutMultObjectsMax", fMaxDistObjects);

		SetValue(ini, Section::Lod, "fLODFadeOutMultItemsMin", fMinDistItems);
		SetValue(ini, Section::Lod, "fLODFadeOutMultItemsMax", fMaxDistItems);

		SetValue(ini, Section::Lod, "fLODFadeOutMultActorsMin", fMinDistActors);
		SetValue(ini, Section::Lod, "fLODFadeOutMultActorsMax", fMaxDistActors);

		SetValue(ini, Section::Lod, "fDynamicValueFactor", fLodFactor);

		SetValue(ini, Section::Grass, "fGrassStartFadeDistanceMin", fMinDistGrass);
		SetValue(ini, Section::Grass, "fGrassStartFadeDistanceMax", fMaxDistGrass);

		SetValue(ini, Section::Grass, "fDynamicValueFactor", fGrassFactor);

		SetValue(ini, Section::TerrainManager, "fBlockLevel2DistanceMin", fMinDistBlockLevel2);
		SetValue(ini, Section::TerrainManager, "fBlockLevel2DistanceMax", fMaxDistBlockLevel2);

		SetValue(ini, Section::TerrainManager, "fBlockLevel1DistanceMin", fMinDistBlockLevel1);
		SetValue(ini, Section::TerrainManager, "fBlockLevel1DistanceMax", fMaxDistBlockLevel1);

		SetValue(ini, Section::TerrainManager, "fBlockLevel0DistanceMin", fMinDistBlockLevel0);
		SetValue(ini, Section::TerrainManager, "fBlockLevel0DistanceMax", fMaxDistBlockLevel0);

		SetValue(ini, Section::TerrainManager, "fDynamicValueFactor", fBlockFactor);

		SetValue(ini, Section::GodRays, "iQuality", grQuality);
		SetValue(ini, Section::GodRays, "iGrid", grGrid);
		SetValue(ini, Section::GodRays, "fScale", grScale);
		SetValue(ini, Section::GodRays, "iCascade", grCascade);

		SetValue(ini, Section::Log, "bLog", bLog);

		ini.SaveFile(dirLog.c_str());
	}

	Ini Ini::instance;
}
