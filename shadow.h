#pragma once

#include "f4se_libraries.h"
#include "std_libraries.h"

#include "settings.h"

namespace Shadow {

	constexpr float Second = 1000.0;

	static RelocPtr<float> fDirShadowDistance(0x067333DC);

	class Boost {

	public:

		static Boost& GetInstance() { return instance; }

		void Init();

		void operator()();

		[[nodiscard]] const float GetMs() const { return avg; }
		[[nodiscard]] const float GetDirShadowDistance() const { return !fDirShadowDistance ? 0.0 : *fDirShadowDistance; }
		[[nodiscard]] const float GetDynamicvalue() const { return dyn; }
		[[nodiscard]] const float GetMin() const { return Min; }
		[[nodiscard]] const float GetMax() const { return Max; }

	private:

		Boost() {}
		~Boost() {}

		Boost(const Boost&) = delete;
		Boost(Boost&&) = delete;

		Boost& operator=(const Boost&) = delete;
		Boost& operator=(Boost&&) = delete;

		std::chrono::steady_clock::time_point oldTime{ std::chrono::high_resolution_clock::now() };

		std::uint32_t countFps{};

		float avg{};
		float dyn{};

		float Target{};
		float Delay{};
		float Min{};
		float Max{};

		static Boost instance;
	};
}
