#include "../header/buildings.h"

#ifndef UPGRADEVIEW
#define UPGRADEVIEW

namespace Upgrade{
	
	Label *confirmationLabel;
	Button *acceptBtn, *cancelBtn, *backBtn;
	
	void initializeComponents(){
		confirmationLabel = createLabel("",25,8);
		acceptBtn = createButton("Yes",50,10,7,3,7);
		cancelBtn = createButton("No",40,10,6,3,7);
	}
	
	void printErrorMessage(){
		gotoXY(35, 15);
		SetConsoleTextAttribute(hStdOut, 4);
		printf("Not enough gold to upgrade");
		SetConsoleTextAttribute(hStdOut, 7);
	}
	void printUnlockedTroop(int level, int start){
		Troop::Node *curr = Troop::head;
		
		while(curr){
			Troop::InnerNode *cur = curr->head;
			while(cur){
				if(cur->levelRequired == level){
					gotoXY(35,18 + start);
					printf("%s (HP : %d, Attack : %d)", cur->name, cur->health, cur->attack);
					start++;
				}
				cur = cur->next;
			}
			curr = curr->next;
		}
	}
	
	void printUnlocked(Buildings::Defense *defense, int level){
		Buildings::Defense *def = defense;
		int i = 1;
		
		gotoXY(35,17);
		printf("Unlocked : ");
		while(def){
			gotoXY(35,18 + i);
			printf("%s (HP : %d, Attack : %d)", def->name, def->b.health, def->b.attack);
			i++;
			def = def->next;
		}
		printUnlockedTroop(level,i); 
		
	}
	
	
	
	void printComponents(Buildings::Defense *defense, int level){
		confirmationLabel->print();
		acceptBtn->print();
		cancelBtn->print();
		printUnlocked(defense, level);
	}
	
	void upgradeMenu(int price, int lvl, Buildings::Defense *defense){
		frame();
		initializeComponents();
		sprintf(confirmationLabel->text, 
		"Do you want to upgrade from %d to %d with cost %d?",lvl,lvl+1,price);
		printComponents(defense, lvl+1);
	}
	
}

#endif
