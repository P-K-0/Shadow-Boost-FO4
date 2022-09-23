 #pragma once

#include "pch.h"

namespace Settings {

	constexpr float DefaultFpsTarget = 58.0;
	constexpr float DefaultFpsDelay = 5.0;
	constexpr float DefaultMsTolerance = 0.0;

	constexpr float DefaultMinDistance = 2000.0;
	constexpr float DefaultMaxDistance = 10000.0;

	constexpr float DefaultMinDistObjects = 4.5;
	constexpr float DefaultMaxDistObjects = 6.0;

	constexpr float DefaultMinDistItems = 2.5;
	constexpr float DefaultMaxDistItems = 5.0;

	constexpr float DefaultMinDistActors = 6.0;
	constexpr float DefaultMaxDistActors = 12.0;

	constexpr float DefaultMinDistGrass = 4000.0;
	constexpr float DefaultMaxDistGrass = 10000.0;

	constexpr float DefaultMinDistBlockLevel2 = 30000.0;
	constexpr float DefaultMaxDistBlockLevel2 = 40000.0;

	constexpr float DefaultMinDistBlockLevel1 = 15000.0;
	constexpr float DefaultMaxDistBlockLevel1 = 20000.0;

	constexpr float DefaultMinDistBlockLevel0 = 10000.0;
	constexpr float DefaultMaxDistBlockLevel0 = 12000.0;

	constexpr float DefaultShadowFactor = 100.0;
	constexpr float DefaultLodFactor = 0.1f;
	constexpr float DefaultGrassFactor = 100.0;
	constexpr float DefaultBlockLevelFactor = 100.0;

	constexpr std::int32_t DefaultGrQuality = 3;
	constexpr std::int32_t DefaultGrGrid = 8;
	constexpr float DefaultGrScale = 0.4f;
	constexpr std::int32_t DefaultGrCascade = 1;

	constexpr std::int32_t DefaultKey = 123;
	constexpr std::int32_t DefaultExtendedKey = 0;

	constexpr bool DefaultLog = false;
	constexpr bool DefaultTransparent = false;
	constexpr bool DefaultPauseInMenu = false;
	constexpr bool DefaultShadow = true;
	constexpr bool DefaultLod = true;
	constexpr bool DefaultGrass = true;
	constexpr bool DefaultBlock = true;
	constexpr bool DefaultGodRays = true;

	static const char* DefaultBlackListMenu = "MainMenu;LoadingMenu;PipboyMenu;PauseMenu;SleepWaitMenu"; 

	struct MainValues {

		float fFpsTarget;
		float fFpsDelay;
		float fMsTolerance;

		bool bShadowEnable;
		bool bLodEnable;
		bool bGrassEnable;
		bool bBlockEnable;
		bool bGodRays;

		std::int32_t grQuality;
		std::int32_t grGrid;
		float grScale;
		std::int32_t grCascade;
	};

	struct Values {

		float fMinDistance;
		float fMaxDistance;

		float fMinDistObjects;
		float fMaxDistObjects;

		float fMinDistItems;
		float fMaxDistItems;

		float fMinDistActors;
		float fMaxDistActors;

		float fMinDistGrass;
		float fMaxDistGrass;

		float fMinDistBlockLevel2;
		float fMaxDistBlockLevel2;

		float fMinDistBlockLevel1;
		float fMaxDistBlockLevel1;

		float fMinDistBlockLevel0;
		float fMaxDistBlockLevel0;

		float fShadowFactor;
		float fLodFactor;
		float fGrassFactor;
		float fBlockFactor;
	};

	class Ini {

	public:

		[[nodiscard]] static Ini& GetInstance() noexcept { return instance; }

		void ReadSettings() noexcept;
		[[nodiscard]] bool WriteSettings() noexcept;
		void SetDefault() noexcept;
		void SetNameKey(bool wait = false) noexcept;

		[[nodiscard]] float& GetFpsTarget() noexcept { return m_values.fFpsTarget; }
		[[nodiscard]] float& GetFpsDelay() noexcept { return m_values.fFpsDelay; }
		[[nodiscard]] float& GetMsTolerance() noexcept { return m_values.fMsTolerance; }

		[[nodiscard]] float& GetMinDistance() noexcept { return values.fMinDistance; }
		[[nodiscard]] float& GetMaxDistance() noexcept { return values.fMaxDistance; }

		[[nodiscard]] float& GetMinObjects() noexcept { return values.fMinDistObjects; }
		[[nodiscard]] float& GetMaxObjects() noexcept { return values.fMaxDistObjects; }

