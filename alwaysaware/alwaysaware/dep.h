#pragma once

#include <windows.h>
#include <TlHelp32.h>

typedef NTSTATUS(*PRtlAdjustPrivilege) (ULONG, BOOLEAN, BOOLEAN, PBOOLEAN);
typedef NTSTATUS(NTAPI* PNtCreateThreadEx)(OUT PHANDLE hThread, IN ACCESS_MASK DesiredAccess, IN PVOID ObjectAttributes, IN HANDLE ProcessHandle, IN PVOID lpStartAddress, IN PVOID lpParameter, IN ULONG Flags, IN SIZE_T StackZeroBits, IN SIZE_T SizeOfStackCommit, IN SIZE_T SizeOfStackReserve, OUT PVOID lpBytesBuffer);
typedef DWORD(NTAPI* PRtlCreateUserThread)(IN HANDLE hProcess, IN LPVOID lpSecurityDescriptor, IN DWORD dwCreateSuspended, IN DWORD dwStackZeroBits, IN OUT LPDWORD lpStackReserved, IN OUT LPDWORD lpStackCommit, IN LPVOID lpStartAddress, IN LPVOID lpParam, OUT LPHANDLE lpThreadHandle, OUT LPDWORD dwThreadID);

void Drop(LPCWSTR path, LPCVOID dlldata);
void Inject(LPCWSTR dllpath, LPCWSTR process);

void Exit(DWORD dwerror, UINT exitcode) {
	if (dwerror != NULL) {
		LPCWSTR error = NULL;
		if (!FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, dwerror, NULL, &error, NULL, NULL))
			ExitProcess(EXIT_FAILURE);
		MessageBoxW(NULL, error, L"Error", MB_APPLMODAL | MB_OK | MB_ICONERROR);
		LocalFree(error);
	}
	ExitProcess(exitcode);
}

DWORD GetPIDByName(LPCWSTR processname) {
	DWORD pid;
	HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	PROCESSENTRY32W processentry;
	processentry.dwSize = sizeof(PROCESSENTRY32W);
	if (!Process32FirstW(snap, &processentry))
		Exit(GetLastError(), EXIT_FAILURE);
	while (TRUE) {
		if (!Process32NextW(snap, &processentry))
			Exit(GetLastError(), EXIT_FAILURE);
		else if (!lstrcmpW(processname, processentry.szExeFile)) {
			pid = processentry.th32ProcessID;
			break;
		}
		else continue;
	}
	CloseHandle(snap);
	return pid;
}

