 #pragma once

#include "pch.h"

namespace Settings {

	constexpr float DefaultFpsTarget = 58.0;
	constexpr float DefaultFpsDelay = 5.0;
	constexpr float DefaultMsTolerance = 0.0;

	constexpr float DefaultMinDistance = 2000.0;
	constexpr float DefaultMaxDistance = 4000.0;

	constexpr float DefaultMinDistObjects = 4.5;
	constexpr float DefaultMaxDistObjects = 6.0;

	constexpr float DefaultMinDistItems = 2.5;
	constexpr float DefaultMaxDistItems = 5.0;

	constexpr float DefaultMinDistActors = 6.0;
	constexpr float DefaultMaxDistActors = 12.0;

	constexpr float DefaultMinDistGrass = 3500.0;
	constexpr float DefaultMaxDistGrass = 7000.0;

	constexpr int MaxBlockLevels = 4;

	//ultra
	constexpr float DefaultDistBlockLevel2Ultra = 110000.0;
	constexpr float DefaultDistBlockLevel1Ultra = 90000.0;
	constexpr float DefaultDistBlockLevel0Ultra = 60000.0;

	//high
	constexpr float DefaultDistBlockLevel2High = 80000.0;
	constexpr float DefaultDistBlockLevel1High = 60000.0;
	constexpr float DefaultDistBlockLevel0High = 30000.0;

	//medium
	constexpr float DefaultDistBlockLevel2Medium = 80000.0;
	constexpr float DefaultDistBlockLevel1Medium = 32000.0;
	constexpr float DefaultDistBlockLevel0Medium = 20000.0;

	//low
	constexpr float DefaultDistBlockLevel2Low = 75000.0;
	constexpr float DefaultDistBlockLevel1Low = 25000.0;
	constexpr float DefaultDistBlockLevel0Low = 15000.0;

	constexpr float DistBlockLevel2[] = { DefaultDistBlockLevel2Ultra,
										  DefaultDistBlockLevel2High,
										  DefaultDistBlockLevel2Medium,
										  DefaultDistBlockLevel2Low };

	constexpr float DistBlockLevel1[] = { DefaultDistBlockLevel1Ultra,
										  DefaultDistBlockLevel1High,
										  DefaultDistBlockLevel1Medium,
										  DefaultDistBlockLevel1Low };

	constexpr float DistBlockLevel0[] = { DefaultDistBlockLevel0Ultra,
										  DefaultDistBlockLevel0High,
										  DefaultDistBlockLevel0Medium,
										  DefaultDistBlockLevel0Low };

	constexpr float DefaultShadowFactor = 100.0;
	constexpr float DefaultLodFactor = 0.1f;
	constexpr float DefaultGrassFactor = 100.0;

	constexpr std::int32_t DefaultGrQuality = 3;
	constexpr std::int32_t DefaultGrGrid = 8;
	constexpr float DefaultGrScale = 0.4f;
	constexpr std::int32_t DefaultGrCascade = 1;

	constexpr std::uint16_t DefaultKey = 122;
	constexpr std::uint16_t DefaultExtendedKey = 0;

	constexpr std::uint16_t DefaultKeyStop = 0;
	constexpr std::uint16_t DefaultExtendedKeyStop = 0;

	constexpr float DefaultFontScale = 1.0;

	constexpr bool DefaultLog = false;
	constexpr bool DefaultTransparent = false;
	constexpr bool DefaultPauseInMenu = false;
	constexpr bool DefaultShadow = true;
	constexpr bool DefaultLod = true;
	constexpr bool DefaultGrass = true;
	constexpr bool DefaultBlock = true;
	constexpr bool DefaultGodRays = true;

	static const char* DefaultBlackListMenu = "MainMenu;F4WheelMenu;BarterMenu;BookMenu;Console;ContainerMenu;CookingMenu;"
											  "ExamineMenu;FavoritesMenu;LoadingMenu;LockpickingMenu;Looksmenu;MessageBoxMenu;"
											  "PauseMenu;PipboyMenu;SleepWaitMenu;SPECIALMenu;WorkshopMenu"; 

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

