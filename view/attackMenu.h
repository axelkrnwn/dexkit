#include "../model/troop.h"
#ifndef ATTACKMENU
#define ATTACKMENU

namespace Attack{
	
	Button *pveBtn, *pvpBtn, *backBtn;
	Label *types[4],*troops[17], *trains[17], *yourTroops[17];
	Label *troopLbl, *trainingLbl, *yourTroopLbl;
	
	void initializeComponents(){
		backBtn = createButton("Back",85,30,8,3,7);
		pveBtn = createButton("Attack Ellers",10,30,17,3,7);
		pvpBtn = createButton("Attack Player",40,30,17,3,7);
		troopLbl = createLabel("All Troops",2,1);
		trainingLbl = createLabel("Training",21,1);
		yourTroopLbl = createLabel("Your Troops",44,1);
	}
	void printTypes(){
		for (int i = 0;i < 4;i++){
			types[i]->print();
		}
	}
	
	void printErrorMessage(char msg[]){
		gotoXY(5, 28);
		SetConsoleTextAttribute(hStdOut, 4);
		printf("%s", msg);
		SetConsoleTextAttribute(hStdOut, 7);
	}
	void clearErrorMsg(){
		int len = 40;
		for (int i = 0;i < len;i++){
			gotoXY(5 + i, 28);
			printf(" ");
		}
	}
	
	
	void printComponents(){
		backBtn->print();
		pveBtn->print();
		pvpBtn->print();
		troopLbl->print();
		trainingLbl->print();
		yourTroopLbl->print();
		Troop::viewTrain();
		Troop::viewCamp(44,2, -1);
	}
	
	void attackMenu(){
		initializeComponents();
		printComponents();
	}
}

#endif
