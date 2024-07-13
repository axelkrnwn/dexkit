#include "../header/graphics.h"
#include "../header/buildings.h"
#include <string.h>
#ifndef SHOPVIEW
#define SHOPVIEW

namespace Shop{
	
	Button *backBtn, *buyBtn,*nextBtn, *prevBtn;
	Label *shopLabel;
	
	Card *cards[4];
	
	void initializeComponents(){
		backBtn = createButton("Back",2,2,10,3,7);
		shopLabel = createLabel("Shop",15,3);
		buyBtn = createButton("Buy Building",42,30,strlen("Buy Building") + 4,3,7);
		prevBtn = createButton("<<",3,30,8,3,7);
		nextBtn = createButton(">>",87,30,8,3,7);
	}
	
	void initializeCard(char symbol, int idx, int tableIdx){
		cards[idx] = createCard(symbol,20 + 15*idx,6,15,15, tableIdx);
	}
	
	void printCards(){			
		for (int i = 0;i < 4;i++){
			cards[i]->print();
		}
	}
	
	void printComponents(){
		backBtn->print();
		shopLabel->print();
		prevBtn->print();
		buyBtn->print();
		nextBtn->print();
	
	}
	
	void clearDetail(){
		for(int i = 0;i < 3;i++){
			gotoXY(40,23 + i);
			for (int j = 0;j < 4;j++)
				printf("     ");
		}
	}
	
	void showDetail(Buildings::Defense *def){
		clearDetail();
		gotoXY(40, 23);
		printf("%s                        ", def->name);
		gotoXY(40,24);
		printf("Level Required : %d       ", def->lvlRequired);
		gotoXY(40,25);
		printf("Price          : %d       ", def->price);
	}
	
	void printErrorMessage(char error[]){
		gotoXY(35, 28);
		SetConsoleTextAttribute(hStdOut, 4);
		printf("%s", error);
		SetConsoleTextAttribute(hStdOut, 7);
	}
	
	void shopMenu(){
		frame();
		initializeComponents();
		printComponents();
	}
	
}

#endif
