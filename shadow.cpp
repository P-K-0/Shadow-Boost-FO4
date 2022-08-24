
#include "shadow.h"

namespace Shadow {

	void Boost::Init() noexcept
	{
		auto& settings = Settings::Ini::GetInstance();

		target = Second / settings.GetFpsTarget();
	}

	void Boost::RegisterEvents() noexcept
	{
		static bool isRegistered{ false };

		if (isRegistered)
			return;

		isRegistered = GetEventDispatcher<TESLoadGameEvent>()->AddEventSink(&instance);

		SetGodRays();
	}

	void Boost::operator()() noexcept
	{
		auto& settings = Settings::Ini::GetInstance();

		countFps++;

		if (countFps < settings.GetFpsDelay())
			return;

		countFps = 0.0f;

		auto delta = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - oldTime);

		avg = static_cast<float>(delta.count()) / Second / settings.GetFpsDelay(); 
		//fps = Second / avg;

		oldTime = std::chrono::high_resolution_clock::now();

		dyn = (avg - target); 

		if (fDirShadowDistance)
			*fDirShadowDistance = std::clamp(*fDirShadowDistance - dyn * settings.GetShadowFactor(), settings.GetMinDistance(), settings.GetMaxDistance());

		float d = dyn * settings.GetLodFactor();

		if (fLODFadeOutMultObjects)
			*fLODFadeOutMultObjects = std::clamp(*fLODFadeOutMultObjects - d, settings.GetMinObjects(), settings.GetMaxObjects());

		if (fLODFadeOutMultItems)
			*fLODFadeOutMultItems = std::clamp(*fLODFadeOutMultItems - d, settings.GetMinItems(), settings.GetMaxItems());
		
		if (fLODFadeOutMultActors)
			*fLODFadeOutMultActors = std::clamp(*fLODFadeOutMultActors - d, settings.GetMinActors(), settings.GetMaxActors());

		if (fGrassStartFadeDistance)
			*fGrassStartFadeDistance = std::clamp(*fGrassStartFadeDistance - dyn * settings.GetGrassFactor(), settings.GetMinGrass(), settings.GetMaxGrass());

		if (fBlockLevel2Distance && fBlockLevel1Distance && fBlockLevel0Distance) {

			d = dyn * settings.GetBlockFactor();

			*fBlockLevel2Distance = std::clamp(*fBlockLevel2Distance - d, settings.GetMinBlockLevel2(), settings.GetMaxBlockLevel2());
			*fBlockLevel1Distance = std::clamp(*fBlockLevel1Distance - d, settings.GetMinBlockLevel1(), settings.GetMaxBlockLevel1());
			*fBlockLevel0Distance = std::clamp(*fBlockLevel0Distance - d, settings.GetMinBlockLevel0(), settings.GetMaxBlockLevel0());
		}
	}

	void Boost::SetGodRays()
	{
		auto& settings = Settings::Ini::GetInstance();

		if (GrQuality && GrGrid && GrScale && GrCascade) {

			*GrQuality = settings.GetGrQuality();
			*GrGrid = settings.GetGrGrid();
			*GrScale = settings.GetGrScale();
			*GrCascade = settings.GetGrCascade();
		}
	}

	EventResult	Boost::ReceiveEvent(TESLoadGameEvent* evn, void* dispatcher)
	{
		SetGodRays();

		return kEvent_Continue; 
	};

	Boost Boost::instance;
}
