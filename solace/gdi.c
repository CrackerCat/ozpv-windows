#include "dec.h"
#include <math.h>

DWORD WINAPI Grayscale(LPVOID lParam) {
	METRICS *m = (METRICS*)lParam;
	HDC ddc = GetDC(NULL);
	HDC cdc = CreateCompatibleDC(ddc);
	RGB *rgb;
	HSL hsl;
	BITMAPINFO bi = { sizeof(BITMAPINFOHEADER), m->dw, -m->dh, 1, 32, BI_RGB };

	HBITMAP hdib = CreateDIBSection(ddc, &bi, DIB_RGB_COLORS, (void**)&rgb, NULL, 0);

	if (NULL == hdib)
		return -1;

	SelectObject(cdc, hdib);

	while (TRUE) {
		BitBlt(cdc, 0, 0, m->dw, m->dh, ddc, 0, 0, SRCCOPY);
		for (int i = 0; i < m->dw * m->dh; i++) {
			hsl = RGBtoHSL(rgb[i].Red, rgb[i].Green, rgb[i].Blue);
			hsl.Hue = 0.0f;
			hsl.Saturation = 0.0f;
			rgb[i] = HSLtoRGB(hsl.Hue, hsl.Saturation, hsl.Lightness);
		}
		BitBlt(ddc, 0, 0, m->dw, m->dh, cdc, 0, 0, SRCCOPY);

		Sleep(1000);
	}

	return 0;
}

DWORD WINAPI HueRotation(LPVOID lParam) {
	METRICS *m = (METRICS*)lParam;
	HDC ddc = GetDC(NULL);
	HDC cdc = CreateCompatibleDC(ddc);
	RGB *rgb;
	HSL hsl;
	BITMAPINFO bi = { sizeof(BITMAPINFOHEADER), m->dw, -m->dh, 1, 32, BI_RGB };

	HBITMAP hdib = CreateDIBSection(ddc, &bi, DIB_RGB_COLORS, (void**)&rgb, NULL, 0);

	if (NULL == hdib)
		return -1;

	SelectObject(cdc, hdib);

	while (TRUE) {
		BitBlt(cdc, 0, 0, m->dw, m->dh, ddc, 0, 0, SRCCOPY);
		for (int i = 0; i < m->dw * m->dh; i++) {
			hsl = RGBtoHSL(rgb[i].Red, rgb[i].Green, rgb[i].Blue);
			hsl.Hue += 6.0f;
			rgb[i] = HSLtoRGB(hsl.Hue, hsl.Saturation, hsl.Lightness);
		}
		BitBlt(ddc, 0, 0, m->dw, m->dh, cdc, 0, 0, SRCCOPY);
		Sleep(1);
	}

	return 0;
}

DWORD WINAPI ScreenMelter(LPVOID lParam) {
	METRICS *m = (METRICS*)lParam;
	HDC ddc;
	HDC cdc;
	int r;
	float d;

	while (TRUE) {
		ddc = GetDC(NULL);
		cdc = CreateCompatibleDC(ddc);

		SelectObject(cdc, CreateCompatibleBitmap(ddc, 15, m->dh));

		r = random() % (m->dw + 1);

		for (int i = 5; i < 20; i += 3) {
			for (int j = 1; j < 15; j++) {
				BitBlt(cdc, 0, 0, 20, m->dh, ddc, r, 0, SRCCOPY);
				d = asmsin((j * M_PI_F) / 15.0f) * i;
				StretchBlt(cdc, j, 0, 1, d, cdc, j, 0, 1, 1, SRCCOPY);
				BitBlt(cdc, j, 0, 1, m->dh, cdc, j, -d, SRCCOPY);
				BitBlt(ddc, r, 0, 15, m->dh, cdc, 0, 0, SRCCOPY);
				Sleep(1);
			}
		}
		DeleteDC(ddc);
		DeleteDC(cdc);

		Sleep(1);
	}

	return 0;
}

DWORD WINAPI SolitaireCards(LPVOID lParam) {
	METRICS *m = (METRICS*)lParam;
	HDC ddc = GetDC(NULL);
	HDC cdc = CreateCompatibleDC(ddc);
	BITMAP bc;

	int cardpx = random_r(0, 20) % (m->dw + 1);
	int cardpy = 0;
	int floorhits = 0;
	int energy = 2;
	BOOL falling = TRUE;
	BOOL movingright = TRUE;

	HMODULE hcd = LoadLibrary(L"cards.dll");

	if (NULL == hcd) {
		MessageBox(NULL, L"Could not find cards.dll, skipping effect.", L"solace", MB_SYSTEMMODAL | MB_OK | MB_ICONERROR);
		return 0;
	}

	HBITMAP hbc = LoadBitmap(hcd, MAKEINTRESOURCE(CARD(7)));

	GetObject(hbc, sizeof(BITMAP), &bc);

	SelectObject(cdc, hbc);

	FreeLibrary(hcd);

	BitBlt(ddc, cardpx, cardpy, m->dw, m->dh, cdc, 0, 0, SRCCOPY);

	while (TRUE) {
		if (TRUE == movingright)
			cardpx += 8;
		else
			cardpx -= 8;

		if (TRUE == falling) {
			energy++;
			cardpy += energy;
		}
		else {
			energy -= 2;
			cardpy -= energy;
		}

		if (cardpx > (m->dw - bc.bmWidth))
			movingright = FALSE;

		if (cardpx < 0)
			movingright = TRUE;

		if (cardpy > (m->dh - bc.bmHeight)) {
			falling = FALSE;
			floorhits++;
		}

		if (energy <= 0) {
			falling = TRUE;
		}

		if (floorhits > 5) {
			cardpx = random() % (m->dw + 1);
			cardpy = 0;
			floorhits = 0;
			energy = 2;
		}

		BitBlt(ddc, cardpx, cardpy, m->dw, m->dh, cdc, 0, 0, SRCCOPY);
		Sleep(1);
	}

	return 0;
}