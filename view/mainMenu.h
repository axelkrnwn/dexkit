#include "../header/graphics.h"
#include "../header/logo.h"
using namespace component;

Button *playBtn, *loadBtn, *htpBtn, *exitBtn;

void initializeComponents(){
	playBtn =  createButton("  Play Game  ", 38,18,19,3,110);
	loadBtn = createButton("  Load Game  ",38,22,19,3,110);
	htpBtn = createButton("?", 90,30,5,3, 110);
	exitBtn = createButton("  Exit Game  ",38,26,19,3, 79);
}

void printComponents(){
	frame();
	logo(40,5);
	playBtn->print();
	loadBtn->print();
	htpBtn->print();
	exitBtn->print();
}

void mainMenu(){
	initializeComponents();
	printComponents();
}

