#include "../header/consoleHelper.h"
#include "../header/window.h"
#include "../header/modes.h"

#ifndef EXITMENU
#define EXITMENU


void exitPage(){
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	clear(97, 1, 1, consoleSize.Y - 2);
	modes::keyInput();
	
	
	gotoXY(25,10);
	char str[] = "Thanks For Playing!";
	typingAnimation(str);
	
	gotoXY(25,12);
	char str1[] = "Created By XK";
	typingAnimation(str1);
	gotoXY(25,14);
	
	char str2[] = "Inspired by Passion, Driven by Purpose,";
	typingAnimation(str2);
	gotoXY(25,15);
	char str3[] = "Together We Shatter Limits and Redefine Boundaries.";
	typingAnimation(str3);
	
	gotoXY(0,consoleSize.Y + 1);
	getchar();
}


void exit(bool *exited){	
	*exited = true;
	exitPage();
}

#endif
