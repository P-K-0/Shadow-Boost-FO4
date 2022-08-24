#pragma once

#include "f4se_libraries.h"

#include "d3d_hook.h"

#include "settings.h"

namespace f4se {

	constexpr char* NamePlugin = "ShadowBoostFO4";

	class Plugin {

	public:

		static Plugin& GetInstance() noexcept
		{
			static Plugin instance;
			return instance;
		}

		[[nodiscard]] bool Query(const F4SEInterface* f4se, PluginInfo* info);
		[[nodiscard]] bool Load(const F4SEInterface* f4se);

		void AddTask(ITaskDelegate* task);

		//F4SEPapyrusInterface& GetPapyrusInterface() { return *f4se_papyrus_interface; }

	private:

		Plugin() {}
		~Plugin() {}

		Plugin(const Plugin&) = delete;
		Plugin(Plugin&&) = delete;

		Plugin& operator=(const Plugin&) = delete;
		Plugin& operator=(Plugin&&) = delete;

		static void MsgCallback(F4SEMessagingInterface::Message* msg);

		IDebugLog iLog;

		PluginHandle hPlugin;

		F4SEMessagingInterface* f4se_msg_interface{ nullptr };
		F4SETaskInterface* f4se_task_interface{ nullptr };
		F4SEPapyrusInterface* f4se_papyrus_interface{ nullptr };
	};
}
