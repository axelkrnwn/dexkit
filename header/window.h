#include <windows.h>

#ifndef WINDOW
#define WINDOW

COORD consoleSize = {100, 35};

void createWindow(){
	bool ok = SetConsoleScreenBufferSize(hStdOut,consoleSize);
	SMALL_RECT size = {0,0,consoleSize.X - 1, consoleSize.Y - 1};
	SetConsoleWindowInfo(hStdOut, TRUE, &size);
}

#endif
