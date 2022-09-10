
#include "f4se_plugin.h"

extern "C" DLLEXPORT bool F4SEAPI F4SEPlugin_Query(const F4SE::QueryInterface * a_f4se, F4SE::PluginInfo * a_info)
{
	return f4se::Plugin::GetInstance().Query(a_f4se, a_info);
}

extern "C" DLLEXPORT bool F4SEAPI F4SEPlugin_Load(const F4SE::LoadInterface * a_f4se)
{
	return f4se::Plugin::GetInstance().Load(a_f4se);
}
