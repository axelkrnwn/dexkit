#include "./helper.h"

#ifndef BUILDINGS
#define BUILDINGS

namespace Buildings{
	
	struct Building{
		Position pos;
		int health;
		int attack;
		bool destroyed;
		Building *next, *prev;
	};
	
	Building *createBuilding(int health, int attack, int x, int y){
		Building *newB = (Building*)malloc(sizeof(Building));
		newB->health = health;
		newB->attack = attack;
		newB->pos = {x,y};
		newB->destroyed = false;
		newB->next = newB->prev = NULL;
		return newB;
	}
	
	struct GoldMine{
		Building b;
		int count;
		int gold;
		
		void addGold(Player::Node *player){
			if (this->count == 2){
				player->gold += this->gold;
				this->count = 0;
			}
			this->count+=1;
		}
	} *goldMines[10] = {NULL}, *enemyGoldMines[10];
	
	GoldMine* createGoldMine(int x, int y){
		GoldMine *g =(GoldMine*)malloc(sizeof(GoldMine));
		g->b.pos = {x,y};
		g->b.health = 100;
		g->b.attack = 0;
		g->b.destroyed = false;
		g->gold = 10;
		g->count = 0;
		return g;
	}
	
	struct TownHall{
		Building b;
		int count;
		int gold;
		
		void addGold(Player::Node *player){
			if (this->count == 4){
				player->gold += this->gold;
				this->count = 0;
			}
			this->count+=1;
		}
	} *townHall = NULL, *enemyTownHall = NULL;
	
	bool collectGold(Player::Node *player){
		int gold = player->gold;
		
		for (int i = 0;i < player->level;i++){
			goldMines[i]->addGold(player);
		}
		townHall->addGold(player);
		
		return gold != player->gold;
	}
	
	
	TownHall *createTownHall(int x, int y){
		TownHall *g = (TownHall*)malloc(sizeof(TownHall));
		g->b.pos = {x,y};
		g->gold = 5;
		g->b.health = 700;
		g->b.destroyed = false;
		g->b.attack = 0;
		g->count = 0;
		return g;
	}
	
	struct Defense{
		int lvlRequired, price, max_quantity, quantity, counter, cooldown, range;
		char symbol;
		char name[100];
		Building b;
		Defense *next, *prev;
	} *playerDef[10] = {NULL}, *allDef[10] = {NULL}, *enemyDef[10] = {NULL};
	
	Defense *createDefense(char name[], int x, int y, 
	int lvlRequired, int price, int health, int attack, int max_quantity,
	char symbol, int cooldown, int range){
		Defense *newDef = (Defense*)malloc(sizeof(Defense));
		newDef->b.pos = {x,y};
		newDef->price = price;
		newDef->b.health = health;
		newDef->b.attack = attack;
		newDef->quantity = 0;
		newDef->counter = 0;
		newDef->cooldown = cooldown;
		newDef->range = range;
		newDef->lvlRequired = lvlRequired;
		newDef->max_quantity = max_quantity;
		
		newDef->symbol = symbol;
		strcpy(newDef->name, name);
		newDef->prev = newDef->next = NULL;
		return newDef;
	}
	
	void pushDefense(Defense *defense, Defense *defenses[]){
		int key = defense->lvlRequired - 1;
		if (!defenses[key]){
			defenses[key] = defense;
		}else{
			Defense *curr = defenses[key];		
			while(curr->next){
				curr = curr->next;
			}
			curr->next = defense;
			defense->prev = curr;
		}
	}
	
	
	Defense *getDefense(int key, char symbol, Defense *defenses[10]){
		if (!defenses[key]){
			return NULL;
		}else{
			if (defenses[key]->symbol == symbol){
				return defenses[key];
			}else{
				Defense *curr = defenses[key];
				while(curr){
					if(curr->symbol == symbol){
						break;
					}
					curr = curr->next;
				}
				return curr;
			}
		}
	}
	
	int readAllDefense(){
		int lvlRequired, price, health, attack, max_quantity, quantity, cooldown, range;
		char symbol;
		char name[100];
		int size = 0;
		
		FILE *fp = fopen("./database/all_defenses.csv","r");
		
		while(!feof(fp)){
			fscanf(fp,"%[^,],%d,%d,%d,%d,%d,%c,%d,%d\n",
			name,&price,&lvlRequired,&health,
			&attack,&max_quantity,&symbol,
			&cooldown,&range);
			size++;
			pushDefense(createDefense(name,-1,-1,lvlRequired,price,health,attack,max_quantity,symbol, cooldown, range), allDef);
		}
		
		fclose(fp);
		return size;
	}
	
	void readBuildings(Player::Node *player, Defense *defenses[],
	bool isPlayer){
		char path[100];
		sprintf(path, "./database/%s/buildings.csv",player->name);
		FILE *fp = fopen(path, "r");
		
		int x,y;
		char name[100];
		fscanf(fp,"%[^,],%d,%d\n",name,&y, &x);	
		if (isPlayer){
			townHall = createTownHall(x,y);
		}else{
			enemyTownHall = createTownHall(x,y);
			enemyTownHall->b.destroyed = false;
		}
		
		for (int i = 0;i < player->level;i++){
			fscanf(fp,"%[^,],%d,%d\n",name,&y, &x);	
			if (isPlayer){
				goldMines[i] = createGoldMine(x,y);
			}else{
				enemyGoldMines[i] = createGoldMine(x,y);
				enemyGoldMines[i]->b.destroyed = false;				
			}
		}
		while(!feof(fp)){
			int lvl;
			fscanf(fp,"%[^,],%d,%d,%d\n",name,&lvl,&y,&x);
			Defense *def = getDefense(lvl-1,name[0],allDef);
			Defense *newDef = createDefense(name,x,y,def->lvlRequired,def->price
			,def->b.health, def->b.attack,def->max_quantity,name[0], def->cooldown, def->range);
			newDef->b.destroyed = false;
			pushDefense(newDef,defenses);
		}
		fclose(fp);
	}
	
	void popAll(int i, bool isPlayer = true){
		if (isPlayer){
			if (!playerDef[i]) return;
			while(playerDef[i]){
				Defense *temp = playerDef[i];
				playerDef[i] = playerDef[i]->next;
				
				free(temp);
				temp = NULL;
			}
			playerDef[i] = NULL;
		}else{
			if (!enemyDef[i]) return;
			while(enemyDef[i]){
				Defense *temp = enemyDef[i];
				enemyDef[i] = enemyDef[i]->next;
				
				free(temp);
				temp = NULL;
			}
			enemyDef[i] = NULL;
		}
	}
	
	void saveProgress(char name[], int lvl){
		char path[100];
		sprintf(path, "./database/%s/buildings.csv", name);
		FILE *fp = fopen(path, "w");
		fprintf(fp,"%s,%d,%d\n","Town Hall",townHall->b.pos.y, townHall->b.pos.x);	
		for (int i = 0;i < lvl;i++)
		fprintf(fp,"%s,%d,%d\n","Gold Mine",goldMines[i]->b.pos.y,goldMines[i]->b.pos.x);
		
		for (int i = 0;i < 10;i++){
			if (playerDef[i]){
				Defense *curr = playerDef[i];
				while(curr){
					fprintf(fp,"%s,%d,%d,%d\n",curr->name,curr->lvlRequired,
					curr->b.pos.y,curr->b.pos.x);
					curr=curr->next;
				}
			}
		}
		
		fclose(fp);
	}
	
	
}
#endif
