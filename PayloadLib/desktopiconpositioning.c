#include "dec.h"
#include <commctrl.h>

int SetDesktopIconPositions(int x, int y) {
	HWND slvwh = GetSysListView32();
	int ic = ListView_GetItemCount(slvwh);

	for (int i = 0; i < ic; i++)
		ListView_SetItemPosition(slvwh, i, x, y);

	return 0;
}

HWND GetSysListView32() {
	HWND pmwh = FindWindow(L"Progman", NULL);
	HWND sdvwh = NULL; // FindWindowEx(pmwh, NULL, L"SHELLDLL_DefView", NULL);

	if (sdvwh == NULL)
		EnumWindows(&GetSHELLDLL_DefView, (LPARAM)&sdvwh);

	return FindWindowEx(sdvwh, NULL, L"SysListView32", NULL);
}

BOOL CALLBACK GetSHELLDLL_DefView(HWND hwnd, LPARAM lParam) {
	HWND sdvwh = FindWindowEx(hwnd, NULL, L"SHELLDLL_DefView", NULL);
	
	if (sdvwh != NULL) {
		*((HWND*)lParam) = sdvwh;
		return FALSE;
	}

	return TRUE;
}