
#include "shadow.h"

namespace Shadow {

	void Boost::Init()
	{
		auto& settings = Settings::Ini::GetInstance();

		Target = Second / settings.GetFpsTarget();
		Delay = settings.GetFpsDelay();
		Min = settings.GetMinDistance();
		Max = settings.GetMaxDistance();
	}

	void Boost::operator()()
	{
		if (!fDirShadowDistance)
			return;

		countFps++;

		if (countFps < Delay)
			return;

		countFps = 0;

		auto delta = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - oldTime);

		avg = static_cast<float>(delta.count()) / Second / Delay;

		oldTime = std::chrono::high_resolution_clock::now();

		dyn = (avg - Target) * 100.0;

		*fDirShadowDistance = std::clamp(*fDirShadowDistance - dyn, Min, Max);
	}

	Boost Boost::instance;
}

