#include "../header/mazeGeneration.h"
#include "../view/initGameMenu.h"
#include "../view/gameLobbyMenu.h"
#include "./attackMenuController.h"
#include "./shopController.h"
#include "./leaderboardController.h"
#include "./upgradeController.h"
#include <stdlib.h>
#ifndef GAMECONTROLLER
#define GAMECONTROLLER

namespace GameController{
	
	char **map;
	Player::Node *player;	
	bool exitedGame;
	char curName[30];
	
	void clearErrorMsg(){
			gotoXY(55, 19);
			printf("                ");
	}
	
	void saveMap(){
		char path[100];
		sprintf(path, "database/%s/map.csv", curName);
		FILE *fp = fopen(path, "w");	
		for (int i = 0;i < 31;i++){
			for (int j = 0;j < 35;j++){
				if (j > 0){
					fprintf(fp,",");
				}
				fprintf(fp,"%c",map[i][j]);
			}
			fprintf(fp, "\n");
		}	
		fclose(fp);
	}
	
	void createDirectory(){
		char command[100];
		sprintf(command,"mkdir \"database/%s\"", curName);
		system(command);
	}
	
	int checkAround(int x, int y, int bX, int bY){
		if (x + 1 == bX && y == bY) return 1;
		if (x - 1 == bX && y == bY) return 1;
		if (x == bX && y + 1 == bY) return 1;
		if (x == bX && y - 1 == bY) return 1;
		
		return 0;
	}
	
	int validatePos(int x, int y){
		if (x < 0 || x > 30 || y < 0 || y > 34 || map[x][y] != ' '){
			return 1;
		}
		if (Buildings::townHall && y == Buildings::townHall->b.pos.x && 
		x == Buildings::townHall->b.pos.y){
			return 1;
		}
		for (int i = 0;i < player->level;i++){
			if (Buildings::goldMines[i] && 
			y == Buildings::goldMines[i]->b.pos.x && 
			x == Buildings::goldMines[i]->b.pos.y){
				return 1;
			}
		}
		for (int i = 0;i < 10;i++){
			if (Buildings::playerDef[i]){
				Buildings::Defense *curr = Buildings::playerDef[i];
				while(curr){
					if (curr->b.pos.x == y && curr->b.pos.y == x){
						return 1;
					}
					curr = curr->next;
				}	
			}
		}
		return 0;
	}
	
	void inputPosition(char str3[], char name[], char symbol, int count, bool isDef, bool isTH, bool isG, int lvl){
		int x, y;
		do{
			GameLobbyView::showInputText(str3);
			do{
				COORD coord = leftClickListener();	
				if (coord.X == -1 && coord.Y == -1){
					x = -1;
					y = -1;
					continue;
				}
					
				y = coord.X - 10;
				x = coord.Y - 2;	
				if (validatePos(x, y) == 1){
					GameLobbyView::printErrorMessage();
				}	
			}while(validatePos(x,y));
			
			clearErrorMsg();
			
			if (isTH){
				Buildings::townHall = Buildings::createTownHall(y,x);
				GameLobbyView::printTownHall(Buildings::townHall);
			}
			if (isG){
				Buildings::goldMines[player->level - 1] = Buildings::createGoldMine(y,x); 
				GameLobbyView::printGoldMine(Buildings::goldMines, 1);
			}
			if (isDef){
				Buildings::Defense* def = getDefense(lvl-1, symbol, Buildings::allDef);
				
				Buildings::pushDefense(Buildings::createDefense(def->name, y,x,
				def->lvlRequired,def->price,def->b.health,def->b.attack,def->max_quantity,symbol,
				def->cooldown, def->range), Buildings::playerDef);	
						
				GameLobbyView::printDefense(Buildings::playerDef);
			}
			
			SetConsoleTextAttribute(hStdOut, 7);
//			GameLobbyView::clearInput(str3);
			
			count--;
		}while(count > 0);
	}	
	
