
#include "f4se_plugin.h"

extern "C" {

	bool F4SEPlugin_Query(const F4SEInterface* f4se, PluginInfo* info)
	{
		return f4se::Plugin::GetInstance().Query(f4se, info);
	}

	bool F4SEPlugin_Load(const F4SEInterface* f4se)
	{
		return f4se::Plugin::GetInstance().Load(f4se);
	}
}
