#include "dec.h"

int CustomMessageBoxIcon() {
	HHOOK hook = SetWindowsHookEx(WH_CBT, &IconHookProc, NULL, GetCurrentThreadId());
	MessageBox(NULL, L"exampletext", L"examplecaption", MB_SYSTEMMODAL | MB_YESNO | MB_ICONERROR);
	return 0;
}

LRESULT CALLBACK IconHookProc(int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode == HCBT_ACTIVATE) {
		HWND hwnd = (HWND)wParam;

		SHSTOCKICONINFO ssii;
		ssii.cbSize = sizeof(SHSTOCKICONINFO);
		SHGetStockIconInfo(SIID_AUDIOFILES, SHGSI_ICON | SHGSI_LARGEICON, &ssii);

		SendMessage(GetDlgItem(hwnd, IDICON), STM_SETICON, (WPARAM)ssii.hIcon, 0);
	}
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

int CustomMessageBoxPosition() {
	HHOOK hook = SetWindowsHookEx(WH_CBT, &PositionHookProc, NULL, GetCurrentThreadId());
	MessageBox(NULL, L"exampletext", L"examplecaption", MB_SYSTEMMODAL | MB_YESNO | MB_ICONERROR);
	return 0;
}

LRESULT CALLBACK PositionHookProc(int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode == HCBT_CREATEWND) {
		CBT_CREATEWND *cw = (CBT_CREATEWND*)lParam;

		if (cw->lpcs->style & WS_DLGFRAME || cw->lpcs->style & WS_POPUP) {
			HWND hwnd = (HWND)wParam;
			cw->lpcs->x = 0;
			cw->lpcs->y = 0;
		}
	}
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

int CustomMessageBoxButtonText() {
	HHOOK hook = SetWindowsHookEx(WH_CBT, &ButtonHookProc, NULL, GetCurrentThreadId());
	MessageBox(NULL, L"exampletext", L"examplecaption", MB_SYSTEMMODAL | MB_YESNO | MB_ICONERROR);
	return 0;
}

LRESULT CALLBACK ButtonHookProc(int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode == HCBT_ACTIVATE) {
		HWND hwnd = (HWND)wParam;

		if (GetDlgItem(hwnd, IDYES) != NULL)
			SetDlgItemText(hwnd, IDYES, L"examplebuttontext");
	}
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}