	struct BlockLevel {

		float fDistBlockLevel2;
		float fDistBlockLevel1;
		float fDistBlockLevel0;
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

		BlockLevel blockLevel[MaxBlockLevels];

		float fShadowFactor;
		float fLodFactor;
		float fGrassFactor;
	};

	enum class MenuStyle {

		Classic = 0,
		Light,
		Dark
	};

	class Ini {

	public:

		[[nodiscard]] static Ini& GetSingleton() noexcept { return instance; }

		void SetDefault(bool isEditor = false) noexcept;
		void ReadSettings(bool isEditor = false) noexcept;
		[[nodiscard]] bool WriteSettings(bool isEditor = false) noexcept;

		[[nodiscard]] const float& GetFpsTarget() const noexcept { return m_values.fFpsTarget; }
		[[nodiscard]] const float& GetFpsDelay() const noexcept { return m_values.fFpsDelay; }
		[[nodiscard]] const float& GetMsTolerance() const noexcept { return m_values.fMsTolerance; }
		[[nodiscard]] const float& GetMinDistance() const noexcept { return values.fMinDistance; }
		[[nodiscard]] const float& GetMaxDistance() const noexcept { return values.fMaxDistance; }
		[[nodiscard]] const float& GetMinObjects() const noexcept { return values.fMinDistObjects; }
		[[nodiscard]] const float& GetMaxObjects() const noexcept { return values.fMaxDistObjects; }
		[[nodiscard]] const float& GetMinItems() const noexcept { return values.fMinDistItems; }
		[[nodiscard]] const float& GetMaxItems() const noexcept { return values.fMaxDistItems; }
		[[nodiscard]] const float& GetMinActors() const noexcept { return values.fMinDistActors; }
		[[nodiscard]] const float& GetMaxActors() const noexcept { return values.fMaxDistActors; }
		[[nodiscard]] const float& GetMinGrass() const noexcept { return values.fMinDistGrass; }
		[[nodiscard]] const float& GetMaxGrass() const noexcept { return values.fMaxDistGrass; }
		[[nodiscard]] const BlockLevel& GetBlockLevel(int index) const noexcept { return values.blockLevel[index]; }
		[[nodiscard]] const float& GetShadowFactor() const noexcept { return values.fShadowFactor; }
		[[nodiscard]] const float& GetLodFactor() const noexcept { return values.fLodFactor; }
		[[nodiscard]] const float& GetGrassFactor() const noexcept { return values.fGrassFactor; }
		[[nodiscard]] const bool& IsEnabledLog() const noexcept { return bLog; }
		[[nodiscard]] const bool& IsTransparent() const noexcept { return bTransparent; }
		[[nodiscard]] const bool& IsPauseInMenu() const noexcept { return bPauseInMenu; }
		[[nodiscard]] const std::uint16_t& GetKey() const noexcept { return iKey; }
		[[nodiscard]] const std::uint16_t& GetExtendedKey() const noexcept { return iExtendedKey; }
		[[nodiscard]] const std::uint16_t& GetKeyStop() const noexcept { return iKeyStop; }
		[[nodiscard]] const std::uint16_t& GetExtendedKeyStop() const noexcept { return iExtendedKeyStop; }
		[[nodiscard]] const float& GetFontScale() const noexcept { return fFontScale; }
		[[nodiscard]] const MenuStyle& GetMenuStyle() const noexcept { return mnuStyle; }
		[[nodiscard]] const std::int32_t& GetGrQuality() const noexcept { return m_values.grQuality; }
		[[nodiscard]] const std::int32_t& GetGrGrid() const noexcept { return m_values.grGrid; }
		[[nodiscard]] const float& GetGrScale() const noexcept { return m_values.grScale; }
		[[nodiscard]] const std::int32_t& GetGrCascade() const noexcept { return m_values.grCascade; }
		[[nodiscard]] const bool& GetShadowEnabled() const noexcept { return m_values.bShadowEnable; }
		[[nodiscard]] const bool& GetLodEnabled() const noexcept { return m_values.bLodEnable; }
		[[nodiscard]] const bool& GetGrassEnabled() const noexcept { return m_values.bGrassEnable; }
		[[nodiscard]] const bool& GetBlockEnabled() const noexcept { return m_values.bBlockEnable; }
		[[nodiscard]] const bool& GetGodRaysEnabled() const noexcept { return m_values.bGodRays; }

