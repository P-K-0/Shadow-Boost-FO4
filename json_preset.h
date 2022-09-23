#pragma once

#include "pch.h"

#include "settings.h"

namespace json_preset {

	static const char* PresetFile = "Data\\F4SE\\Plugins\\ShadowBoostFO4_Preset.json";

	constexpr std::uint32_t LightPlugin = 0xfe000000;
	constexpr std::uint32_t NormalPlugin = 0xff000000;
	constexpr std::uint64_t Interior = 0x100000000;
	constexpr std::uint32_t EslFlag = 1 << 9;

	class InfoPlugin {

	public:

		InfoPlugin(const std::uint64_t id) noexcept;
		InfoPlugin(const RE::TESFile* file, const std::string& key) noexcept;

		~InfoPlugin() noexcept {}

		InfoPlugin& operator=(const InfoPlugin&) = delete;
		InfoPlugin& operator=(InfoPlugin&&) = delete;

		InfoPlugin(const InfoPlugin&) = delete;
		InfoPlugin(InfoPlugin&&) = delete;

		[[nodiscard]] const std::uint32_t& GetIndex() const noexcept { return _index; }
		[[nodiscard]] const std::uint32_t& GetFormID() const noexcept { return _formId; }
		[[nodiscard]] const std::uint64_t& GetID() const noexcept { return _id; }
		[[nodiscard]] const std::uint8_t& IsInterior() const noexcept { return _interior; }
		[[nodiscard]] const bool& IsLight() const noexcept { return _light; }
		[[nodiscard]] const std::string& GetKey() const noexcept { return _key; }
	
	private:

		std::uint32_t _index{};
		std::uint32_t _formId{};
		std::uint8_t _interior{};
		std::uint64_t _id{};
		bool _light{};
		std::string _key;
	};

	class Preset {

	public:

		using Values = Settings::Values;

		[[nodiscard]] static Preset& GetInstance() noexcept { return instance; }

		void Load() noexcept;
		void Save() noexcept;

		void Add(const std::uint32_t id, const bool isInterior) noexcept;
		void Remove(const std::uint32_t id, const bool isInterior) noexcept;
		void Get(const std::uint32_t id, const bool isInterior) noexcept;
		void InitDefaultValues() noexcept { defValues = Settings::Ini::GetInstance().GetValues(); }

		[[nodiscard]] const bool HasPreset() const noexcept { return hasPreset; }

	private:

		Preset() noexcept {}
		~Preset() noexcept {}
	
		Preset(const Preset&) = delete;
		Preset(Preset&&) = delete;

		Preset& operator=(const Preset&) = delete;
		Preset& operator=(Preset&&) = delete;

		inline [[nodiscard]] std::uint64_t AsID(const std::uint32_t id, const bool isInterior) noexcept { return (isInterior ? Interior : 0) | id; }

		std::unordered_map<std::uint64_t, Values> map;

		bool hasPreset{};

		Settings::Values defValues{};

		static Preset instance;
	};
}
