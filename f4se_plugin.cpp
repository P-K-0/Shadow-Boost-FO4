
#include "f4se_plugin.h"

namespace f4se {

	bool Plugin::Query(const F4SEInterface* f4se, PluginInfo* info)
	{
		std::string dirLog{ "\\My Games\\Fallout4\\F4SE\\" };

		dirLog += NamePlugin;
		dirLog += ".log";

		iLog.OpenRelative(CSIDL_MYDOCUMENTS, dirLog.c_str());

		info->infoVersion = PluginInfo::kInfoVersion;
		info->name = NamePlugin;
		info->version = 1;

		hPlugin = f4se->GetPluginHandle();

		if (f4se->runtimeVersion < RUNTIME_VERSION_1_10_162) {

			_ERROR("runtime error!");

			return false;
		}

		if (f4se->isEditor) {

			_ERROR("Plugin is not compatible with editor!");

			return false;
		}

		if (!(f4se_msg_interface = static_cast<F4SEMessagingInterface*>(f4se->QueryInterface(kInterface_Messaging)))) {

			_ERROR("Messaging Interface error!");

			return false;
		}

		if (!(f4se_task_interface = static_cast<F4SETaskInterface*>(f4se->QueryInterface(kInterface_Task)))) {

			_ERROR("Task interface error!");

			return false;
		}

		if (!(f4se_papyrus_interface = static_cast<F4SEPapyrusInterface*>(f4se->QueryInterface(kInterface_Papyrus)))) {

			_ERROR("Papyrus interface error!");

			return false;
		}

		Settings::Ini::GetInstance().ReadSettings();

		Shadow::Boost::GetInstance().Init();

		Hook::D3D::Register();

		_DMESSAGE("Shadow Boost FO4 by PK0 started...");

		return true;
	}

	bool Plugin::Load(const F4SEInterface* f4se)
	{
		if (f4se_msg_interface)
			return f4se_msg_interface->RegisterListener(hPlugin, "F4SE", MsgCallback);

		return false;
	}

	void Plugin::MsgCallback(F4SEMessagingInterface::Message* msg)
	{
		switch (msg->type) {

		case F4SEMessagingInterface::kMessage_NewGame:

			break;

		case F4SEMessagingInterface::kMessage_GameLoaded:

			Shadow::Boost::GetInstance().RegisterEvents();

			break;
		}
	}

	void Plugin::AddTask(ITaskDelegate* task)
	{
		if (f4se_task_interface)
			f4se_task_interface->AddTask(task);
	}
}
