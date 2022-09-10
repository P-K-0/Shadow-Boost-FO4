#pragma once

#include "pch.h"

#include <d3d11.h>

namespace imgui_menu {

	class Menu {

	public:

		static Menu& GetInstance() noexcept { return instance; }

		[[nodiscard]] bool Init(HWND hWnd, ID3D11Device*, ID3D11DeviceContext*) noexcept;

		void Clear() noexcept;
		void Present() noexcept;

		void SetLocation(const std::uint32_t, const char*, const std::uint32_t, const char*, const bool) noexcept;

		void Show() noexcept { isShow = !isShow; }
		void DisableWaitForKey() noexcept { waitForKey = false; }
		[[nodiscard]] const bool IsShow() const noexcept { return isShow; }
		[[nodiscard]] const bool WaitForKey() const noexcept { return waitForKey; }

	private:

		Menu() {}
		~Menu() {}

		Menu(const Menu&) = delete;
		Menu(Menu&&) = delete;

		Menu& operator=(const Menu&) = delete;
		Menu& operator=(Menu&&) = delete;

		bool isShow{};
		bool waitForKey{};

		ImGuiWindowFlags flags{ 0 };

		struct Location {

			std::uint32_t uInterior{};
			std::string sInterior;
			std::uint32_t uExterior{};
			std::string sExterior;
			bool isInterior{};
		};

		Location location;

		inline [[nodiscard]] const std::uint32_t GetLocationID() const noexcept { return location.isInterior ? location.uInterior : location.uExterior; }

		ID3D11Device* device{ nullptr };
		ID3D11DeviceContext* context{ nullptr };

		static Menu instance;
	};
}