		void SetFpsTarget(float value) noexcept { m_values.fFpsTarget = value; }
		void SetFpsDelay(float value) noexcept { m_values.fFpsDelay = value; }
		void SetMsTolerance(float value) noexcept { m_values.fMsTolerance = value; }
		void SetMinDistance(float value) noexcept { values.fMinDistance = value; }
		void SetMaxDistance(float value) noexcept { values.fMaxDistance = value; }
		void SetMinObjects(float value) noexcept { values.fMinDistObjects = value; }
		void SetMaxObjects(float value) noexcept { values.fMaxDistObjects = value; }
		void SetMinItems(float value) noexcept { values.fMinDistItems = value; }
		void SetMaxItems(float value) noexcept { values.fMaxDistItems = value; }
		void SetMinActors(float value) noexcept { values.fMinDistActors = value; }
		void SetMaxActors(float value) noexcept { values.fMaxDistActors = value; }
		void SetMinGrass(float value) noexcept { values.fMinDistGrass = value; }
		void SetMaxGrass(float value) noexcept { values.fMaxDistGrass = value; }
		void SetBlockLevel(int index, const BlockLevel& blockLevel) noexcept { values.blockLevel[index] = blockLevel; }
		void SetShadowFactor(float value) noexcept { values.fShadowFactor = value; }
		void SetLodFactor(float value) noexcept { values.fLodFactor = value; }
		void SetGrassFactor(float value) noexcept { values.fGrassFactor = value; }
		void EnableLog(bool value) noexcept { bLog = value; }
		void EnableTransparent(bool value) noexcept { bTransparent = value; }
		void EnablePauseInMenu(bool value) noexcept { bPauseInMenu = value; }
		void SetKey(std::uint16_t value) noexcept { iKey = value; }
		void SetExtendedKey(std::uint16_t value) noexcept { iExtendedKey = value; }
		void SetKeyStop(std::uint16_t value) noexcept { iKeyStop = value; }
		void SetExtendedKeyStop(std::uint16_t value) noexcept { iExtendedKeyStop = value; }
		void SetFontScale(float value) noexcept { fFontScale = value; }
		void SetMenuStyle(MenuStyle value) noexcept { mnuStyle = value; }
		void SetGrQuality(std::int32_t value) noexcept { m_values.grQuality = value; }
		void SetGrGrid(std::int32_t value) noexcept { m_values.grGrid = value; }
		void SetGrScale(float value) noexcept { m_values.grScale = value; }
		void SetGrCascade(std::int32_t value) noexcept { m_values.grCascade = value; }
		void SetShadowEnabled(bool value) noexcept { m_values.bShadowEnable = value; }
		void SetLodEnabled(bool value) noexcept { m_values.bLodEnable = value; }
		void SetGrassEnabled(bool value) noexcept { m_values.bGrassEnable = value; }
		void SetBlockEnabled(bool value) noexcept { m_values.bBlockEnable = value; }
		void SetGodRaysEnabled(bool value) noexcept { m_values.bGodRays = value; }

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

		MainValues m_values{};
		Values values{};

		bool bLog{ DefaultLog };
		bool bTransparent{ DefaultTransparent };
		bool bPauseInMenu{ DefaultPauseInMenu };

		MenuStyle mnuStyle{ MenuStyle::Dark };

		std::vector<std::string> vBlacklistMenu;

		std::uint16_t iKey{ DefaultKey };
		std::uint16_t iExtendedKey{ DefaultExtendedKey };

		std::uint16_t iKeyStop{ DefaultKeyStop };
		std::uint16_t iExtendedKeyStop{ DefaultExtendedKeyStop };

		float fFontScale{ DefaultFontScale };

		std::string dirLog;

		std::string sBlackListMenu;

		static Ini instance;
	};
}
