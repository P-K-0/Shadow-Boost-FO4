#pragma once

#include "pch.h"

#include "shadow.h"
#include "settings.h"
#include "json_preset.h"
#include "translation.h"

#include <d3d11.h>

namespace imgui_menu {

	class HotKey {

	public:

		static const std::string GetNameKey(std::uint16_t iKey) noexcept
		{
			char sKey[255]{};

			std::uint32_t scanCode = MapVirtualKey(iKey, MAPVK_VK_TO_VSC);
			scanCode <<= 16;

			GetKeyNameText(scanCode, sKey, sizeof sKey);

			return sKey;
		}

		static const std::string GetNameKey(bool wait, std::uint16_t iKey, std::uint16_t iExtendedKey) noexcept
		{
			std::string sKey = Translation::Language::GetSingleton()[Translation::LangID::WaitForNewKey];

			if (!wait) {

				sKey.clear();

				if (iExtendedKey != 0)
					sKey = GetNameKey(iExtendedKey) + "+";

				sKey += GetNameKey(iKey);
			}

			return sKey;
		}
	};

	class Wnd {

	public:

		Wnd() noexcept {}
		~Wnd() noexcept {}

		Wnd(Wnd&&) = delete;
		Wnd(const Wnd&) = delete;
		
		Wnd& operator=(Wnd&&) = delete;
		Wnd& operator=(const Wnd&) = delete;

		template<typename Fn = std::function<void()>>
		void Create(const char* name, Fn fn, bool onTop = false, ImGuiWindowFlags flags = 0, float fontScale = 1.0f) noexcept;

		virtual void OnClosed() noexcept {}
		virtual void OnKeyUp(std::uint16_t vkCode, bool isExtended) noexcept {}
		virtual void OnKeyDown(std::uint16_t vkCode, bool isExtended, bool wasKeyDown) noexcept {}

		virtual void Show(bool show) noexcept { isShow = show; }
		virtual void Show() noexcept { isShow = !isShow; }

		virtual [[nodiscard]] const bool& IsShow() const noexcept { return isShow; }
		virtual [[nodiscard]] const bool& IsCursorEnabled() const noexcept { return isCursorEnabled; }

	protected:

		void EnableCursor(bool show) noexcept;

		bool isShow{};
		bool isCursorEnabled{};
	};

	class Menu : 
		public Wnd {

	public:

		[[nodiscard]] static Menu& GetSingleton() noexcept { return instance; }

		[[nodiscard]] bool Init(HWND hWnd, ID3D11Device*, ID3D11DeviceContext*) noexcept;

		void Render() noexcept;

		virtual void Show() noexcept;

		void SetLocation(const std::uint32_t, const char*, const std::uint32_t, const char*, const bool) noexcept;

		virtual void OnClosed() noexcept { EnableCursor(false); }
		virtual void OnKeyUp(std::uint16_t vkCode, bool isExtended) noexcept;
		virtual void OnKeyDown(std::uint16_t vkCode, bool isExtended, bool wasKeyDown) noexcept;

	private:

		Menu() noexcept {}
		~Menu() noexcept {}

		Menu(const Menu&) = delete;
		Menu(Menu&&) = delete;

		Menu& operator=(const Menu&) = delete;
		Menu& operator=(Menu&&) = delete;

		Wnd wndSettings;
		Wnd wndEditor;
		Wnd wndHelp;
		Wnd wndAbout;

		bool waitForKeyOpen{};
		bool waitForKeyStop{};

		std::uint16_t vkExtended{};

		void SetMenuStyle() noexcept;

		struct Location {

			std::uint32_t uInterior{};
			std::string sInterior;
			std::uint32_t uExterior{};
			std::string sExterior;
			bool isInterior{};
		};

		Location location;

		inline [[nodiscard]] const std::uint32_t GetLocationID() const noexcept { return location.isInterior ? location.uInterior : location.uExterior; }

		static Menu instance;
	};
}