	void placeBuildings(){
		modes::mouseInput();
		inputPosition("Click path on the map to locate Town Hall","Town Hall",
		'T',1, false, true, false,0);
		inputPosition("Click path on the map to locate Cannon   ","Cannon",
		'C',1, true, false,false,1);
		inputPosition("Click path on the map to Gold Mine       ", "Gold Mine"
		,'G',1, false, false, true,0);
		Buildings::saveProgress(player->name, player->level);
	}
	
	
	void exitMenu(){
		Buildings::saveProgress(player->name, player->level);
		Player::saveProgress();
		Troop::saveProgress(player->name);
		while(Troop::queueHead){
			Troop::dequeue();
		}
		free(Buildings::townHall);
		for (int i = 0;i < player->level;i++){
			if (Buildings::goldMines[i]) free(Buildings::goldMines[i]);
			Buildings::popAll(i);
		}
		Troop::popAllCamp();
		exitedGame = true;
	}
	
	void back(){	
		modes::mouseInput();
		clear(98, 1, 1, consoleSize.Y - 2);
		frame();
		GameLobbyView::gameLobby();
		GameLobbyView::showPlayer(player);
		GameLobbyView::printMap(map);
		GameLobbyView::printBuildings(player->level);
	}
	
	void attackMenu(){
		AttackController::start(player);
		back();
	}
	
	void upgradeMenu(){
		clear(98, 1, 1, consoleSize.Y - 2);
		int currLvl = player->level;
		UpgradeController::start(player);
		if (currLvl < player->level){
			modes::mouseInput();
			clear(98, 1, 1, consoleSize.Y - 2);
			GameLobbyView::printMap(map);
			GameLobbyView::printBuildings(currLvl);
			inputPosition("Click path on the map to Gold Mine       ", 
			"Gold Mine",'G',1, false, false, true,0);
			Buildings::saveProgress(player->name, player->level);
			Player::saveProgress();
		}
		
		back();
	}
	
	void leaderboardMenu(){
		LeaderboardController::player = player;
		clear(98, 1, 1, consoleSize.Y - 2);
		LeaderboardController::start();
		modes::mouseInput();
		back();
	}
	
	void shopMenu(){
		ShopController::player = player;
		Buildings::Defense *def = ShopController::start();
		if (def){
			modes::mouseInput();
			clear(98, 1, 1, consoleSize.Y - 2);
			GameLobbyView::printMap(map);
			GameLobbyView::printBuildings(player->level);
			inputPosition("Click path on the map to locate Cannon   ",
			def->name,def->symbol,1, true,false,false, def->lvlRequired);
			Buildings::saveProgress(player->name, player->level);
		}
		back();
	}
	
	void eventListener(COORD coord){
		if (GameLobbyView::attackBtn->isValid(coord)){
			attackMenu();
		}
		if (GameLobbyView::upgradeBtn->isValid(coord)){
			upgradeMenu();
		}
		if (GameLobbyView::shopBtn->isValid(coord)){
			shopMenu();
		}
		if (GameLobbyView::leaderBoardBtn->isValid(coord)){
			leaderboardMenu();
		}
		if (GameLobbyView::exitBtn->isValid(coord)){
			exitMenu();
		}
		
	}
	
	void game(char name[]){
		SetConsoleTextAttribute(hStdOut, 7);
		modes::mouseInput();
		exitedGame = false;
		clear(98, 1, 1, consoleSize.Y - 2);
		GameLobbyView::gameLobby();
		GameLobbyView::printMap(map);
		GameLobbyView::printBuildings(player->level);
		GameLobbyView::showPlayer(player);
		
		while(!exitedGame){
			COORD coord = leftClickListener();
			eventListener(coord);	
			if (Buildings::collectGold(player)){
				GameLobbyView::showPlayer(player);
			}	
			AttackController::trainProcess();
			Sleep(1000);	
		}
	}
	
	void start(char name[]){
		strcpy(curName, name);
		createDirectory();
		map = PlayerBase::prim();		
		GameLobbyView::printMap(map);
		placeBuildings();
		saveMap();
		game(name);
	}
	
	void load(int id){
		player = Player::search(Player::root, id);
		player->troopCapacity = 10;
		for (int i = 2;i <= player->level;i++){
			player->troopCapacity += 3;
		}
		Buildings::readBuildings(player, Buildings::playerDef,
		true);
		map = PlayerBase::loadMap(player->name);
		Troop::readCamp(player->name);
		game(player->name);
		
	}
}

#endif
