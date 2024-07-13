#ifndef PVECONTROLLER
#define PVECONTROLLER

#include "../header/sorting.h"
#include "../model/player.h"
#include "../model/projectile.h"
#include "../header/helper.h"
#include "../header/pathFinding.h"
#include "../view/winMenu.h"
#include "../view/pveView.h"

namespace Pve{
	
	Player::Node *player;
	bool exitPve, isAttack;
	int cooldown, curIdx = 0;
	char **map;
	int dest, all, attackCount;
	
	void showInformation();
	
	int validateBuilding(int x, int y){
		if (Buildings::enemyTownHall && !Buildings::enemyTownHall->b.destroyed && x == Buildings::enemyTownHall->b.pos.x && 
		y == Buildings::enemyTownHall->b.pos.y){
			return 1;
		}
		for (int i = 0;i < player->level;i++){
			if (Buildings::enemyGoldMines[i] && 
			x == Buildings::enemyGoldMines[i]->b.pos.x && 
			y == Buildings::enemyGoldMines[i]->b.pos.y && 
			!Buildings::enemyGoldMines[i]->b.destroyed){
				return 1;
			}
		}
		for (int i = 0;i < player->level;i++){
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
	
	void initBuildings(){
		
		int x, y;
		x = 15;
		y = 15;
		enemyTownHall = createTownHall(y,x);
		enemyTownHall->b.destroyed = false;
		
		for (int i = 0;i < player->level;i++){
			do{
				x = (rand()%15)*2 + 1;
				y = (rand()%17)*2 + 1;
			}while(validatePos(x,y));
			
			enemyGoldMines[i] = createGoldMine(y,x);
			enemyGoldMines[i]->b.destroyed = false;
			
			Defense *curr = allDef[i];
			while(curr){
				for (int j = 0;j < curr->max_quantity - (6 - player->level);j++){
					do{
						x = (rand()%15)*2 + 1;
						y = (rand()%17)*2 + 1;
					}while(validatePos(x,y));
					Defense *def = createDefense(curr->name,y, x,curr->lvlRequired,curr->price
					,curr->b.health, curr->b.attack,curr->max_quantity,curr->name[0], curr->cooldown, curr->range);
					def->b.destroyed = false;
					
					pushDefense(def,enemyDef);
				}
				curr = curr->next;
			}
		}
			
	}
	
	
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
		}
		for (int i = 0;i < player->level;i++){
			if (Buildings::enemyGoldMines[i]) free(Buildings::enemyGoldMines[i]);
				Buildings::popAll(i, false);
		}
	}
	
