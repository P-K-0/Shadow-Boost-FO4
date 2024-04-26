
#include "f4se_plugin.h"

#include "translation.h"
#include "shadow.h"
#include "json_preset.h"

namespace f4se {

	bool Plugin::Query(const F4SE::QueryInterface* a_f4se, F4SE::PluginInfo* a_info) noexcept
	{
#ifndef NDEBUG
		auto sink = std::make_shared<spdlog::sinks::msvc_sink_mt>();
#else
		auto path = logger::log_directory();
		if (!path) {
			return false;
		}

		*path /= fmt::format(FMT_STRING("{}.log"), Version::Project);
		auto sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true);
#endif

		auto log = std::make_shared<spdlog::logger>("global log"s, std::move(sink));

#ifndef NDEBUG
		log->set_level(spdlog::level::trace);
#else
		log->set_level(spdlog::level::info);
		log->flush_on(spdlog::level::info);
#endif

		spdlog::set_default_logger(std::move(log));
		spdlog::set_pattern("%s(%#): [%^%l%$] %v"s);

		logger::info(FMT_STRING("{} {}"), Version::Project, Version::SVersion);

		a_info->infoVersion = F4SE::PluginInfo::kVersion;
		a_info->name = Version::Project.data();
		a_info->version = Version::Major;

		if (a_f4se->IsEditor()) {

			logger::critical("loaded in editor"sv);

			return false;
		}

		const auto ver = a_f4se->RuntimeVersion();
		if (ver < F4SE::RUNTIME_1_10_130) {

			logger::critical(FMT_STRING("unsupported runtime v{}"sv), ver.string());

			return false;
		}

		if (a_f4se->IsEditor()) {

			logger::error("Plugin is not compatible with editor!");

			return false;
		}

		Settings::Ini::GetSingleton().ReadSettings();

		Translation::Language::GetSingleton().Load();

		Hook::D3D::Register();

		logger::info("Shadow Boost FO4 by PK0 started...");

		return true;
	}

	bool Plugin::Load(const F4SE::LoadInterface* a_f4se) noexcept
	{
		F4SE::Init(a_f4se);

		f4se_msg_interface = (F4SE::MessagingInterface*)(F4SE::GetMessagingInterface());

		if (!f4se_msg_interface) {

			logger::error("Messaging Interface error!");

			return false;
		}

		f4se_task_interface = (F4SE::TaskInterface*)(F4SE::GetTaskInterface());

		if (!f4se_task_interface) {

			logger::error("Task interface error!");

			return false;
		}

		f4se_papyrus_interface = (F4SE::PapyrusInterface*)(F4SE::GetPapyrusInterface());

		if (!f4se_papyrus_interface) {

			logger::error("Papyrus interface error!");

			return false;
		}

		if (f4se_msg_interface) {
			return f4se_msg_interface->RegisterListener(MsgCallback);
		}

		return true;
	}

	void Plugin::MsgCallback(F4SE::MessagingInterface::Message* msg) noexcept
	{
		switch (msg->type) {

		case F4SE::MessagingInterface::kGameLoaded:

			Shadow::Boost::GetSingleton().GameLoaded();

			json_preset::Preset::GetSingleton().Load();

			break;
		}
	}

	void Plugin::AddTask(F4SE::ITaskDelegate* task) noexcept
	{
		if (f4se_task_interface) {
			f4se_task_interface->AddTask(task);
		}
	}
}
