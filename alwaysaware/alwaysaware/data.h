#pragma once

LPCWSTR PROCESS_NAME = L"explorer.exe";
LPCWSTR DLL_PATH = L"C:\\Windows\\AlwaysAwareEx.dll";

#define DLL_X86 0x00
#define DLL_X64 0x00

#ifdef _WIN64
#define DLL_DATA DLL_X64
#else
#define DLL_DATA DLL_X86
#endif