#include "dec.h"
#include <commctrl.h>
#include <math.h>

DWORD WINAPI Ebola(LPVOID lParam) {
	METRICS *m = (METRICS*)lParam;
	MSG msg;

	WNDCLASS wc = { 0, (WNDPROC)EbolaProc, 0, 0, GetModuleHandle(NULL), NULL, LoadCursor(NULL, IDC_ARROW), NULL, NULL, L"Ebola" };
	RegisterClass(&wc);
	CreateWindowEx(WS_EX_TOPMOST | WS_EX_TOOLWINDOW, wc.lpszClassName, NULL, WS_POPUP, 0, 0, m->dw, m->dh, HWND_DESKTOP, NULL, wc.hInstance, NULL);

	while (GetMessage(&msg, NULL, 0, 0)) {
		DispatchMessage(&msg);
	}
	return 0;
}

LRESULT CALLBACK EbolaProc(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
	if (Msg == WM_CREATE) {
		HDC wdc = GetDC(hwnd);
		RECT whd;

		GetWindowRect(hwnd, &whd);
		ShowWindow(hwnd, SW_SHOW);

		PlaySound(L"SystemHand", GetModuleHandle(NULL), SND_ASYNC);
		FillRect(wdc, &whd, CreateSolidBrush(RGB(0, 0, 0)));
	}
	return DefWindowProc(hwnd, Msg, wParam, lParam);
}

DWORD WINAPI IconWorm(LPVOID lParam) {
	METRICS *m = (METRICS*)lParam;
	HDC ddc = GetDC(m->dwh);

	double y = 0;
	double r = random() % (m->dh + 1);

	for (double i = 0.0;; i += 0.1) {
		DrawIcon(ddc, i * 50, (y * 50), LoadIcon(NULL, IDI_ERROR));
		y = sin(i);

		if ((i * 50) > m->dw) {
			i = 0.0;
			r = random() % (m->dh + 1);
		}
		Sleep(1);
	}
}

DWORD WINAPI Grayscale(LPVOID lParam) {
	METRICS *m = (METRICS*)lParam;
	HDC ddc = GetDC(m->dwh);
	HDC cdc = CreateCompatibleDC(ddc);
	RGBQUAD rq[256];
	BITMAPINFO bi = { sizeof(BITMAPINFOHEADER), m->dw, -m->dh, 1, 8, BI_RGB };

	for (int i = 0; i < 256; i++) {
		rq[i].rgbRed = i;
		rq[i].rgbGreen = i;
		rq[i].rgbBlue = i;
		rq[i].rgbReserved = 0;
	}

	HBITMAP hdib = CreateDIBSection(ddc, &bi, DIB_RGB_COLORS, (void**)&rq, NULL, 0);

	SelectObject(cdc, hdib);

	while (TRUE) {
		BitBlt(cdc, 0, 0, m->dw, m->dh, ddc, 0, 0, SRCCOPY);
		BitBlt(ddc, 0, 0, m->dw, m->dh, cdc, 0, 0, SRCCOPY);

		Sleep(1000);
	}
}

DWORD WINAPI IncInvert(LPVOID lParam) {
	METRICS *m = (METRICS*)lParam;
	HDC ddc = GetDC(m->dwh);
	
	for (int i = 0; i < m->dh; i++) {
		BitBlt(ddc, 0, i, m->dw, 1, ddc, 0, i, NOTSRCCOPY);
	}
	return 0;
}

DWORD WINAPI RGBScreen(LPVOID lParam) {
	METRICS *m = (METRICS*)lParam;
	HDC ddc = GetDC(m->dwh);

	while (TRUE) {
		SelectObject(ddc, CreateSolidBrush(RGB_RAND));
		BitBlt(ddc, 0, 0, m->dw, m->dh, ddc, 0, 0, PATINVERT);
		Sleep(1000);
	}
}

DWORD WINAPI SetPixels(LPVOID lParam) {
	METRICS *m = (METRICS*)lParam;
	HDC ddc = GetDC(m->dwh);
	
	while (TRUE) {
		SetPixel(ddc, random() % (m->dw + 1), random() % (m->dh + 1), RGB_RAND);
	}
}

DWORD WINAPI ColorMerge(LPVOID lParam) {
	METRICS *m = (METRICS*)lParam;
	HDC ddc = GetDC(m->dwh);

	while (TRUE) {
		SelectObject(ddc, CreateSolidBrush(RGB_RAND));
		BitBlt(ddc, 0, 0, m->dw, m->dh, ddc, 0, 0, MERGECOPY);
		Sleep(1000);
	}
}

DWORD WINAPI CursorDraw(LPVOID lParam) {
	METRICS *m = (METRICS*)lParam;
	HDC ddc = GetDC(m->dwh);
	CURSORINFO ci;

	while (TRUE) {
		ci.cbSize = sizeof(CURSORINFO);
		GetCursorInfo(&ci);

		DrawIcon(ddc, ci.ptScreenPos.x, ci.ptScreenPos.y, ci.hCursor);
	}
}

