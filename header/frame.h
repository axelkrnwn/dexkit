#include "consoleHelper.h"
#include "window.h"
#ifndef FRAME
#define FRAME

void frame(){
	for (int i = 0;i < consoleSize.X;i++){
		for (int j = 0;j < consoleSize.Y;j++){
			if (i == 0 && j == 0){
				gotoXY(i,j);
				printf("%c",201);
			}
			else if (i == consoleSize.X -1 && j == 0){
				gotoXY(i,j);
				printf("%c",187);
			}
			else if (i == 0 && j == consoleSize.Y-1){
				gotoXY(i,j);
				printf("%c",200);
			}
			else if (i == consoleSize.X-1 && j == consoleSize.Y-1){
				gotoXY(i,j);
				printf("%c",188);
			}
			else if (i == 0 || i == consoleSize.X - 1){
				gotoXY(i,j);
				printf("%c",186);
			}
			else if (j == 0 || j == consoleSize.Y - 1){
				gotoXY(i,j);
				printf("%c",205);
			}
		}
	}
}

#endif
