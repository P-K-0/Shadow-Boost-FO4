#pragma once

#include "f4se_libraries.h"
#include "std_libraries.h"

#include "settings.h"

namespace Shadow {

	constexpr float Second = 1000.0;

	static REL::Relocation<float*> fDirShadowDistance(REL::ID(777729));

	static REL::Relocation<float*> fLODFadeOutMultObjects(REL::ID(123089));
	static REL::Relocation<float*> fLODFadeOutMultItems(REL::ID(1023729));
	static REL::Relocation<float*> fLODFadeOutMultActors(REL::ID(342335));

	static REL::Relocation<float*> fGrassStartFadeDistance(REL::ID(183225));

	static REL::Relocation<float*> fBlockLevel2Distance(REL::ID(379949));
	static REL::Relocation<float*> fBlockLevel1Distance(REL::ID(1304050));
	static REL::Relocation<float*> fBlockLevel0Distance(REL::ID(646264));

	static REL::Relocation<std::int32_t*> GrQuality(REL::ID(957649));
	static REL::Relocation<std::int32_t*> GrGrid(REL::ID(289565));
	static REL::Relocation<float*> GrScale(REL::ID(70509));
	static REL::Relocation<std::int32_t*> GrCascade(REL::ID(542491));

	template<typename T>
	inline constexpr RE::BSTEventSource<T>* GetDispatcher(const REL::ID& id)
	{
		using fn = RE::BSTEventSource<T>* (*)();
		REL::Relocation<fn> func(id);
		return func();
	}

	struct TESCellAttachDetachEvent {

		RE::TESObjectREFR* refr;
		std::uint32_t unknown;
	};

	struct TESLoadGameEvent {

	};

	class TaskLocation : 
		public F4SE::ITaskDelegate {

	public:

		TaskLocation() {}

		virtual void Run();
	};

	class Boost : 
		public RE::BSTEventSink<RE::MenuOpenCloseEvent>,
		public RE::BSTEventSink<TESCellAttachDetachEvent>,
		public RE::BSTEventSink<TESLoadGameEvent>,
		public RE::BSTEventSink<RE::CellAttachDetachEvent> {

	public:

		static Boost& GetInstance() noexcept { return instance; }

		void GameLoaded() noexcept;

		void ReloadStuff() noexcept;

		void EnableShadow() noexcept;
		void EnableLod() noexcept;
		void EnableBlock() noexcept;
		void EnableGrass() noexcept;
		void EnableGodRays() noexcept;

		void SetGodRays() noexcept;
		void SetupFps() noexcept;
		void SetPlayerLocation() noexcept;

		void Run() noexcept { SetupFps(); run = true; }
		void Pause() noexcept { if (Settings::Ini::GetInstance().IsPauseInMenu()) { run = false; } }

		virtual RE::BSEventNotifyControl ProcessEvent(const RE::MenuOpenCloseEvent& a_event, RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_source);
		virtual RE::BSEventNotifyControl ProcessEvent(const TESLoadGameEvent& a_event, RE::BSTEventSource<TESLoadGameEvent>* a_source);
		virtual RE::BSEventNotifyControl ProcessEvent(const TESCellAttachDetachEvent& a_event, RE::BSTEventSource<TESCellAttachDetachEvent>* a_source);
		virtual RE::BSEventNotifyControl ProcessEvent(const RE::CellAttachDetachEvent& a_event, RE::BSTEventSource<RE::CellAttachDetachEvent>* a_source);

		void operator()() noexcept;

		operator bool() noexcept;

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
		
	private:

		Boost() {}
		~Boost() {}

		Boost(const Boost&) = delete;
		Boost(Boost&&) = delete;

		Boost& operator=(const Boost&) = delete;
		Boost& operator=(Boost&&) = delete;

		std::chrono::steady_clock::time_point oldTime{};

		void SaveOriginalValues() noexcept;

		bool run{};

		float countFps{};
		float target{};
		float tolerance{};
		float fps{};
		float avg{};
		float dyn{};

		float o_fDirShadowDistance{};
		float o_fLODFadeOutMultObjects{};
		float o_fLODFadeOutMultItems{};
		float o_fLODFadeOutMultActors{};
		float o_fGrassStartFadeDistance{};
		float o_fBlockLevel2Distance{};
		float o_fBlockLevel1Distance{};
		float o_fBlockLevel0Distance{};
		std::int32_t o_GrQuality{};
		std::int32_t o_GrGrid{};
		float o_GrScale{};
		std::int32_t o_GrCascade{};

		std::unordered_map<std::string, bool> mapMenus;

		static Boost instance;
	};
}