DWORD WINAPI FlipScreen(LPVOID lParam) {
	METRICS *m = (METRICS*)lParam;
	HDC ddc = GetDC(m->dwh);

	while (TRUE) {
		StretchBlt(ddc, 0, m->dh, m->dw, -m->dh, ddc, 0, 0, m->dw, m->dh, SRCCOPY);
		Sleep(1000);
	}
}

DWORD WINAPI GreenScreen(LPVOID lParam) {
	METRICS *m = (METRICS*)lParam;
	HDC ddc = GetDC(m->dwh);
	RGBQUAD rq = { 0x00, 0xFF, 0x00, 0x00 };
	BITMAPINFO bi = { sizeof(BITMAPINFOHEADER), 1, 1, 1, 32, BI_RGB };

	while (TRUE) {
		StretchDIBits(ddc, 0, 0, m->dw, m->dh, 0, 0, 1, 1, &rq, &bi, DIB_RGB_COLORS, SRCINVERT);
		Sleep(1000);
	}
}

DWORD WINAPI HatchScreen(LPVOID lParam) {
	METRICS *m = (METRICS*)lParam;
	HDC ddc = GetDC(m->dwh);
	HBRUSH hb;

	while (TRUE) {
		hb = CreateHatchBrush(HS_HORIZONTAL, RGB_RAND);
		SelectObject(ddc, hb);
		SetBkColor(ddc, RGB_RAND);
		BitBlt(ddc, 0, 0, m->dw, m->dh, ddc, 0, 0, PATINVERT);

		DeleteObject(hb);
		Sleep(1000);
	}
}

DWORD WINAPI ScreenMelter(LPVOID lParam) {
	METRICS *m = (METRICS*)lParam;
	HDC ddc;
	HDC cdc;
	int r;
	double d;

	while (TRUE) {
		ddc = GetDC(m->dwh);
		cdc = CreateCompatibleDC(ddc);

		SelectObject(cdc, CreateCompatibleBitmap(ddc, 15, m->dh));

		r = random() % (m->dw + 1);

		for (int i = 5; i < 20; i += 3) {
			for (int j = 1; j < 15; j++) {
				BitBlt(cdc, 0, 0, 20, m->dh, ddc, r, 0, SRCCOPY);
				d = sin((j * M_PI) / 15.0) * i;
				StretchBlt(cdc, j, 0, 1, d, cdc, j, 0, 1, 1, SRCCOPY);
				BitBlt(cdc, j, 0, 1, m->dh, cdc, j, -d, SRCCOPY);
				BitBlt(ddc, r, 0, 15, m->dh, cdc, 0, 0, SRCCOPY);
				Sleep(1);
			}
		}
		DeleteDC(ddc);
		DeleteDC(cdc);
	}
}

DWORD WINAPI SpiralScreen(LPVOID lParam) {
	METRICS *m = (METRICS*)lParam;
	HDC ddc = GetDC(m->dwh);
	POINT p[3];

	p[0].x = m->dwd.left + 50;
	p[0].y = m->dwd.top;
	p[1].x = m->dwd.right + 50;
	p[1].y = m->dwd.top + 50;
	p[2].x = m->dwd.left - 50;
	p[2].y = m->dwd.bottom - 50;

	while (TRUE) {
		PlgBlt(ddc, p, ddc, 0, 0, m->dw, m->dh, NULL, 0, 0);
	}
}

DWORD WINAPI ComicSansText(LPVOID lParam) {
	METRICS *m = (METRICS*)lParam;
	HDC ddc = GetDC(m->dwh);

	SelectObject(ddc, COMIC_SANS);

	while (TRUE) {
		SetBkColor(ddc, RGB_RAND);
		SetTextColor(ddc, RGB_RAND);
		TextOut(ddc, random() % (m->dw + 1), random() % (m->dh + 1), L"exampletext", 11);
	}
}

DWORD WINAPI ExtrudeScreen(LPVOID lParam) {
	METRICS *m = (METRICS*)lParam;
	HDC ddc = GetDC(m->dwh);
	HDC bdc = CreateCompatibleDC(ddc);
	BLENDFUNCTION bf = { AC_SRC_OVER, 0, 64, 0 };

	SelectObject(bdc, CreateCompatibleBitmap(ddc, m->dw, m->dh));

	while (TRUE) {
		BitBlt(bdc, 0, 0, m->dw, m->dh, ddc, 0, 0, SRCCOPY);
		StretchBlt(ddc, -5, -5, m->dw + 10, m->dh + 10, ddc, 0, 0, m->dw, m->dh, SRCCOPY);
		AlphaBlend(ddc, 0, 0, m->dw, m->dh, bdc, 0, 0, m->dw, m->dh, bf);
	}
}