		[[nodiscard]] float& GetMinItems() noexcept { return values.fMinDistItems; }
		[[nodiscard]] float& GetMaxItems() noexcept { return values.fMaxDistItems; }

		[[nodiscard]] float& GetMinActors() noexcept { return values.fMinDistActors; }
		[[nodiscard]] float& GetMaxActors() noexcept { return values.fMaxDistActors; }

		[[nodiscard]] float& GetMinGrass() noexcept { return values.fMinDistGrass; }
		[[nodiscard]] float& GetMaxGrass() noexcept { return values.fMaxDistGrass; }

		[[nodiscard]] float& GetMinBlockLevel2() noexcept { return values.fMinDistBlockLevel2; }
		[[nodiscard]] float& GetMaxBlockLevel2() noexcept { return values.fMaxDistBlockLevel2; }

		[[nodiscard]] float& GetMinBlockLevel1() noexcept { return values.fMinDistBlockLevel1; }
		[[nodiscard]] float& GetMaxBlockLevel1() noexcept { return values.fMaxDistBlockLevel1; }

		[[nodiscard]] float& GetMinBlockLevel0() noexcept { return values.fMinDistBlockLevel0; }
		[[nodiscard]] float& GetMaxBlockLevel0() noexcept { return values.fMaxDistBlockLevel0; }

		[[nodiscard]] float& GetShadowFactor() noexcept { return values.fShadowFactor; }

		[[nodiscard]] float& GetLodFactor() noexcept { return values.fLodFactor; }

		[[nodiscard]] float& GetGrassFactor() noexcept { return values.fGrassFactor; }

		[[nodiscard]] float& GetBlockFactor() noexcept { return values.fBlockFactor; }

		[[nodiscard]] bool& IsEnabledLog() noexcept { return bLog; }
		[[nodiscard]] bool& IsTransparent() noexcept { return bTransparent; }
		[[nodiscard]] bool& IsPauseInMenu() noexcept { return bPauseInMenu; }

		[[nodiscard]] std::int32_t& GetKey() noexcept { return iKey; }
		[[nodiscard]] std::int32_t& GetExtendedKey() noexcept { return iExtendedKey; }

		[[nodiscard]] const std::string& GetHotkey() const noexcept { return strHotkey; }

		[[nodiscard]] std::int32_t& GetGrQuality() noexcept { return m_values.grQuality; }
		[[nodiscard]] std::int32_t& GetGrGrid() noexcept { return m_values.grGrid; }
		[[nodiscard]] float& GetGrScale() noexcept { return m_values.grScale; }
		[[nodiscard]] std::int32_t& GetGrCascade() noexcept { return m_values.grCascade; }

		[[nodiscard]] bool& GetShadowEnabled() noexcept { return m_values.bShadowEnable; }
		[[nodiscard]] bool& GetLodEnabled() noexcept { return m_values.bLodEnable; }
		[[nodiscard]] bool& GetGrassEnabled() noexcept { return m_values.bGrassEnable; }
		[[nodiscard]] bool& GetBlockEnabled() noexcept { return m_values.bBlockEnable; }
		[[nodiscard]] bool& GetGodRaysEnabled() noexcept { return m_values.bGodRays; }

		void SetMainValues(const MainValues& val) noexcept { m_values = val; }
		void SetValues(const Values& val) noexcept { values = val; }

		[[nodiscard]] const MainValues& GetMainValues() const noexcept { return m_values; }
		[[nodiscard]] const Values& GetValues() const noexcept { return values; }

		[[nodiscard]] const bool IsMenuOpen() noexcept;

	private:

		Ini() noexcept {}
		~Ini() noexcept {}

		Ini(const Ini&) = delete;
		Ini(Ini&&) = delete;

		Ini& operator=(const Ini&) = delete;
		Ini& operator=(Ini&&) = delete;

		void ParseString(const std::string& str, std::vector<std::string>& vec) noexcept;
		const std::string GetNameKey(std::int32_t key) noexcept;

		MainValues m_values{};
		Values values{};

		bool bLog{ DefaultLog };
		bool bTransparent{ DefaultTransparent };
		bool bPauseInMenu{ DefaultPauseInMenu };

		std::vector<std::string> vBlacklistMenu;

		std::int32_t iKey{ DefaultKey };
		std::int32_t iExtendedKey{ DefaultExtendedKey };

		std::string dirLog;
		std::string strHotkey;

		std::string sBlackListMenu;

		static Ini instance;
	};
}
