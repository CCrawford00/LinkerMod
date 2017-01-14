#include "stdafx.h"

const void *Com_GetBspLump(LumpType type, unsigned int elemSize, unsigned int *count)
{
	return ((void *(__cdecl *)(LumpType, unsigned int, unsigned int *))0x00560F80)(type, elemSize, count);
}

unsigned int Com_GetBspVersion()
{
	return ((unsigned int(__cdecl *)())0x004D07E0)();
}

void Com_LoadBsp(const char *filename)
{
	((void(__cdecl *)(const char *filename))0x00474BB0)(filename);
}

void Com_SaveLump(LumpType type, const void *newLump, unsigned int size, ComSaveLumpBehavior behavior)
{
	((void(__cdecl *)(LumpType, const void *, unsigned int, ComSaveLumpBehavior))0x004E1250)(type, newLump, size, behavior);
}

const char *Com_GetHunkStringCopy(const char *string)
{
	size_t len = strlen(string);
	void *hunkCopy = Hunk_AllocAlign(len + 1, 1, "Com_GetHunkStringCopy", 14);

	return (const char *)memcpy(hunkCopy, string, len + 1);
}