DWORD WINAPI GradientBlend(LPVOID lParam) {
	METRICS *m = (METRICS*)lParam;
	HDC ddc = GetDC(m->dwh);
	HDC bdc = CreateCompatibleDC(ddc);
	TRIVERTEX tv[2];
	GRADIENT_RECT gr[1];
	BLENDFUNCTION bf = { AC_SRC_OVER, 0, 64, 0 };
	int r;
	int g;
	int b;

	SelectObject(bdc, CreateCompatibleBitmap(ddc, m->dw, m->dh));

	tv[0].x = 0;
	tv[0].y = 0;
	tv[1].x = m->dw;
	tv[1].y = m->dh;

	gr[0].UpperLeft = 0;
	gr[0].LowerRight = 1;

	while (TRUE) {
		for (int i = 0; i < 2; i++) {
			r = random() % 256;
			g = random() % 256;
			b = random() % 256;
			tv[i].Red = (r << 8) | r;
			tv[i].Green = (g << 8) | g;
			tv[i].Blue = (b << 8) | b;
			tv[i].Alpha = 0;
		}
		GradientFill(bdc, tv, 2, gr, 1, GRADIENT_FILL_RECT_V);
		AlphaBlend(ddc, 0, 0, m->dw, m->dh, bdc, 0, 0, m->dw, m->dh, bf);
		Sleep(1000);
	}
}

DWORD WINAPI SolitaireCards(LPVOID lParam) {
	METRICS *m = (METRICS*)lParam;
	HDC ddc = GetDC(m->dwh);
	HDC cdc = CreateCompatibleDC(ddc);
	BITMAP bc;

	int cardpx = 0;
	int cardpy = 0;
	int wallhits = 0;
	int energy = 2;
	BOOL isfalling = TRUE;
	BOOL movingforward = TRUE;

	HMODULE hcd = LoadLibrary(L"cards.dll");

	if (hcd == NULL) {
		MessageBox(NULL, L"Could not find cards.dll", L"Error", MB_SYSTEMMODAL | MB_OK | MB_ICONERROR);
		return 0;
	}

	HBITMAP hbc = LoadBitmap(hcd, MAKEINTRESOURCE(CARD(7)));

	GetObject(hbc, sizeof(BITMAP), &bc);

	SelectObject(cdc, hbc);

	FreeLibrary(hcd);

	BitBlt(ddc, cardpx, cardpy, m->dw, m->dh, cdc, 0, 0, SRCCOPY);

	while (TRUE) {
		if (movingforward == TRUE)
			cardpx += 8;
		else
			cardpx -= 8;

		if (isfalling == TRUE) {
			energy++;
			cardpy += energy;
		}
		else {
			energy--;
			cardpy -= energy;
		}

		if (cardpx > (m->dw - bc.bmWidth)) {
			movingforward = FALSE;
			wallhits++;
		}

		if (cardpy > (m->dh - bc.bmHeight))
			isfalling = FALSE;

		if (cardpx == 0) {
			movingforward = TRUE;
			wallhits++;
		}

		if (energy < 0) {
			isfalling = TRUE;
		}

		if (wallhits == 5) {
			DeleteObject(ddc);
			DeleteObject(cdc);
			DeleteObject(hbc);
			return 0;
		}

		BitBlt(ddc, cardpx, cardpy, m->dw, m->dh, cdc, 0, 0, SRCCOPY);
		Sleep(1);
	}
}

DWORD WINAPI SaturationTunnel(LPVOID lParam) {
	METRICS *m = (METRICS*)lParam;
	HDC ddc = GetDC(m->dwh);
	HDC bdc = CreateCompatibleDC(ddc);
	BLENDFUNCTION bf = { AC_SRC_OVER, 0, 64, 0 };

	SelectObject(bdc, CreateCompatibleBitmap(ddc, m->dw, m->dh));

	SetStretchBltMode(ddc, HALFTONE);

	while (TRUE) {
		BitBlt(bdc, 0, 0, m->dw, m->dh, ddc, 0, 0, SRCCOPY);
		StretchBlt(ddc, 5, 5, m->dw - 10, m->dh - 10, ddc, 0, 0, m->dw, m->dh, SRCCOPY);
		AlphaBlend(ddc, 0, 0, m->dw, m->dh, bdc, 0, 0, m->dw, m->dh, bf);
	}
}

DWORD WINAPI DesktopIconFollow(LPVOID lParam) {
	HWND slvwh = GetSysListView32();
	int ic = ListView_GetItemCount(slvwh);
	POINT cc;

	while (TRUE) {
		GetCursorPos(&cc);
		for (int i = 0; i < ic; i++)
			ListView_SetItemPosition(slvwh, i, cc.x, cc.y);
	}
}

DWORD WINAPI FullScreenRepaint(LPVOID lParam) {
	METRICS *m = (METRICS*)lParam;
	HDC ddc = GetDC(m->dwh);
	HWND fwh;
	RECT fwd;
	HDC fdc;

	while (TRUE) {
		fwh = GetForegroundWindow();
		fdc = GetDC(fwh);
		GetWindowRect(fwh, &fwd);
		
		StretchBlt(ddc, 0, 0, m->dw, m->dh, fdc, 0, 0, fwd.right - fwd.left, fwd.bottom - fwd.top, SRCCOPY);

		ReleaseDC(fwh, fdc);
		DeleteObject(fwh);
		DeleteObject(&fwd);
	}
}