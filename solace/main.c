#include "dec.h"

int main(void) {
	/*if (isok() != TRUE)
		ExitProcess(EXIT_SUCCESS);*/

	/* initalize desktop window measurements for drawing */
	METRICS m;
	m.dwh = GetDesktopWindow();
	GetWindowRect(m.dwh, &m.dwd);
	m.dw = m.dwd.right - m.dwd.left;
	m.dh = m.dwd.bottom - m.dwd.top;

	HANDLE thread;

	thread = CreateThread(NULL, 0, ScreenMelter, &m, 0, NULL);

	Sleep(10000);

	CreateThread(NULL, 0, SolitaireCards, &m, 0, NULL);

	Sleep(20000);

	/* launch threads

	HANDLE thread;

	thread = CreateThread(NULL, 0, ScreenMelter, &m, 0, NULL);

	Sleep(10000);

	CreateThread(NULL, 0, SolitaireCards, &m, 0, NULL);

	Sleep(20000);

	if (thread != 0)
		TerminateThread(thread, EXIT_SUCCESS);
	*/

	return 0;
}