	void printNodeMap(PathFinding::Node *map[35][31]){
		for(int i = 0; i < 35; i ++){
			for(int j = 0; j < 31; j++){
				gotoXY(42 + i,2 + j);
				printf("%c", map[i][j]->value);
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
		printf("%d/%d", dest,all);	
	}
	
	void eventListener(COORD coord){
		if (PveView::backBtn->isValid(coord) && !isAttack){
			popBuildings();
			while(Troop::attackHead){
				Troop::popAttackTroop();
			}
			exitPve = true;
		}
		if (isAttack && PveView::endBtn->isValid(coord)){
			while(Troop::attackHead){
				Troop::popAttackTroop();
			}
			exitPve = true;
		}
		if (coord.X >= 71 && coord.Y >= 9 && coord.Y < 9 + Troop::getCampLength()){
			curIdx = coord.Y - 9;
			Troop::viewCamp(71,9, curIdx);
		}
		
		if (coord.X >= 10 && coord.X <= 45 &&
			coord.Y >= 2 && coord.Y <= 33){
				if (validatePos(coord.Y - 2,coord.X - 10) == 1){
					PveView::printErrorMessage();
					return;
				}
				isAttack = true;
				PveView::clearErrorMsg();
				clear(
				PveView::backBtn->size.x,
				PveView::backBtn->pos.x,
				PveView::backBtn->pos.y,
				PveView::backBtn->size.y);
				PveView::endBtn->print();
				if (Troop::campHead){
					Troop::spawnTroop(&curIdx,coord,player, map);
					attackCount++;
					if (Troop::attackTail->target == NULL){
						exitPve = true;
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
	}
	
	bool thStarValidation(){
		if (!enemyTownHall->b.destroyed){
			return false;
		}
		return true;
	}
	
	
	
	void showInformation(){
		Sorting::Entity arr[all];
		int cur = 0;
		char str[] = "Town Hall";
		arr[cur].name = str;
		arr[cur++].health = Buildings::enemyTownHall->b.health;
		
		for (int i = 0;i < player->level;i++){
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
		Sorting::quickSort(arr, 0, all - 1);
		int counter = (all > 5)? 5:all;
		for (int i = 0;i < counter;i++){
			gotoXY(50,5 + i);
			printf("%-14s %4d",arr[i].name, arr[i].health);
		}
		if (all > 5){
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
	
	int destroyedCount(int *total){
		int destroyed = 0;
		if (thStarValidation()){
			destroyed++;
		}
		*total += 1;
		
		for (int i = 0;i < player->level;i++){
			if (enemyGoldMines[i]->b.destroyed){
				destroyed++;
			}
			*total += 1;
		}
		for (int i = 0;i < player->level;i++){
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
		popBuildings();
		exitPve = true;
		while(true){
			COORD coord = leftClickListener();
			if (Win::backBtn->isValid(coord)){
				return;
			}
		}
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
					printf("o");
				}
			}else{
				if (curr->target ->health > 0)
					curr->target->health -= curr->attack;
				if (curr->target->health <= 0){
					curr->target->health = 0;
					curr->target->alive = false;
					if (validateBuilding(curr->target->pos.x, curr->target->pos.y) == 0){
						gotoXY(curr->pos.x
						 + 10, curr->pos.y + 2);
						printf(" ");
					}else{
					}
					PveView::printBuildings(player->level);
				}
				curr->hit = true;
			}
			curr = curr->next;
		}
		showInformation();
	}
	
	void defend(Troop::Camp *troop){
			for (int i = 0;i < player->level;i++){
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
	
	
	void move(){
		int destroyed = 0;
		int prevX, prevY;
		int stars = winValidation(&destroyed);
		if (stars == 3){
			Sleep(1000);
			while(Troop::attackHead){
				Troop::popAttackTroop();
			}
				
			player->gold += destroyed*player->level*100;
			
			popAllBullet();
			exitPve = true;
			result(stars);
			return;
		}
		if (isAttack && exitPve || (!Troop::campHead && !checkAliveTroop())){
			Sleep(1000);
			while(Troop::attackHead){
				Troop::popAttackTroop();
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
				Troop::updateBuildings(player, curr, curr->pos.x+10, curr->pos.y+2, map);
				if (curr->target == NULL){
					return;
				} 		
				PathFinding::dfs(curr->nodeMap,curr->target->pos, curr->pos);
			}
			
			if ((strcmpi(curr->category, "Ranged") == 0 && eucledianDist(curr->pos, curr->target->pos) <=5) || 
				(curr->pos.x == curr->target->pos.x && curr->pos.y == curr->target->pos.y)){
				if (curr->target->health > 0)
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
					dest = destroyedCount(&total);
					printDestroyed();
					if (curr->target == NULL) return;	
					
					if (validateBuilding(curr->pos.x, curr->pos.y) || strcmpi(curr->category, "Ranged") == 0){
						SetConsoleTextAttribute(hStdOut, 7);
						gotoXY(curr->pos.x + 10, curr->pos.y + 2);
						printf(" ");
						gotoXY(curr->target->pos.x + 10, curr->target->pos.y + 2);
						printf(" ");
					}
					
					curr->target = NULL;
					inputmap(map,curr);
					Troop::insertBuildings(player, curr, curr->pos.x+10, curr->pos.y+2, map);
					if (curr->target == NULL){
						return;
					} 		
					if (strcmpi(curr->category, "Destroyer") == 0)
						PathFinding::astar(curr->nodeMap,curr->target->pos, curr->pos);
					else
						PathFinding::dfs(curr->nodeMap,curr->target->pos, curr->pos);
				}
			}else{
				int dirX[] = {1,0,-1,0};
				int dirY[] = {0,1,0,-1};
				PathFinding::Node *curPos = curr->nodeMap[curr->pos.x][curr->pos.y]->before;
//				if (strcmp(curr->category, "Ranged") == 0){
//					for (int i = 0;i < 4;i++){		
//						Position pos = {curr->pos.x + dirX[i], curr->pos.y + dirY[i]};
//						if (curr->nodeMap[pos.x][pos.y]->value == ' ' && eucledianDist(curPos->pos,curr->target->pos) > 
//						eucledianDist(pos,curr->target->pos)){
//							curPos = curr->nodeMap[pos.x][pos.y];
//						}
//					}
//				}
				
				curr->prevPos = curr->pos;
				curr->pos = {curPos->pos.y, curPos->pos.x};	
				if (curr->firstMove) curr->firstMove = false;
				
				if (curr->target->destroyed){
					
					inputmap(map,curr);
					curr->target = NULL;
					Troop::insertBuildings(player, curr, curr->pos.x+10, curr->pos.y+2, map);
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
//			if (validatePos(curr->pos.x, curr->pos.y) == 0){
//			}
			curr = curr->next;
		}
	}
	
	int getDefensesCount(){
		int j = 0;
		
		for (int i = 0;i < player->level;i++){
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
		PveView::menu();
		PveView::showPlayer(player);
		PveView::printMap(map);
		PveView::printBuildings(player->level);
		Troop::viewCamp(71,9, curIdx);
		dest = 0;
		all = getDefensesCount() + 1 + player->level;
		printDestroyed();
		showInformation();
		while(!exitPve){
			COORD coord = leftClickListener();
			eventListener(coord);
			Sleep(100);
			if (isAttack){
				move();
			}
		}
	}
	
	void start(Player::Node *p){
		map = EllerBase::eller();
		player = p;
		attackCount = 0;
		exitPve = false;
		
		initBuildings();
		isAttack = false;
		game();
	}
}


#endif
