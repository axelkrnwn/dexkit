#include "../model/player.h"
#include "../header/coinChange.h"
#include "../header/minimumOperation.h"
#include "../view/pvpMenu.h"
#include "../header/helper.h"
#include "../header/pathFinding.h"
#include "../view/winMenu.h"
#include "../model/projectile.h"
#include "../header/sorting.h"

#ifndef PVPCONTROLLER
#define PVPCONTROLLER

namespace Pvp{
	
	bool exitPvp, isAttack;
	int trophyAdd, trophyMin, len, curIdx = 0, cooldown;
	int destroyed= 0, allBuilding = 0, attackCount;
	
	Player::Node *player, *opponent;
	char **map;
	
	void showInformation();
	int getDefensesCount();
	
	void inputmap(char **mazeMap, Troop::Camp *troop){
		for(int i = 0; i < 31; i ++){
			for(int j = 0; j < 35; j++){
				troop->nodeMap[j][i] = PathFinding::constructorNode(mazeMap[i][j], false, 
				NULL,i,j);
			}
		}
	}
	
	void popBuildings(){
		if (Buildings::enemyTownHall){
			free(Buildings::enemyTownHall);
			Buildings::enemyTownHall = NULL;
		}
		for (int i = 0;i < opponent->level;i++){
			if (Buildings::goldMines[i]){
				free(Buildings::enemyGoldMines[i]);
				Buildings::enemyGoldMines[i] = NULL;
			} 
			Buildings::popAll(i, false);
		}
	}
	
	int validateBuilding(int x, int y){
		if (Buildings::enemyTownHall && !Buildings::enemyTownHall->b.destroyed && x == Buildings::enemyTownHall->b.pos.x && 
		y == Buildings::enemyTownHall->b.pos.y){
			return 1;
		}
		for (int i = 0;i < opponent->level;i++){
			if (Buildings::enemyGoldMines[i] && 
			x == Buildings::enemyGoldMines[i]->b.pos.x && 
			y == Buildings::enemyGoldMines[i]->b.pos.y && 
			!Buildings::enemyGoldMines[i]->b.destroyed){
				return 1;
			}
		}
		for (int i = 0;i < opponent->level;i++){
			if (Buildings::enemyDef[i]){
				Buildings::Defense *curr = Buildings::enemyDef[i];
				while(curr){
					if (curr->b.pos.x == x && curr->b.pos.y == y
					 && !curr->b.destroyed){
						return 1;
					}
					curr = curr->next;
				}	
			}
		}
		return 0;
	}
	
	int validatePos(int x, int y){
		if (x < 0 || x > 30 || y < 0 || y > 34 || map[x][y] != ' '){
			return 1;
		}
		return validateBuilding(y,x);
	}
	
	void clearInformation(){
		for (int i = 5;i < 5 + allBuilding;i++){
			gotoXY(50, i);
			printf("                   ");
		}
	}
	
	void eventListener(COORD coord){
		if (!isAttack && PvpView::backBtn->isValid(coord)){
			while(Troop::attackHead){
				Troop::popAttackTroop();
			}
			popBuildings();
			exitPvp = true;
			return;
		}
		if (isAttack && PvpView::endBtn->isValid(coord)){
			while(Troop::attackHead){
				Troop::popAttackTroop();
			}
			exitPvp = true;
		}
		if (coord.X >= 71 && coord.Y >= 9 && coord.Y < 9 + Troop::getCampLength()){
			curIdx = coord.Y - 9;
			Troop::viewCamp(71,9, curIdx);
		}
		
		if (coord.X >= 10 && coord.X <= 45 &&
			coord.Y >= 2 && coord.Y <= 33){
				if (validatePos(coord.Y - 2, coord.X - 10) == 1){
					PvpView::printErrorMessage();
					return;
				}
				isAttack = true;
				PvpView::clearErrorMsg();
				clear(
				PvpView::backBtn->size.x,
				PvpView::backBtn->pos.x,
				PvpView::backBtn->pos.y,
				PvpView::backBtn->size.y);
				
				clear(
				PvpView::randomBtn->size.x + 3,
				PvpView::randomBtn->pos.x,
				PvpView::randomBtn->pos.y,
				PvpView::randomBtn->size.y);
				
				PvpView::endBtn->print();
				
				if (Troop::campHead){
					attackCount++;
					Troop::spawnTroop(&curIdx,coord,opponent, map);
					
					if (Troop::attackTail->target == NULL){
						exitPvp = true;
						return;
					}else{
						showInformation();
						inputmap(map,Troop::attackTail);
						if (strcmpi(Troop::attackTail->category, "Destroyer") == 0)
							PathFinding::astar(Troop::attackTail->nodeMap,Troop::attackTail->target->pos, Troop::attackTail->pos);
						else
							PathFinding::dfs(Troop::attackTail->nodeMap,Troop::attackTail->target->pos, Troop::attackTail->pos);
					}
				}
		}
		if (PvpView::randomBtn->isValid(coord)){
			if (player->gold < 50*player->level){
				PvpView::printErrorMessage2();
				return;
			}
			PvpView::clearErrorMsg();
			player->gold -= 50*player->level;
			popBuildings();
			int random = randomize(player->id, len);
			opponent = Player::search(Player::root, random);
			

			trophyMin = coinChange(opponent->trophy,player->level);
			trophyAdd = minimumOperation(player->trophy, opponent->trophy, player->level, opponent->level);
			Buildings::readBuildings(opponent, Buildings::enemyDef
			, false);
			map = PlayerBase::loadMap(opponent->name);
			
			clearInformation();
			allBuilding = getDefensesCount() + 1 + opponent->level;
			PvpView::showPlayer(opponent,trophyAdd, trophyMin);
			PvpView::printMap(map);
			PvpView::printBuildings(opponent->level);
			showInformation();
		}
	}
	
