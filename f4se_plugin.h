#pragma once

#include "f4se_libraries.h"

#include "d3d_hook.h"

#include "settings.h"

namespace f4se {

	class Plugin {

	public:

		static Plugin& GetInstance() noexcept
		{
			static Plugin instance;
			return instance;
		}

		[[nodiscard]] bool Query(const F4SE::QueryInterface* a_f4se, F4SE::PluginInfo* a_info);
		[[nodiscard]] bool Load(const F4SE::LoadInterface* a_f4se);

		void AddTask(F4SE::ITaskDelegate* task);

		const F4SE::PapyrusInterface& GetPapyrusInterface() { return *f4se_papyrus_interface; }

	private:

		Plugin() {}
		~Plugin() {}

		Plugin(const Plugin&) = delete;
		Plugin(Plugin&&) = delete;

		Plugin& operator=(const Plugin&) = delete;
		Plugin& operator=(Plugin&&) = delete;

		static void MsgCallback(F4SE::MessagingInterface::Message* msg);

		F4SE::MessagingInterface* f4se_msg_interface{ nullptr };
		F4SE::TaskInterface* f4se_task_interface{ nullptr };
		F4SE::PapyrusInterface* f4se_papyrus_interface{ nullptr };
	};
}
