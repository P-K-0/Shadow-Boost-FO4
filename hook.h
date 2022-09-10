#pragma once

#include "pch.h"
#include "std_libraries.h"
#include "f4se_libraries.h"

namespace Hook {

	inline void safe_write(std::uintptr_t addr, std::uintptr_t data)
	{
		REL::safe_write(addr, &data, sizeof(data));
	}

	template<typename Func>
	inline void HookFunc(REL::ID id, Func& oldFn, std::uintptr_t hookFn)
	{
		oldFn = (Func)*(std::uintptr_t*)id.address();

		safe_write(id.address(), hookFn);
	}

	inline void HookFunc(std::uintptr_t* vtbl, int index, std::uintptr_t hookFn, std::uintptr_t* oldFn)
	{
		*oldFn = (std::uintptr_t)(vtbl[index]);

		safe_write((std::uintptr_t)(vtbl + index), hookFn);
	}
}