	bool thStarValidation(){
		if (!enemyTownHall->b.destroyed){
			return false;
		}
		return true;
	}
	
	int destroyedCount(int *total){
		int destroyed = 0;
		if (thStarValidation()){
			destroyed++;
		}
		*total += 1;
		
		for (int i = 0;i < opponent->level;i++){
			if (enemyGoldMines[i]->b.destroyed){
				destroyed++;
			}
			*total += 1;
		}
		for (int i = 0;i < opponent->level;i++){
			Defense *curr = enemyDef[i];
			while(curr){
				if (curr->b.destroyed) destroyed++;
				curr =curr->next;
				*total += 1;
			}
		}
		return destroyed;
	}
	
	int winValidation(int *destroyed){
		int total = 0;
		int star = 0;
		*destroyed = destroyedCount(&total);
		
		if (thStarValidation()){
			star++;
		}
		
		if (total/2 <= *destroyed){
			star++;
		}
		if (total == *destroyed){
			star++;
		}
		return star;	
	}
	
	void result(int stars){
		clear(98, 1, 1, consoleSize.Y - 2);
		Win::menu(stars);
		if (stars == 0){
			gotoXY(42,29);
			printf("Trophy: -%.2d", trophyMin);
		}else{
			gotoXY(42,29);
			printf("Trophy: +%.2d", trophyAdd*stars/3);
		}
		popBuildings();
		exitPvp = true;
		while(true){
			COORD coord = leftClickListener();
			if (Win::backBtn->isValid(coord)){
				return;
			}
		}
	}
	
	void printDestroyed()
	{
		gotoXY(50,1);
		printf("Building :");
		gotoXY(50,2);
		printf("Destroyed/Total");
		gotoXY(50,3);
		printf("%d/%d", destroyed,allBuilding);	
	}
	
	void moveBullet(){
		Bullet *curr = bulletHead;
		while(curr){
			if (curr->hit) {
				curr = curr->next;
				continue;
			}
			if (!curr->firstMove){
				if (validateBuilding(curr->pos.x,curr->pos.y) == 0){
					gotoXY(curr->pos.x
					 + 10, curr->pos.y + 2);
					printf("%c", map[curr->pos.y][curr->pos.x]);
				}
			}
			if (curr->pos.x != curr->target->pos.x || curr->pos.y != curr->target->pos.y){
				PathFinding::Node *curPos = curr->nodeMap[curr->pos.x][curr->pos.y]->before;
				curr->pos = {curPos->pos.y, curPos->pos.x};	
				if (curr->firstMove) curr->firstMove = false;
				
				if (validateBuilding(curr->pos.x,curr->pos.y) == 0){
					gotoXY(curr->pos.x
					 + 10, curr->pos.y + 2);
					printf("%c", 'o');
				}
			}else{
				if (curr->target->health > 0)
					curr->target->health -= curr->attack;
					
				if (curr->target->health <= 0){
					curr->target->health = 0;
					curr->target->alive = false;
					if (!validateBuilding(curr->target->pos.x, curr->target->pos.y)){
						gotoXY(curr->target->pos.x, curr->target->pos.y);
						printf(" ");
					}
					PvpView::printBuildings(opponent->level);
				}
				curr->hit = true;
			}
			curr = curr->next;
		}
		showInformation();
	}
	
