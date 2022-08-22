#pragma once

#include "SimpleIni.h"

#include "f4se_plugin.h"

namespace Settings {

	class Ini {

	public:

		static Ini& GetInstance() { return instance; }

		void ReadSettings();

		[[nodiscard]] const float GetFpsTarget() const { return fFpsTarget; }
		[[nodiscard]] const float GetFpsDelay() const { return fFpsDelay; }
		[[nodiscard]] const float GetMinDistance() const { return fMinDistance; }
		[[nodiscard]] const float GetMaxDistance() const { return fMaxDistance; }

		[[nodiscard]] const bool IsEnabledLog() const { return bLog; }
		[[nodiscard]] const float GetLogPosX() const { return fLogPosX; }
		[[nodiscard]] const float GetLogPosY() const { return fLogPosY; }

	private:

		Ini() {}
		~Ini() {}

		Ini(const Ini&) = delete;
		Ini(Ini&&) = delete;

		Ini& operator=(const Ini&) = delete;
		Ini& operator=(Ini&&) = delete;

		float fFpsTarget{ 58.0 };
		float fFpsDelay{ 5.0 };
		float fMinDistance{ 2000.0 };
		float fMaxDistance{ 10000.0 };

		bool bLog{ false };
		float fLogPosX{ 0.0 };
		float fLogPosY{ 0.0 };

		static Ini instance;
	};
}
