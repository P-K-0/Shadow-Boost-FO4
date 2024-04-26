
#include "json_preset.h"

#include "json/json.h"

namespace json_preset {

	template<typename T>
	inline char* as_bytes(T& t)
	{
		void* addr = &t;

		return static_cast<char*>(addr);
	}

	std::ifstream& operator>>(std::ifstream& ifs, char& val)
	{
		ifs.read(as_bytes(val), sizeof(val));

		return ifs;
	}

	InfoPlugin::InfoPlugin(const std::uint64_t id) noexcept
		: _id { id }
	{
		if ((id & NormalPlugin) == LightPlugin) {

			_index = static_cast<std::uint32_t>((id >> 12) & 0xfffff);
			_formId = static_cast<std::uint32_t>(id & 0xfff);
			_light = true;
		}
		else {

			_index = static_cast<std::uint32_t>(id >> 24) & 0xff;
			_formId = static_cast<std::uint32_t>(id & 0xffffff);
			_light = false;
		}

		_interior = static_cast<std::uint8_t>((id & Interior) >> 32);

		_key = std::to_string(_interior) + ":" + std::to_string(_formId);

		if (_index == 0xff)
			_formId = 0;
	}

	InfoPlugin::InfoPlugin(const RE::TESFile* file, const std::string& key) noexcept
		: _key{ key }
	{
		std::regex reg{ R"((\d*):(\d*))"};
		std::smatch matches;

		if (!std::regex_search(key, matches, reg))
			return;

		if (matches.size() < 3)
			return;

		_interior = static_cast<std::uint8_t>(std::stoi(matches[1].str()));
		_formId = static_cast<std::uint32_t>(std::stoul(matches[2].str()));

		if (file->flags & EslFlag) {

			_index = 0xfe000 | file->GetSmallFileCompileIndex();
			_light = true;
		}
		else {

			_index = file->GetCompileIndex();
			_light = false;
		}

		if (_interior)
			_id = Interior;

		if (_light) {

			_id |= (_index << 12) | _formId;
		}
		else {

			_id |= (_index << 24) | _formId;
		}
	}

	void Preset::Load() noexcept 
	{ 	
		InitDefaultValues();

		auto DataHandler = RE::TESDataHandler::GetSingleton();
	 
		if (!DataHandler)
			return;

		Json::Reader jReader;
		Json::Value root;

		std::string buff;

		std::ifstream fJson{ PresetFile, std::ios_base::binary };

		for (char c; fJson >> c; )
			buff += c;

		if (!jReader.parse(buff, root))
			return;	

		for (auto& root_m : root.getMemberNames()) {

			for (auto& m : root[root_m.c_str()].getMemberNames()) {

				//logger::info("{} {} ", root_m.c_str(), m.c_str());

				std::uint64_t id{};

				for (auto& f : DataHandler->files) {

					if (_strcmpi(f->GetFilename().data(), root_m.c_str()) == 0) {

						InfoPlugin info{ f, m.c_str() };

						id = info.GetID();

						//logger::info("{}", id);

						break;
					}
				}

				if (id == 0)
					continue;

				auto& _map = map[id];
				auto& rr = root[root_m.c_str()][m.c_str()];

				_map.fMinDistance = rr["fMinDistance"].asFloat();
				_map.fMaxDistance = rr["fMaxDistance"].asFloat();

				_map.fShadowFactor = rr["fDynamicValueFactor"].asFloat();

				_map.fMinDistObjects = rr["fLODFadeOutMultObjectsMin"].asFloat();
				_map.fMaxDistObjects = rr["fLODFadeOutMultObjectsMax"].asFloat();

				_map.fMinDistItems = rr["fLODFadeOutMultItemsMin"].asFloat();
				_map.fMaxDistItems = rr["fLODFadeOutMultItemsMax"].asFloat();

				_map.fMinDistActors = rr["fLODFadeOutMultActorsMin"].asFloat();
				_map.fMaxDistActors = rr["fLODFadeOutMultActorsMax"].asFloat();

				_map.fLodFactor = rr["fLodDynamicValueFactor"].asFloat();

				_map.fMinDistGrass = rr["fGrassStartFadeDistanceMin"].asFloat();
				_map.fMaxDistGrass = rr["fGrassStartFadeDistanceMax"].asFloat();

				_map.fGrassFactor = rr["fGrassDynamicValueFactor"].asFloat();

				_map.blockLevel[0].fDistBlockLevel2 = rr["fBlockLevel2Distance"].asFloat();
				_map.blockLevel[0].fDistBlockLevel1 = rr["fBlockLevel1Distance"].asFloat();
				_map.blockLevel[0].fDistBlockLevel0 = rr["fBlockLevel0Distance"].asFloat();

				_map.blockLevel[1].fDistBlockLevel2 = rr["fBlockLevel2DistanceLevel1"].asFloat();
				_map.blockLevel[1].fDistBlockLevel1 = rr["fBlockLevel1DistanceLevel1"].asFloat();
				_map.blockLevel[1].fDistBlockLevel0 = rr["fBlockLevel0DistanceLevel1"].asFloat();

				_map.blockLevel[2].fDistBlockLevel2 = rr["fBlockLevel2DistanceLevel2"].asFloat();
				_map.blockLevel[2].fDistBlockLevel1 = rr["fBlockLevel1DistanceLevel2"].asFloat();
				_map.blockLevel[2].fDistBlockLevel0 = rr["fBlockLevel0DistanceLevel2"].asFloat();

				_map.blockLevel[3].fDistBlockLevel2 = rr["fBlockLevel2DistanceLevel3"].asFloat();
				_map.blockLevel[3].fDistBlockLevel1 = rr["fBlockLevel1DistanceLevel3"].asFloat();
				_map.blockLevel[3].fDistBlockLevel0 = rr["fBlockLevel0DistanceLevel3"].asFloat();
			}
		}
	}

