#pragma once

#include "f4se_libraries.h"
#include "std_libraries.h"

#include "settings.h"

namespace Shadow {

	constexpr float Second = 1000.0;

	static RelocPtr<float> fDirShadowDistance(0x067333DC);

	static RelocPtr<float> fLODFadeOutMultObjects(0x038C4E1C);
	static RelocPtr<float> fLODFadeOutMultItems(0x038C4E20);
	static RelocPtr<float> fLODFadeOutMultActors(0x038C4E24);

	static RelocPtr<float> fGrassStartFadeDistance(0x06722D88);

	static RelocPtr<float> fBlockLevel2Distance(0x037498E8);
	static RelocPtr<float> fBlockLevel1Distance(0x037498D0);
	static RelocPtr<float> fBlockLevel0Distance(0x037498B8);

	static RelocPtr<std::int32_t> GrQuality(0x038C9F08);
	static RelocPtr<std::int32_t> GrGrid(0x0673296C);
	static RelocPtr<float> GrScale(0x06732964);
	static RelocPtr<std::int32_t> GrCascade(0x038C9EDC);

	class Boost : 
		public BSTEventSink<TESLoadGameEvent> {

	public:

		static Boost& GetInstance() noexcept { return instance; }

		void Init() noexcept;
		void RegisterEvents() noexcept;

		virtual	EventResult	ReceiveEvent(TESLoadGameEvent* evn, void* dispatcher);

		void operator()() noexcept;

		operator bool() noexcept {
			return (fDirShadowDistance &&
				fLODFadeOutMultObjects &&
				fLODFadeOutMultItems &&
				fLODFadeOutMultActors &&
				fGrassStartFadeDistance &&
				fBlockLevel2Distance &&
				fBlockLevel1Distance &&
				fBlockLevel0Distance &&
				GrQuality &&
				GrGrid &&
				GrScale &&
				GrCascade);
		}

		[[nodiscard]] const float& GetFps() const noexcept { return fps; }
		[[nodiscard]] const float& GetMs() const noexcept { return avg; }
		[[nodiscard]] const float& GetDynamicvalue() const noexcept { return dyn; }

		[[nodiscard]] const float& GetDirShadowDistance() const noexcept { return *fDirShadowDistance; }

		[[nodiscard]] const float& GetLODFadeOutMultObjects() const noexcept { return *fLODFadeOutMultObjects; }
		[[nodiscard]] const float& GetLODFadeOutMultItems() const noexcept { return *fLODFadeOutMultItems; }
		[[nodiscard]] const float& GetLODFadeOutMultActors() const noexcept { return *fLODFadeOutMultActors; }

		[[nodiscard]] const float& GetGrassDistance() const noexcept { return *fGrassStartFadeDistance; }

		[[nodiscard]] const float& GetBlockLevel2() const noexcept { return *fBlockLevel2Distance; }
		[[nodiscard]] const float& GetBlockLevel1() const noexcept { return *fBlockLevel1Distance; }
		[[nodiscard]] const float& GetBlockLevel0() const noexcept { return *fBlockLevel0Distance; }

		[[nodiscard]] const std::int32_t& GetGrQuality() const noexcept { return *GrQuality; }
		[[nodiscard]] const std::int32_t& GetGrGrid() const noexcept { return *GrGrid; }
		[[nodiscard]] const float& GetGrScale() const noexcept { return *GrScale; }
		[[nodiscard]] const std::int32_t& GetGrCascade() const noexcept { return *GrCascade; }
		
		void SetGodRays();

	private:

		Boost() {}
		~Boost() {}

		Boost(const Boost&) = delete;
		Boost(Boost&&) = delete;

		Boost& operator=(const Boost&) = delete;
		Boost& operator=(Boost&&) = delete;

		std::chrono::steady_clock::time_point oldTime{ std::chrono::high_resolution_clock::now() };

		float countFps{};
		float target{};
		float fps{};
		float avg{};
		float dyn{};

		static Boost instance;
	};
}
