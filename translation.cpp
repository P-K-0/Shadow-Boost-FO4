
#include "translation.h"

#include "SimpleIni.h"

namespace Translation {

	std::wstring Language::GetFallout4ini() noexcept
	{
		wchar_t* buffer{ nullptr };
		const auto ret = SHGetKnownFolderPath(FOLDERID_Documents, KNOWN_FOLDER_FLAG::KF_FLAG_DEFAULT, nullptr, std::addressof(buffer));
		std::unique_ptr<wchar_t[], decltype(&CoTaskMemFree)> knownPath(buffer, CoTaskMemFree);

		if (!knownPath || ret != S_OK)
			return std::wstring();

		std::filesystem::path p = knownPath.get();
		p /= "My Games/Fallout4/Fallout4.ini"sv;
		return p.c_str();
	}

	const std::string& Language::GetFallout4lang() noexcept
	{
		auto fileIni = GetFallout4ini();

		if (fileIni.empty())
			return sLang;

		CSimpleIni ini;
		ini.SetUnicode();
		SI_Error err = ini.LoadFile(fileIni.c_str());

		if (err != 0)
			return sLang;

		sLang = ini.GetValue("General", "sLanguage", DefaultLang);

		return sLang;
	}

	void Language::Load() noexcept
	{
		std::string dir = TranslationsDir;
		dir += GetFallout4lang() + "\\" + TranslationFile;

		logger::info("{} ", dir);

		std::ifstream ifs{ dir, std::ios_base::binary };

		if (!ifs)
			return;

		std::string str;
		std::string item;
		std::uint32_t index{};
		std::size_t found{};

		while (!ifs.eof()) {

			std::getline(ifs, str, '\r');

			found = str.find("\t");

			if (found != std::string::npos && found > 0) {

				index = std::stoi(str.substr(0, found));

				item = str.substr(found + 1);

				std::replace(item.begin(), item.end(), '\\', '\n');

				logger::info("{} {}", index, item);
				
				if (index < vec.size())
					vec[index] = item;
			}
		}
	}

	const ImWchar* Language::SelectGlyph(ImGuiIO& io, const LangGlyph& glyph) noexcept
	{
		switch (glyph) {

		case LangGlyph::Cyrillic:

			return io.Fonts->GetGlyphRangesCyrillic();

		case LangGlyph::Japanese:

			return io.Fonts->GetGlyphRangesJapanese();

		case LangGlyph::Korean:

			return io.Fonts->GetGlyphRangesKorean();

		case LangGlyph::ChineseFull:

			return io.Fonts->GetGlyphRangesChineseFull();

		case LangGlyph::ChineseSimplifiedCommon:

			return io.Fonts->GetGlyphRangesChineseSimplifiedCommon();

		case LangGlyph::Thai:

			return io.Fonts->GetGlyphRangesThai();

		case LangGlyph::Vietnamese:

			return io.Fonts->GetGlyphRangesVietnamese();
		}

		return io.Fonts->GetGlyphRangesDefault();
	}

	void Language::SetGlyph(ImGuiIO& io) noexcept
	{
		std::string dir = TranslationsDir;
		dir += sLang + "\\font.ttf";

		if (std::filesystem::exists(std::filesystem::path(dir))) {

			logger::info("{}"sv, dir);
	
			LangGlyph glyph{ LangGlyph::Default };

			for (auto& v : vecLang) 
				if (_strcmpi(v.second.c_str(), sLang.c_str()) == 0) {

					glyph = v.first;
					break;	
				}

			io.Fonts->AddFontFromFileTTF(dir.c_str(), 13, nullptr, SelectGlyph(io, glyph));
		}
	}

	Language Language::instance;
}