	void Preset::Save() noexcept
	{
		auto DataHandler = RE::TESDataHandler::GetSingleton();

		if (!DataHandler)
			return;

		Json::Value root;

		for (auto& m : map) {

			InfoPlugin info{ m.first };

			if (info.GetFormID() == 0)
				continue;

			//if (info.GetIndex() == 0xff || info.GetID() == 0x0)
			//	continue;

			for (auto f : DataHandler->files) {

				if (info.IsLight() && f->GetSmallFileCompileIndex() != info.GetIndex())
					continue;

				if (!info.IsLight() && f->GetCompileIndex() != info.GetIndex())
					continue;

				auto& r = root[f->GetFilename().data()][info.GetKey().c_str()];

				r["fMinDistance"] = m.second.fMinDistance;
				r["fMaxDistance"] = m.second.fMaxDistance;

				r["fDynamicValueFactor"] = m.second.fShadowFactor;

				r["fLODFadeOutMultObjectsMin"] = m.second.fMinDistObjects;
				r["fLODFadeOutMultObjectsMax"] = m.second.fMaxDistObjects;

				r["fLODFadeOutMultItemsMin"] = m.second.fMinDistItems;
				r["fLODFadeOutMultItemsMax"] = m.second.fMaxDistItems;

				r["fLODFadeOutMultActorsMin"] = m.second.fMinDistActors;
				r["fLODFadeOutMultActorsMax"] = m.second.fMaxDistActors;

				r["fLodDynamicValueFactor"] = m.second.fLodFactor;

				r["fGrassStartFadeDistanceMin"] = m.second.fMinDistGrass;
				r["fGrassStartFadeDistanceMax"] = m.second.fMaxDistGrass;

				r["fGrassDynamicValueFactor"] = m.second.fGrassFactor;

				r["fBlockLevel2Distance"] = m.second.blockLevel[0].fDistBlockLevel2;
				r["fBlockLevel1Distance"] = m.second.blockLevel[0].fDistBlockLevel1;
				r["fBlockLevel0Distance"] = m.second.blockLevel[0].fDistBlockLevel0;

				r["fBlockLevel2DistanceLevel1"] = m.second.blockLevel[1].fDistBlockLevel2;
				r["fBlockLevel1DistanceLevel1"] = m.second.blockLevel[1].fDistBlockLevel1;
				r["fBlockLevel0DistanceLevel1"] = m.second.blockLevel[1].fDistBlockLevel0;

				r["fBlockLevel2DistanceLevel2"] = m.second.blockLevel[2].fDistBlockLevel2;
				r["fBlockLevel1DistanceLevel2"] = m.second.blockLevel[2].fDistBlockLevel1;
				r["fBlockLevel0DistanceLevel2"] = m.second.blockLevel[2].fDistBlockLevel0;

				r["fBlockLevel2DistanceLevel3"] = m.second.blockLevel[3].fDistBlockLevel2;
				r["fBlockLevel1DistanceLevel3"] = m.second.blockLevel[3].fDistBlockLevel1;
				r["fBlockLevel0DistanceLevel3"] = m.second.blockLevel[3].fDistBlockLevel0;

				break;
			}
		}

		std::ofstream fJson{ PresetFile, std::ios_base::trunc };

		fJson << root << std::endl;
	}

	void Preset::Get(const std::uint32_t id, const bool isInterior) noexcept
	{
		const auto& it = map.find(AsID(id, isInterior));

		auto& settings = Settings::Ini::GetSingleton();

		if (it != map.end()) {

			hasPreset = true;

			settings.SetValues((*it).second);
			
			return;
		}

		hasPreset = false;

		settings.SetValues(defValues);
	}

	void Preset::Add(const std::uint32_t id, const bool isInterior) noexcept
	{
		hasPreset = true;

		map[AsID(id, isInterior)] = Settings::Ini::GetSingleton().GetValues();
	}

	void Preset::Remove(const std::uint32_t id, const bool isInterior) noexcept
	{
		hasPreset = false;

		map.erase(AsID(id, isInterior));
	}

	Preset Preset::instance;
}
