
#include "settings.h"

namespace Settings {

	void Ini::ReadSettings()
	{
		std::string dirLog{ "Data\\F4SE\\Plugins\\" };

		dirLog += f4se::NamePlugin;
		dirLog += ".ini";

		CSimpleIni ini;

		ini.SetUnicode();
		SI_Error error = ini.LoadFile(dirLog.c_str());

		if (error == 0) {

			fFpsTarget = std::stof(ini.GetValue("Main", "fFpsTarget", "58.0"));
			fFpsDelay = std::stof(ini.GetValue("Main", "fFpsDelay", "5.0"));
			fMinDistance = std::stof(ini.GetValue("Main", "fMinDistance", "2000.0"));
			fMaxDistance = std::stof(ini.GetValue("Main", "fMaxDistance", "10000.0"));

			bLog = std::stof(ini.GetValue("Log", "bLog", "0"));
			fLogPosX = std::stof(ini.GetValue("Log", "fLogPosX", "0.0"));
			fLogPosY = std::stof(ini.GetValue("Log", "fLogPosY", "0.0"));
		}
	}

	Ini Ini::instance;
}
