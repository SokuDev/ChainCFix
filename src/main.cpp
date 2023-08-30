//
// Created by PinkySmile on 31/10/2020
//

#include <SokuLib.hpp>

void __fastcall ObjectHandler_SpawnBullet(void *This, unsigned _, unsigned p1, unsigned p2, unsigned p3, unsigned p4, unsigned p5, void *data, unsigned size)
{
	float *data2 = SokuLib::New<float>(size + 1);

	memcpy(data2, data, size * sizeof(float));
	data2[size] = 1;
	((void (__thiscall *)(void *, unsigned, unsigned, unsigned, unsigned, unsigned, void *, unsigned))0x46eb30)(This, p1, p2, p3, p4, p5, data2, size + 1);
	SokuLib::Delete(data2);
}

// We check if the game version is what we target (in our case, Soku 1.10a).
extern "C" __declspec(dllexport) bool CheckVersion(const BYTE hash[16])
{
	return memcmp(hash, SokuLib::targetHash, sizeof(SokuLib::targetHash)) == 0;
}

// Called when the mod loader is ready to initialize this module.
// All hooks should be placed here. It's also a good moment to load settings from the ini.
extern "C" __declspec(dllexport) bool Initialize(HMODULE hMyModule, HMODULE hParentModule)
{
	DWORD old;

#ifdef _DEBUG
	FILE *_;

	AllocConsole();
	freopen_s(&_, "CONOUT$", "w", stdout);
	freopen_s(&_, "CONOUT$", "w", stderr);
#endif

	VirtualProtect((PVOID)TEXT_SECTION_OFFSET, TEXT_SECTION_SIZE, PAGE_EXECUTE_WRITECOPY, &old);
	SokuLib::TamperNearJmpOpr(0x590486, ObjectHandler_SpawnBullet);
	SokuLib::TamperNearJmpOpr(0x590C4C, ObjectHandler_SpawnBullet);
	SokuLib::TamperNearJmpOpr(0x590683, ObjectHandler_SpawnBullet);
	SokuLib::TamperNearJmpOpr(0x5906ED, ObjectHandler_SpawnBullet);
	VirtualProtect((PVOID)TEXT_SECTION_OFFSET, TEXT_SECTION_SIZE, old, &old);

	FlushInstructionCache(GetCurrentProcess(), nullptr, 0);
	return true;
}

extern "C" int APIENTRY DllMain(HMODULE hModule, DWORD fdwReason, LPVOID lpReserved)
{
	return TRUE;
}

// New mod loader functions
// Loading priority. Mods are loaded in order by ascending level of priority (the highest first).
// When 2 mods define the same loading priority the loading order is undefined.
extern "C" __declspec(dllexport) int getPriority()
{
	return 0;
}

// Not yet implemented in the mod loader, subject to change
// SokuModLoader::IValue **getConfig();
// void freeConfig(SokuModLoader::IValue **v);
// bool commitConfig(SokuModLoader::IValue *);
// const char *getFailureReason();
// bool hasChainedHooks();
// void unHook();