#pragma once

#pragma warning(push)
#include "F4SE/F4SE.h"
#include "RE/Fallout.h"
#include "RE/Bethesda/PlayerCharacter.h"
#include "RE/Bethesda/Events.h"
#include "RE/Bethesda/UI.h"
#include "RE/Bethesda/TESDataHandler.h"

#ifdef NDEBUG
#include <spdlog/sinks/basic_file_sink.h>
#else
#include <spdlog/sinks/msvc_sink.h>
#endif
#pragma warning(pop)

#define DLLEXPORT __declspec(dllexport)

namespace logger = F4SE::log;

using namespace std::literals;

#include "Version.h"

#include "imgui.h"

#include "IMGUI/imgui_impl_dx11.h"
#include "IMGUI/imgui_impl_win32.h"

#include "std_libraries.h"
