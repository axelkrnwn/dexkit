#include "../header/consoleHelper.h"
#include "../header/buildings.h"

#ifndef GAMELOBBYVIEW
#define GAMELOBBYVIEW

namespace GameLobbyView{
	
	Button *exitBtn, *attackBtn, *shopBtn, *leaderBoardBtn, *upgradeBtn;
	
	void printTownHall(Buildings::TownHall *th){
		gotoXY(th->b.pos.x + 10, th->b.pos.y + 2);
		SetConsoleTextAttribute(hStdOut, 158);
		printf("T");	
	}
	
	void printGoldMine(Buildings::GoldMine *g[], int level){
		for (int i = 0;i < level;i++){
			gotoXY(g[i]->b.pos.x + 10, g[i]->b.pos.y + 2);
			SetConsoleTextAttribute(hStdOut, 110);
			printf("G");	
		}
	}
	
	void printErrorMessage(){
		gotoXY(55, 19);
		SetConsoleTextAttribute(hStdOut, 4);
		printf("Position Invalid");
		SetConsoleTextAttribute(hStdOut, 7);
	}
	
	void printDefense(Buildings::Defense *defenses[]){
		for (int i = 0;i < 10;i++){
			if (defenses[i]){
				Buildings::Defense *curr = defenses[i];
				while(curr){
					gotoXY(curr->b.pos.x + 10, curr->b.pos.y + 2);
					SetConsoleTextAttribute(hStdOut, 78);
					printf("%c", curr->symbol);
					curr = curr->next;
				}
			}
		}
	}
	
	void printMap(char **map){
		
		for (int i = 0;i < 31;i++){
			gotoXY(10,i + 2); 
			printf("%s",map[i]);
		}
	}
	
	void printBuildings(int level){
		printTownHall(Buildings::townHall);
		printGoldMine(Buildings::goldMines, level);
		printDefense(Buildings::playerDef);
		SetConsoleTextAttribute(hStdOut, 7);
	}
	
	
	void clearInput(char str[]){
		int len = 8;
		for (int i = 0; i < len;i++){
			gotoXY(55 + strlen(str) + i, 17);
			printf(" ");
		}
		gotoXY(55 + strlen(str), 17);
	}
	
	void showPlayer(Player::Node *player){	
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
		
		for (int i = 0;i < 30;i++){
			gotoXY(70 + i,5);
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
	
	void showInputText(char str[]){
		gotoXY(55,17);
		printf("%s", str);
	}
	
	void initializeComponents(){
		attackBtn =      createButton(" Attack Base ",70,9,17,3,7);
		shopBtn =        createButton("  Game Shop  ",70,13,17,3,7);
		upgradeBtn =     createButton("   Upgrade   ",70,17,17,3,7);		
		leaderBoardBtn = createButton(" LeaderBoard ",70,21,17,3,7);		
		exitBtn =        createButton("  Save&Exit  ",70,25,17,3,7);
	}
	
	void printComponents(){
		attackBtn->print();
		shopBtn->print();
		upgradeBtn->print();
		leaderBoardBtn->print();
		exitBtn->print();
	}
	
	void gameLobby(){
		initializeComponents();
		printComponents();
	}
	
}

#endif

