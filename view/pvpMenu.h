#include "../header/consoleHelper.h"
#include "../header/buildings.h"

#ifndef PVPVIEW
#define PVPVIEW

namespace PvpView{
	
	Button *backBtn, *randomBtn, *endBtn;
	
	void printMap(char **map){
		
		for (int i = 0;i < 31;i++){
			gotoXY(10,i + 2); 
			printf("%s",map[i]);
		}
	}
		
	void showPlayer(Player::Node *player, int plus, int minus){
		clear(28,71,1,4);	
		gotoXY(70,0);
		printf("%c",203);		
		gotoXY(70,1);
		printf("%c%s",186, player->name);	
		gotoXY(70,2);
		printf("%cLevel  : %d",186, player->level);
		gotoXY(70,3);
		printf("%cGold   : %d",186, player->gold);
		gotoXY(70,4);
		printf("%cTrophy : %d",186, player->trophy);
		gotoXY(70,5);
		printf("%c + %d",186,plus);
		gotoXY(70,6);
		printf("%c - %d",186,minus);
		
		for (int i = 0;i < 30;i++){
			gotoXY(70 + i,7);
			if (i == 0){
				printf("%c",200);
			}
			else if (i == 29){
				printf("%c",185);
			}
			else{
				printf("%c",205);
			}
		}
	}

	
	void printTownHall(Buildings::TownHall *th){
		gotoXY(th->b.pos.x + 10, th->b.pos.y + 2);
		if (th->b.destroyed == false){
			SetConsoleTextAttribute(hStdOut, 158);
			printf("T");	
		}else{
			SetConsoleTextAttribute(hStdOut, 7);
			printf(" ");				
		}
	}
	
	
	void printErrorMessage(){
		gotoXY(75, 29);
		SetConsoleTextAttribute(hStdOut, 4);
		printf("    Position Invalid");
		SetConsoleTextAttribute(hStdOut, 7);
	}
	
	void printErrorMessage2(){
		gotoXY(75, 29);
		SetConsoleTextAttribute(hStdOut, 4);
		printf("Your gold not enough");
		SetConsoleTextAttribute(hStdOut, 7);
	}
	
	void clearErrorMsg(){
		gotoXY(75, 29);
		printf("                    ");
	}
	
	void printGoldMine(Buildings::GoldMine *g[], int level){
		for (int i = 0;i < level;i++){
			gotoXY(g[i]->b.pos.x + 10, g[i]->b.pos.y + 2);
			if (!g[i]->b.destroyed){
				SetConsoleTextAttribute(hStdOut, 110);
				printf("G");	
			}else{		
				SetConsoleTextAttribute(hStdOut, 7);		
				printf(" ");	
			}
		}
	}
	
	void printDefense(Buildings::Defense *defenses[]){
		for (int i = 0;i < 10;i++){
			if (defenses[i]){
				Buildings::Defense *curr = defenses[i];
				while(curr){
					gotoXY(curr->b.pos.x + 10, curr->b.pos.y + 2);
					if (!curr->b.destroyed){
						SetConsoleTextAttribute(hStdOut, 78);
						printf("%c", curr->symbol);
					}else{
						SetConsoleTextAttribute(hStdOut, 7);
						printf("%c", ' ');						
					}
					curr = curr->next;
				}
			}
		}
	}
	
	void printBuildings(int level){
		printTownHall(Buildings::enemyTownHall);
		printGoldMine(Buildings::enemyGoldMines, level);
		printDefense(Buildings::enemyDef);
		SetConsoleTextAttribute(hStdOut, 7);
	}
	
	
	void initializeComponent(){
		backBtn = createButton("back",88,31,8,3,7);
		randomBtn = createButton("random",70,31,10,3,7);
		endBtn = createButton("end", 89,31,7,3,7);
	}
	
	void printComponent(){
		backBtn->print();
		randomBtn->print();
	}
	
	void menu(){
		frame();
		initializeComponent();
		printComponent();	
	}
	
}

#endif
