#include "dep.h"

DWORD WINAPI Distort(LPVOID parameter) {
	POINT p[3];
	while (TRUE) {
		graphics
		p[0].x = rect.left - 2;
		p[0].y = rect.top - 2;
		p[1].x = rect.right + 2;
		p[1].y = rect.top;
		p[2].x = rect.left - 2;
		p[2].y = rect.bottom + 2;
		PlgBlt(hdc, p, hdc, 1, 1, w, h, NULL, NULL, NULL);
		Sleep(1);
	}
}

DWORD WINAPI Spiral(LPVOID parameter) {
	POINT p[3];
	while (TRUE) {
		graphics
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

DWORD WINAPI InvertedTriangles(LPVOID parameter) {
	POINT p[3];
	while (TRUE) {
		graphics
		p[0].x = w;
		p[0].y = random() % w;
		p[1].x = random() % h;
		p[1].y = random() % 400;
		p[2].x = random() % 400;
		p[2].y = h;
		InvertRgn(hdc, CreatePolygonRgn(p, 3, ALTERNATE));
		Sleep(random() % 2000);
	}
}

DWORD WINAPI InvertedCircles(LPVOID parameter) {
	while (TRUE) {
		graphics
		InvertRgn(hdc, CreateEllipticRgn(random() % w, random() % h, random() % w, random() % h));
		Sleep(random() % 2000);
	}
}

DWORD WINAPI SideIncInvert(LPVOID parameter) {
	while (TRUE) {
		graphics
		for (int i = 0; i < w; i++) {
			BitBlt(hdc, i, NULL, 1, h, hdc, i, NULL, NOTSRCCOPY);
		}
	}
}

DWORD WINAPI SplitFlip(LPVOID parameter) {
	while (TRUE) {
		graphics
		while (TRUE) {
			StretchBlt(hdc, w / 2, NULL, w, h, hdc, NULL, NULL, w, h, NOTSRCCOPY);
			StretchBlt(hdc, w, NULL, -w, h, hdc, w / 2, NULL, w, h, SRCCOPY);
			Sleep(500);
		}
	}
}