
#include "hook.h"

//#include <Windows.h>

namespace Hook {

	// source F4SE 
	//void* GetIATAddr(void* _module, const char* searchDllName, const char* searchImportName)
	//{
	//	unsigned char* base = (unsigned char*)_module;
	//	IMAGE_DOS_HEADER* dosHeader = (IMAGE_DOS_HEADER*)base;
	//	IMAGE_NT_HEADERS* ntHeader = (IMAGE_NT_HEADERS*)(base + dosHeader->e_lfanew);
	//	IMAGE_IMPORT_DESCRIPTOR* importTable =
	//		(IMAGE_IMPORT_DESCRIPTOR*)(base + ntHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);

	//	for (; importTable->Characteristics; ++importTable)
	//	{
	//		const char* dllName = (const char*)(base + importTable->Name);

	//		if (!_stricmp(dllName, searchDllName))
	//		{
	//			// found the dll

	//			IMAGE_THUNK_DATA* thunkData = (IMAGE_THUNK_DATA*)(base + importTable->OriginalFirstThunk);
	//			uintptr_t* iat = (uintptr_t*)(base + importTable->FirstThunk);

	//			for (; thunkData->u1.Ordinal; ++thunkData, ++iat)
	//			{
	//				if (!IMAGE_SNAP_BY_ORDINAL(thunkData->u1.Ordinal))
	//				{
	//					IMAGE_IMPORT_BY_NAME* importInfo = (IMAGE_IMPORT_BY_NAME*)(base + thunkData->u1.AddressOfData);

	//					if (!_stricmp((char*)importInfo->Name, searchImportName))
	//					{
	//						// found the import
	//						return iat;
	//					}
	//				}
	//			}

	//			return NULL;
	//		}
	//	}

	//	return NULL;
	//}
}