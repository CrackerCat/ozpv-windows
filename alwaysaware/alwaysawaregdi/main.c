#include <windows.h>

int random() {
	HCRYPTPROV cryptprov = NULL;
	if (cryptprov == NULL)
		CryptAcquireContext(&cryptprov, NULL, NULL, PROV_RSA_FULL, CRYPT_SILENT | CRYPT_VERIFYCONTEXT);
	int out = NULL;
	CryptGenRandom(cryptprov, sizeof(out), (BYTE*)(&out));
	return out & INT_MAX;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrev, LPSTR lpCmdLine, int nShowCmd) {

	POINT p[3];
	while (TRUE) {
		HDC hdc = GetWindowDC(GetDesktopWindow());
		RECT rect;
		GetWindowRect(GetDesktopWindow(), &rect);
		int w = rect.right - rect.left;
		int h = rect.bottom - rect.top;
		int x = random() % 100;
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