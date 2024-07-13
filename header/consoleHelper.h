#include <windows.h>
#include <stdio.h>
#define hStdOut GetStdHandle(STD_OUTPUT_HANDLE)
#define hStdIn GetStdHandle(STD_INPUT_HANDLE)

#ifndef consoleHelper
#define consoleHelper

void gotoXY(int x, int y){
	COORD pos = {x,y};
	SetConsoleCursorPosition(hStdOut, pos);
}

void typingAnimation(char a[]){
	for (int i = 0;a[i] != '\0';i++){
		printf("%c", a[i]);
		Sleep(30);
	}
}

void clear(int n, int startX, int startY, int length){
		
	char spaces[n+5] = "";
	for (int i = 0;i < n;i++){
		strcat(spaces," ");
	}
	for (int i = 0;i < length;i++){
		gotoXY(startX,startY + i);
		printf("%s", spaces);
	}
}

#endif