	void defend(Troop::Camp *troop){
			for (int i = 0;i < opponent->level;i++){
				Defense *curr = enemyDef[i];
				while(curr){
					if (curr->b.destroyed == false && curr->cooldown <= curr->counter){
						curr->counter = 0;
						spawnBullet(troop, curr, map);
					} 
					curr->counter += 1;
					curr = curr->next;
				}
			}
		
	}
	
	bool checkAliveTroop(){
		Troop::Camp *curr = Troop::attackHead;
		while(curr){
			if (curr->alive) return true;
			curr = curr->next;
		} 
		return false;
	}
	
	
	void showInformation(){
		Sorting::Entity arr[allBuilding];
		int cur = 0;
		char str[] = "Town Hall";
		arr[cur].name = str;
		arr[cur++].health = Buildings::enemyTownHall->b.health;
		
		for (int i = 0;i < opponent->level;i++){
			char str2[] = "Gold Mine";
			arr[cur].name =str2;
			arr[cur++].health = Buildings::enemyGoldMines[i]->b.health;
			Buildings::Defense *curr = Buildings::enemyDef[i];
			while(curr){
				arr[cur].name = curr->name;
				arr[cur++].health = curr->b.health;
				curr = curr->next;
			}
		}
		Sorting::quickSort(arr, 0, allBuilding - 1);
		int counter = (allBuilding > 5)? 5:allBuilding;
		for (int i = 0;i < counter;i++){
			gotoXY(50,5 + i);
			printf("%-14s %4d",arr[i].name, arr[i].health);
		}
		if (allBuilding > 5){
			gotoXY(50,10);
			printf("...");
		}
		cur = 0;
		if (attackCount > 0){
			Sorting::Entity arr2[attackCount];
			Troop::Camp *curr = Troop::attackHead;
			while(curr){
				arr2[cur].name = curr->name;	
				arr2[cur++].health = curr->health;	
				curr = curr->next;
			}
			Sorting::mergeSort(arr2,0, attackCount - 1);
			int counter = (attackCount > 5)? 5:attackCount;
			for (int i = 0;i < counter;i++){
				gotoXY(50,13 + i);
				printf("%-14s %4d",arr2[i].name, arr2[i].health);
			}
			if (attackCount > 5){
				gotoXY(50,18);
				printf("...");
			}
		}
		
		
	}
	
