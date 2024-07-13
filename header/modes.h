#include <windows.h>
#include "./consoleHelper.h"

#ifndef modes
#define modes

namespace modes{
	
	void mouseInput(){
		DWORD mode = ENABLE_EXTENDED_FLAGS;
	
		if (!SetConsoleMode(hStdIn, mode)){
			return;
		}		
		CONSOLE_CURSOR_INFO info;
		info.dwSize = 100;
		info.bVisible = false;
		SetConsoleCursorInfo(hStdOut, &info);
		
		mode = ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT;
		SetConsoleMode(hStdIn, mode);
	}
	
	void keyInput(){
		DWORD mode;
		mode = 503;
		CONSOLE_CURSOR_INFO info;
		info.dwSize = 100;
		info.bVisible = true;
		SetConsoleCursorInfo(hStdOut, &info);
		SetConsoleMode(hStdIn, mode);
	}
	
}

#endif

