#include "dec.h"
#include <TlHelp32.h>

int random() {
	int r = 0;
	HCRYPTPROV c;

	CryptAcquireContext(&c, NULL, NULL, PROV_RSA_FULL, CRYPT_SILENT | CRYPT_VERIFYCONTEXT);
	CryptGenRandom(c, sizeof(int), (BYTE*)&r);
	CryptReleaseContext(c, 0);

	return r & INT_MAX;
}

int ProcessSetPrivilege(LPCWSTR lpPrivilegeName, DWORD dwAttributes) {
	HANDLE ht;
	TOKEN_PRIVILEGES tp;

	tp.PrivilegeCount = 1;
	tp.Privileges[0].Attributes = dwAttributes;

	OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &ht);

	LookupPrivilegeValue(NULL, lpPrivilegeName, &tp.Privileges[0].Luid);

	AdjustTokenPrivileges(ht, FALSE, &tp, sizeof(TOKEN_PRIVILEGES), NULL, NULL);

	return 0;
}

DWORD LookupProcessID(LPCWSTR lpFilename) {
	DWORD dwPid = 0;
	HANDLE hs = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 procEntry = { sizeof(PROCESSENTRY32) };

	if (!Process32First(hs, &procEntry))
		return 0;

	while (Process32Next(hs, &procEntry) == TRUE) {
		if (!lstrcmp(lpFilename, procEntry.szExeFile)) {
			dwPid = procEntry.th32ProcessID;
			break;
		}
	}
	return dwPid;
}