	void move(){
		int destroy = 0;
		int stars = winValidation(&destroy);
		if (stars == 3){
			Sleep(1000);
			while(Troop::attackHead){
				Troop::popAttackTroop();
			}
			player->trophy += trophyAdd;
			if (opponent->trophy - trophyMin > 0)
				opponent->trophy -= trophyMin;
				
			player->gold += destroy*player->level*10;
			
			popAllBullet();
			exitPvp = true;
			result(stars);
			return;
		}
		if (isAttack && exitPvp || (!Troop::campHead && !checkAliveTroop())){
			Sleep(1000);
			while(Troop::attackHead){
				Troop::popAttackTroop();
			}
			if (stars == 0){
				player->trophy -= trophyMin;
				opponent->trophy += trophyAdd;
			}else{
				player->trophy += trophyAdd*stars/3;
				player->gold += destroy*player->level*10*stars/3;
				if (opponent->trophy - trophyMin*stars/3 > 0)
					opponent->trophy -= trophyMin*stars/3;
			}			
			popAllBullet();
			result(stars);
			return;
		}
		moveBullet();
		Troop::Camp *curr = Troop::attackHead;
		while(curr){
			if (!curr->alive){
				curr = curr->next;
				continue;
			}
			if (!curr->firstMove){
				if (validateBuilding(curr->pos.x, curr->pos.y) == 0 && 
				!(strcmpi(curr->category, "Ranged") == 0 && eucledianDist(curr->pos, curr->target->pos) <=5)){
					gotoXY(curr->pos.x
					 + 10, curr->pos.y + 2);
					printf("%c", map[curr->pos.y][curr->pos.x]);
				}
			}
			
			if (strcmpi(curr->category, "Ranged") == 0){
				inputmap(map,curr);
				Troop::updateBuildings(opponent, curr, curr->pos.x+10, curr->pos.y+2, map);
				if (curr->target == NULL){
					return;
				} 		
				PathFinding::dfs(curr->nodeMap,curr->target->pos, curr->pos);
			}
			
			if ((strcmpi(curr->category, "Ranged") == 0 && eucledianDist(curr->pos, curr->target->pos) <=5) || 
				(curr->pos.x == curr->target->pos.x && curr->pos.y == curr->target->pos.y)){
				curr->target->health -= curr->attack;
				gotoXY(curr->target->pos.x + 10, curr->target->pos.y + 2);
				
				SetConsoleTextAttribute(hStdOut, 167);
				printf("*");
				SetConsoleTextAttribute(hStdOut, 7);
				showInformation();
				if (strcmpi(curr->category, "Ranged") == 0)
					Troop::drawTroop(curr);	
				if (curr->target->health <= 0){
					curr->target->destroyed = true;
					curr->target->health = 0;
					
					int total = 0;
					destroyed = destroyedCount(&total);
					printDestroyed();
					if (curr->target == NULL) return;
					
					
					if (validateBuilding(curr->pos.x, curr->pos.y) || strcmpi(curr->category, "Ranged") == 0){
						SetConsoleTextAttribute(hStdOut, 7);
						gotoXY(curr->pos.x + 10, curr->pos.y + 2);
						printf(" ");
						gotoXY(curr->target->pos.x + 10, curr->target->pos.y + 2);
						printf(" ");
						
					}
					
					inputmap(map,curr);
					curr->target = NULL;
					Troop::insertBuildings(opponent, curr, curr->pos.x+10, curr->pos.y+2, map);
					if (curr->target == NULL){
						return;
					} 		
					if (strcmpi(curr->category, "Destroyer") == 0)
						PathFinding::astar(curr->nodeMap,curr->target->pos, curr->pos);
					else
						PathFinding::dfs(curr->nodeMap,curr->target->pos, curr->pos);
				}
			}else{
				PathFinding::Node *curPos = curr->nodeMap[curr->pos.x][curr->pos.y]->before;
				curr->prevPos = curr->pos;
				curr->pos = {curPos->pos.y, curPos->pos.x};	
				if (curr->firstMove) curr->firstMove = false;
				
				if (curr->target->destroyed){
					
					inputmap(map,curr);
					curr->target = NULL;
					Troop::insertBuildings(opponent, curr, curr->pos.x+10, curr->pos.y+2, map);
					if (curr->target == NULL){
						return;
					} 		
					if (strcmpi(curr->category, "Destroyer") == 0)
						PathFinding::astar(curr->nodeMap,curr->target->pos, curr->pos);
					else
						PathFinding::dfs(curr->nodeMap,curr->target->pos, curr->pos);
				}
				
				Bullet *cur = bulletHead;
				while(cur){
					cur->target = curr;
					inputMapBullet(cur, map);
					PathFinding::dijkstra(cur->nodeMap, cur->target->pos, cur->pos);
					cur = cur->next;
				}
				if (validateBuilding(curr->pos.x, curr->pos.y) == 0)
					Troop::drawTroop(curr);	
			}
			defend(curr);
			curr = curr->next;
		}
	}
	
	int getDefensesCount(){
		int j = 0;
		
		for (int i = 0;i < opponent->level;i++){
			Defense *curr = enemyDef[i];
			while(curr){
				j++;
				curr = curr->next;
			}
		}
		return j;
	}
	
	void game(){
		cooldown = 20;
		PvpView::menu();
		PvpView::showPlayer(opponent,trophyAdd, trophyMin);
		PvpView::printMap(map);
		PvpView::printBuildings(opponent->level);
		Troop::viewCamp(71,9, curIdx);
		destroyed = 0;
		allBuilding = getDefensesCount() + 1 + opponent->level;
		printDestroyed();
		showInformation();
		while(!exitPvp){
			COORD coord = leftClickListener();
			eventListener(coord);
			Sleep(100);
			if (isAttack){
				move();
			}
		}
	}
	
	void start(Player::Node *p){
		player = p;	
		player->gold -= 50*player->level;
		exitPvp = false;
		len = Player::getSize();
		int random = randomize(player->id, len);
		isAttack = false;
		attackCount = 0;
		opponent = Player::search(Player::root, random);
		Buildings::readBuildings(opponent, Buildings::enemyDef
		, false);
		map = PlayerBase::loadMap(opponent->name);
		
		trophyMin = coinChange(opponent->trophy,player->level);
		trophyAdd = minimumOperation(player->trophy, opponent->trophy, player->level, opponent->level);
		game();
	}
	
}

#endif
