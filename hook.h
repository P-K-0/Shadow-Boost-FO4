#pragma once

#include "std_libraries.h"
#include "f4se_libraries.h"

namespace Hook {

	template<typename Func>
	inline void HookImportFunc(const char* dll, const char* nameFunc, Func& oldFn, std::uintptr_t hookFn)
	{
		std::uintptr_t thunkAddress = (std::uintptr_t)GetIATAddr((UInt8*)GetModuleHandle(NULL), dll, nameFunc);
		oldFn = (Func)*(std::uintptr_t*)thunkAddress;
		SafeWrite64(thunkAddress, hookFn);
	}

	inline std::uintptr_t HookFunc(std::uintptr_t* vtbl, int index, std::uintptr_t hookFn, std::uintptr_t* oldFn)
	{
		std::uintptr_t returnAddy = vtbl[index];
		*oldFn = returnAddy;

		SafeWrite64((std::uintptr_t)(vtbl + index), hookFn);

		return returnAddy;
	}
}
