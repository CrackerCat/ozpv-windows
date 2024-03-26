#include "dec.h"

int main() {
	/* initalize desktop window measurements (screen measurements) */
	METRICS m;
	m.dwh = GetDesktopWindow();
	GetWindowRect(m.dwh, &m.dwd);
	m.dw = m.dwd.right - m.dwd.left;
	m.dh = m.dwd.bottom - m.dwd.top;

	/* launch gdi threads
 	 * CreateThread(NULL, 0, examplethreadname, &m, 0, NULL);
	 */

	Final();

	Sleep(INFINITE);
}