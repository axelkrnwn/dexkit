#include <windows.h>
#include "./consoleHelper.h"

COORD leftClickListener(){
	DWORD mode, cNumRecord;
	INPUT_RECORD irInBuf[128];
	
	if (GetAsyncKeyState(0x01)){
		ReadConsoleInput(hStdIn,irInBuf,128, &cNumRecord);
		for (DWORD i = 0;i < cNumRecord; i++){
			
			switch(irInBuf[i].EventType){
				case MOUSE_EVENT:{
					MOUSE_EVENT_RECORD record = irInBuf[i].Event.MouseEvent;
					if (record.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED){
						int x = record.dwMousePosition.X;
						int y = record.dwMousePosition.Y;
						return {x, y};
					}
					break;
				}
			}
		}
	}
	
	return {-1,-1};
}
