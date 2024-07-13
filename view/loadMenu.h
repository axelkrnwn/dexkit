#include "../model/player.h"
using namespace component;

#ifndef LOADMENUVIEW
#define LOADMENUVIEW


namespace LoadMenuView{
	
	Button *back, *next, *prev;
	Label *loadLbl;

	void initializeComponents(){
		loadLbl = createLabel("Load game",40 ,8);
		back = createButton(" <= ", 3,3,10,3,110);
		next = createButton(" >> ", 60,30,10,3,110);
		prev = createButton(" << ", 20,30,10,3,110);
	}
	
	void printComponents(){
		loadLbl->print();
		back->print();
		next->print();
		prev->print();	
	}
	
	void createPoint(int y, char c){
		gotoXY(61, y);
		printf("%c", c);
	}
	
	void printConfirmation(){
		gotoXY(30,23);
		printf("Are you sure want to load this player? [Y | N]");
	}
	
	void cancelConfirmation(){
		gotoXY(30,23);
		printf("                                              ");
	}
	
	void loadMenu(){
		initializeComponents();
		printComponents();
	}
	
}


#endif
