#include "dep.h"
#include "data.h"

int Main() {
	//Drop(DLL_PATH, DLL_DATA);
	Inject(DLL_PATH, PROCESS_NAME);
}

void Drop(LPCWSTR path, LPCVOID dlldata) {
	DWORD dwDump = NULL;
	HANDLE drop = CreateFileW(path, FILE_GENERIC_WRITE, NULL, NULL, CREATE_NEW, NULL, NULL);
	WriteFile(drop, dlldata, sizeof(dlldata), &dwDump, NULL);
	CloseHandle(drop);
}

void Inject(LPCWSTR dllpath, LPCWSTR processname) {
	DWORD dwDump = NULL, dwPID = GetPIDByName(processname);
	PRtlAdjustPrivilege RtlAdjustPrivilege = GetProcAddress(GetModuleHandle(L"ntdll.dll"), "RtlAdjustPrivilege");
	RtlAdjustPrivilege(20, 1, NULL, &dwDump);
	HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID);
	if (!processname)
		Exit(GetLastError(), EXIT_FAILURE);
	LPVOID baselocation = VirtualAllocEx(process, NULL, sizeof(dllpath), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	if (!baselocation)
		Exit(GetLastError(), EXIT_FAILURE);
	if (!WriteProcessMemory(process, baselocation, dllpath, sizeof(dllpath), NULL))
		Exit(GetLastError(), EXIT_FAILURE);
    HANDLE createthread = CreateRemoteThread(process, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLibraryA, baselocation, NULL, NULL);
	if (!createthread) {
		createthread = NULL;
		PNtCreateThreadEx NtCreateThreadEx = GetProcAddress(GetModuleHandle(L"ntdll.dll"), "NtCreateThreadEx");
		PRtlCreateUserThread RtlCreateUserThread = GetProcAddress(GetModuleHandle(L"ntdll.dll"), "RtlCreateUserThread");
		if (!NtCreateThreadEx(&createthread, 0x1FFFFF, NULL, process, (LPTHREAD_START_ROUTINE)LoadLibraryA, baselocation, FALSE, NULL, NULL, NULL, NULL))
			createthread = NULL;
		else if (!RtlCreateUserThread(process, NULL, FALSE, NULL, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLibraryA, baselocation, &createthread, NULL))
			Exit(GetLastError(), EXIT_FAILURE);
	}
	if (WaitForSingleObject(createthread, INFINITE) == WAIT_FAILED)
		Exit(GetLastError(), EXIT_FAILURE);
	MessageBoxW(NULL, L"Success", L"Success", MB_APPLMODAL | MB_OK);
	Exit(NULL, EXIT_SUCCESS);
}