#pragma once

#include "SimpleIni.h"

#include "f4se_plugin.h"

namespace Settings {

	constexpr float DefaultFpsTarget = 58.0;
	constexpr float DefaultFpsDelay = 5.0;

	constexpr float DefaultMinDistance = 2000.0;
	constexpr float DefaultMaxDistance = 10000.0;

	constexpr float DefaultMinDistObjects = 4.5;
	constexpr float DefaultMaxDistObjects = 9.0;

	constexpr float DefaultMinDistItems = 2.5;
	constexpr float DefaultMaxDistItems = 5.0;

	constexpr float DefaultMinDistActors = 6.0;
	constexpr float DefaultMaxDistActors = 12.0;

	constexpr float DefaultMinDistGrass = 6000.0;
	constexpr float DefaultMaxDistGrass = 10000.0;

	constexpr float DefaultMinDistBlockLevel2 = 30000.0;
	constexpr float DefaultMaxDistBlockLevel2 = 40000.0;

	constexpr float DefaultMinDistBlockLevel1 = 15000.0;
	constexpr float DefaultMaxDistBlockLevel1 = 20000.0;

	constexpr float DefaultMinDistBlockLevel0 = 10000.0;
	constexpr float DefaultMaxDistBlockLevel0 = 12000.0;

	constexpr float DefaultShadowFactor = 100.0;
	constexpr float DefaultLodFactor = 0.1;
	constexpr float DefaultGrassFactor = 100.0;
	constexpr float DefaultBlockLevelFactor = 100.0;

	constexpr std::int32_t DefaultGrQuality = 3;
	constexpr std::int32_t DefaultGrGrid = 8;
	constexpr float DefaultGrScale = 0.4f;
	constexpr std::int32_t DefaultGrCascade = 1;

	constexpr bool DefaultLog = false;

	class Ini {

	public:

		static Ini& GetInstance() noexcept { return instance; }

		void ReadSettings() noexcept;
		void WriteSettings() noexcept;

		[[nodiscard]] float& GetFpsTarget() noexcept { return fFpsTarget; }
		[[nodiscard]] float& GetFpsDelay() noexcept { return fFpsDelay; }

		[[nodiscard]] float& GetMinDistance() noexcept { return fMinDistance; }
		[[nodiscard]] float& GetMaxDistance() noexcept { return fMaxDistance; }

		[[nodiscard]] float& GetMinObjects() noexcept { return fMinDistObjects; }
		[[nodiscard]] float& GetMaxObjects() noexcept { return fMaxDistObjects; }

		[[nodiscard]] float& GetMinItems() noexcept { return fMinDistItems; }
		[[nodiscard]] float& GetMaxItems() noexcept { return fMaxDistItems; }

		[[nodiscard]] float& GetMinActors() noexcept { return fMinDistActors; }
		[[nodiscard]] float& GetMaxActors() noexcept { return fMaxDistActors; }

		[[nodiscard]] float& GetMinGrass() noexcept { return fMinDistGrass; }
		[[nodiscard]] float& GetMaxGrass() noexcept { return fMaxDistGrass; }

		[[nodiscard]] float& GetMinBlockLevel2() noexcept { return fMinDistBlockLevel2; }
		[[nodiscard]] float& GetMaxBlockLevel2() noexcept { return fMaxDistBlockLevel2; }

		[[nodiscard]] float& GetMinBlockLevel1() noexcept { return fMinDistBlockLevel1; }
		[[nodiscard]] float& GetMaxBlockLevel1() noexcept { return fMaxDistBlockLevel1; }

		[[nodiscard]] float& GetMinBlockLevel0() noexcept { return fMinDistBlockLevel0; }
		[[nodiscard]] float& GetMaxBlockLevel0() noexcept { return fMaxDistBlockLevel0; }

		[[nodiscard]] float& GetShadowFactor() noexcept { return fShadowFactor; }

		[[nodiscard]] float& GetLodFactor() noexcept { return fLodFactor; }

		[[nodiscard]] float& GetGrassFactor() noexcept { return fGrassFactor; }

		[[nodiscard]] float& GetBlockFactor() noexcept { return fBlockFactor; }

		[[nodiscard]] bool& IsEnabledLog() noexcept { return bLog; }

		[[nodiscard]] const int& GetKey() const noexcept { return iKey; }

		[[nodiscard]] std::int32_t& GetGrQuality() noexcept { return grQuality; }
		[[nodiscard]] std::int32_t& GetGrGrid() noexcept { return grGrid; }
		[[nodiscard]] float& GetGrScale() noexcept { return grScale; }
		[[nodiscard]] std::int32_t& GetGrCascade() noexcept { return grCascade; }

	private:

		Ini() {}
		~Ini() {}

		Ini(const Ini&) = delete;
		Ini(Ini&&) = delete;

		Ini& operator=(const Ini&) = delete;
		Ini& operator=(Ini&&) = delete;

		float fFpsTarget{ DefaultFpsTarget };
		float fFpsDelay{ DefaultFpsDelay };

		float fMinDistance{ DefaultMinDistance };
		float fMaxDistance{ DefaultMaxDistance };

		float fMinDistObjects{ DefaultMinDistObjects };
		float fMaxDistObjects{ DefaultMaxDistObjects };

		float fMinDistItems{ DefaultMinDistItems };
		float fMaxDistItems{ DefaultMaxDistItems };

		float fMinDistActors{ DefaultMinDistActors };
		float fMaxDistActors{ DefaultMaxDistActors };

		float fMinDistGrass{ DefaultMinDistGrass };
		float fMaxDistGrass{ DefaultMaxDistGrass };

		float fMinDistBlockLevel2{ DefaultMinDistBlockLevel2 };
		float fMaxDistBlockLevel2{ DefaultMaxDistBlockLevel2 };

		float fMinDistBlockLevel1{ DefaultMinDistBlockLevel1 };
		float fMaxDistBlockLevel1{ DefaultMaxDistBlockLevel1 };

		float fMinDistBlockLevel0{ DefaultMinDistBlockLevel0 };
		float fMaxDistBlockLevel0{ DefaultMaxDistBlockLevel0 };

		float fShadowFactor{ DefaultShadowFactor };
		float fLodFactor{ DefaultLodFactor };
		float fGrassFactor{ DefaultGrassFactor };
		float fBlockFactor{ DefaultBlockLevelFactor };

		std::int32_t grQuality{ DefaultGrQuality };
		std::int32_t grGrid{ DefaultGrGrid };
		float grScale{ DefaultGrScale };
		std::int32_t grCascade{ DefaultGrCascade };

		bool bLog{ DefaultLog };

		int iKey{ VK_F12 };

		std::string dirLog{};

		static Ini instance;
	};
}
