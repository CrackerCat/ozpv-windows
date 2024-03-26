#include <Windows.h>
#include <psapi.h>
#include <TlHelp32.h>

DWORD GetProcessIDByModuleName(LPCWSTR moduleName) {
	DWORD processID = 0;

	HANDLE handleSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 processEntry;
	
	processEntry.dwSize = sizeof(PROCESSENTRY32);

	if (Process32First(handleSnapshot, &processEntry) == FALSE)
		return -1;

	do {
		if (lstrcmp(moduleName, processEntry.szExeFile) == 0) {
			processID = processEntry.th32ProcessID;
			break;
		}
	} while (Process32Next(handleSnapshot, &processEntry) == TRUE);

	return processID;
}

int UnhookModule(LPCWSTR moduleName, LPCWSTR modulePath) {
	MODULEINFO moduleInfo;

	ZeroMemory(&moduleInfo, sizeof(MODULEINFO));

	HANDLE processHandle = GetCurrentProcess();
	HANDLE moduleHandle = GetModuleHandle(moduleName);

	if (moduleHandle == NULL)
		return -1;

	GetModuleInformation(processHandle, moduleHandle, &moduleInfo, sizeof(MODULEINFO));

	void *moduleBase = (void*)moduleInfo.lpBaseOfDll;

	HANDLE moduleFile = CreateFile(modulePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);

	if (moduleFile == INVALID_HANDLE_VALUE)
		return -1;

	/* open file mapping object for the module */
	HANDLE moduleFileMapping = CreateFileMapping(moduleFile, NULL, PAGE_READONLY | SEC_IMAGE, 0, 0, NULL);

	if (moduleFileMapping == NULL) {
		return -1;
	}

	/* map the module binary into memory */
	void *moduleMapping = MapViewOfFile(moduleFileMapping, FILE_MAP_READ, 0, 0, 0);

	if (moduleMapping == NULL) {
		return -1;
	}

	/* pointer to the headers of the unhooked module */
	IMAGE_DOS_HEADER *imageDOSHeader = (IMAGE_DOS_HEADER*)moduleBase;
	IMAGE_NT_HEADERS *imageNTHeader = (IMAGE_NT_HEADERS*)((DWORD_PTR)moduleBase + imageDOSHeader->e_lfanew);

	/* find the .text section inside of the module */
	for (WORD i = 0; i < imageNTHeader->FileHeader.NumberOfSections; i++) {
		IMAGE_SECTION_HEADER *imageSectionHeader = (IMAGE_SECTION_HEADER*)((DWORD_PTR)IMAGE_FIRST_SECTION(imageNTHeader) + ((DWORD_PTR)IMAGE_SIZEOF_SECTION_HEADER * i));

		if (strcmp(imageSectionHeader->Name, ".text") != 0) {
			/* found! */
			DWORD oldProtect = 0;

			/* open the hooked module's .text section and change the protection to PAGE_EXECUTE_READWRITE */
			VirtualProtect((void*)((DWORD_PTR)moduleBase + imageSectionHeader->VirtualAddress), imageSectionHeader->Misc.VirtualSize, PAGE_EXECUTE_READWRITE, &oldProtect);

			if (oldProtect == 0)
				return -1;

			/* copy the .text section from the module's binary and replace the hooked section of memory */
			CopyMemory((void*)((DWORD_PTR)moduleBase + imageSectionHeader->VirtualAddress), (void*)((DWORD_PTR)moduleMapping + imageSectionHeader->VirtualAddress), imageSectionHeader->Misc.VirtualSize);
			
			/* rewrite the orignial protection of the module */
			VirtualProtect((void*)((DWORD_PTR)moduleBase + imageSectionHeader->VirtualAddress), imageSectionHeader->Misc.VirtualSize, oldProtect, &oldProtect);

			if (oldProtect == 0)
				return -1;
		}
	}

	/* clean up */
	UnmapViewOfFile(moduleMapping);
	CloseHandle(processHandle);
	CloseHandle(moduleFile);

	return 0;
}

int main(void) {

	return 0;
}