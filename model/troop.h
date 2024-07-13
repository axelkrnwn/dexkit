#ifndef TROOP
#define TROOP
#include "../header/buildings.h"
#include "../header/eucledianDist.h"
#include "../header/pathFinding.h"
#include "./player.h"
using namespace std;
using namespace Buildings;

namespace Troop{
		
	struct InnerNode{
		char name[100];
		int health,attack,defense,space,levelRequired,time;
		InnerNode *next, *prev;
	};
	
	struct Node{
		char category[100];
		int size;
		InnerNode *head, *tail;
		Node *next, *prev;
	} *head = NULL, *tail = NULL, 
	*playerHead = NULL, *playerTail = NULL;
		
	InnerNode *createInnerNode(const char name[], int hp, int attack, int def,
	int space, int lvl, int time){
		InnerNode *newNode = (InnerNode*)malloc(sizeof(InnerNode));
		newNode->prev = newNode->next = NULL;
		strcpy(newNode->name, name);
		newNode->health= hp;
		newNode->attack = attack;
		newNode->defense = def;
		newNode->space = space;
		newNode->levelRequired = lvl;
		newNode->time = time;
		return newNode;
	}
	
	Node *createNode(const char category[]){
		Node *newNode = (Node*)malloc(sizeof(Node));
		newNode->prev = newNode->next = NULL;
		newNode->head = newNode->tail = NULL;
		strcpy(newNode->category, category);
		newNode->size = 0;
		return newNode;
	}	
	Node *getNode(char category[]){
		Node *curr = head;
		while(curr){
			if (strcmp(curr->category, category) == 0) break;
			curr = curr->next;
		}
		return curr;
	}
	
	InnerNode *getTroop(char name[], Node *node){
		InnerNode *curr = node->head;
		while(curr){
			if (strcmp(curr->name, name) == 0) break;
			curr = curr->next;
		}
		return curr;
	}
	void pushInnerNode(Node *node, InnerNode *newNode){
		node->size += 1;
		if (!node->head){
			node->head = node->tail = newNode;
		}else if (node->head->levelRequired >= newNode->levelRequired){
			node->head->prev = newNode;
			newNode->next = node->head;
			node->head = newNode;
		}else if (node->tail->levelRequired <= newNode->levelRequired){
			node->tail->next = newNode;
			newNode->prev = node->tail;
			node->tail = newNode;
		}else{
			InnerNode *curr = node->head;
			while(curr->levelRequired < newNode->levelRequired)
				curr = curr->next;
			
			newNode->prev = curr->prev;
			newNode->next = curr;
			curr->prev->next = newNode;
			curr->prev = newNode;
		}
	}
	void pushNode(Node *newNode, InnerNode *inner){
		if (!head){
			head = tail = newNode;
			pushInnerNode(tail, inner);
		}else{
			Node *search = getNode(newNode->category);
			if (search){
				pushInnerNode(search, inner);
			}else{
				tail->next = newNode;
				newNode->prev = tail;
				tail = newNode;
				pushInnerNode(tail, inner);
			}
		}
		
	}
	
	void readFromFile(){
		FILE *fp = fopen("./database/troops.csv","r");
		
		char name[100], category[100];
		int health,attack,defense,space,levelRequired,time;
		while(!feof(fp)){
			fscanf(fp,"%[^,],%[^,],%d,%d,%d,%d,%d\n",
			name, category,&space,&levelRequired,
			&attack,&health,&time);
			
			pushNode(createNode(category),createInnerNode(
			name,health,attack,0,space,levelRequired,time));
		}
		
		fclose(fp);
	}
	
	void viewTroop(int level){
		int i= 0;
		Node *curr = head;
		
		while(curr){
			gotoXY(2, 2 + i);
			i++;
			printf("%s", curr->category);
			InnerNode *cur = curr->head;
			while(cur){
				gotoXY(2, 2 + i);
				i++;
				if (level < cur->levelRequired){
					SetConsoleTextAttribute(hStdOut,4);	
				}else{
					SetConsoleTextAttribute(hStdOut,10);	
				}
				printf("%s", cur->name);
				SetConsoleTextAttribute(hStdOut,7);
				cur = cur->next;
			}
			i++;
			curr = curr->next;
		}
	}
	
	
	struct Camp{
		char category[100], name[100];
		bool firstMove, alive;
		int health,attack,defense,space, quantity;
		Position pos, prevPos;
		Camp *next, *prev;
		Building *target;
		PathFinding::Node *nodeMap[35][31];
	} *campHead = NULL, *campTail = NULL, *attackHead = NULL, *attackTail = NULL;
	
	
		
	Camp *createCamp(char name[], char category[], int health,
	int attack, int defence, int space){
		Camp *newNode = (Camp*)malloc(sizeof(Camp));
		newNode->prev = newNode->next = NULL;
		strcpy(newNode->name, name);
		strcpy(newNode->category, category);
		newNode->firstMove = newNode->alive = true;
		newNode->target = NULL;
		newNode->health= health;
		newNode->attack = attack;
		newNode->defense = defence;
		newNode->space = space;
		newNode->quantity = 1;
		newNode->target = NULL;
		newNode->pos = {-1,-1};
		newNode->prevPos = {-1,-1};
		return newNode;
	}
	
	void insertRangedBuilding(Camp *troop, Building *building, char **map){
		PathFinding::inputMap(map);
		int newT = PathFinding::getWeight(troop->pos, building->pos);
		PathFinding::inputMap(map);
		int oldT = PathFinding::getWeight(troop->pos, troop->target->pos);
				
		if (newT < oldT) 
			troop->target  = building;
	}
	
	void insertBuilding(Camp *troop, Building *building, char **map){
		
		if (strcmp(troop->category, "Ranged") != 0){
			if (!troop->target){
				if (!building->destroyed) troop->target  = building;
			}else if (!building->destroyed){
				PathFinding::inputMap(map);
				int newT = PathFinding::getWeight(troop->pos, building->pos);
				PathFinding::inputMap(map);
				int oldT = PathFinding::getWeight(troop->pos, troop->target->pos);
				
				if (newT < oldT) 
				troop->target  = building;
				
			}
			
		}else{
			if (!troop->target){
				if (!building->destroyed) troop->target  = building;
			}else if (!building->destroyed){
				if (eucledianDist(troop->target->pos, troop->pos) > 
				eucledianDist(troop->pos, building->pos))
					troop->target  = building;
			}
		}
		
	}
	
	void updateBuilding(Camp *troop, Building *building, char **map){
		if (!troop->target){
			if (!building->destroyed) troop->target  = building;
		}else if (!building->destroyed){
			if (eucledianDist(building->pos, troop->pos) <= 5)
				troop->target  = building;
		}
	}
	
	void updateBuildings(Player::Node *player, Camp *troop, int x, int y, char **map){
		troop->pos = {x - 10,y - 2};
		updateBuilding(troop, &enemyTownHall->b, map);
		for (int i = 0;i < player->level;i++){
			updateBuilding(troop, &enemyGoldMines[i]->b, map);
			Defense *curr = enemyDef[i];
			while(curr){
				updateBuilding(troop, &curr->b, map);
				curr = curr->next;
			}
		}
	}
	
	void insertBuildings(Player::Node *player, Camp *troop, int x, int y, char **map){
		troop->pos = {x - 10,y - 2};
		
		insertBuilding(troop, &enemyTownHall->b, map);
		for (int i = 0;i < player->level;i++){
			insertBuilding(troop, &enemyGoldMines[i]->b, map);
			Defense *curr = enemyDef[i];
			while(curr){
				insertBuilding(troop, &curr->b, map);
				curr = curr->next;
			}
		}
//		if (strcmp(troop->category, "Ranged") == 0){
//			insertRangedBuilding(troop, &enemyTownHall->b, map);
//			for (int i = 0;i < player->level;i++){
//				insertRangedBuilding(troop, &enemyGoldMines[i]->b, map);
//				Defense *curr = enemyDef[i];
//				while(curr){
//					insertRangedBuilding(troop, &curr->b, map);
//					curr = curr->next;
//				}
//			}
//		}
	}
	
	void enqueueCamp(Camp *newC){
		if (!campHead){
			campHead = campTail = newC;
		}else if (strcmp(campHead->category,newC->category) > 0){
			if (strcmp(campHead->name,newC->name) == 0){
				campHead->quantity += 1;
				return;
			}
			campHead->prev = newC;
			newC->next = campHead;
			campHead = newC;
		}else if (strcmp(campTail->category,newC->category) < 0){
			if (strcmp(campTail->name,newC->name) == 0){
				campTail->quantity += 1;
				return;
			}
			newC->prev = campTail;
			campTail->next = newC;
			campTail = newC;
		}else{
			Camp *curr = campHead;
			while(strcmp(curr->category, newC->category) <= 0){
				if (strcmp(curr->name,newC->name) == 0){
					curr->quantity += 1;
					return;
				}
				curr = curr->next;
			}			
			newC->prev = curr->prev;
			newC->next = curr;
			curr->prev->next = newC;
			curr->prev = newC;	
		}
	}
	
	void drawTroop(Camp *troop){
		gotoXY(troop->pos.x
		 + 10, troop->pos.y + 2);
		 printf("%c", troop->name[0]);
	}
	
	void pushAttackTroop(Camp *troop){
		if (!attackHead){
			attackHead = attackTail = troop;
		}else{
			troop->prev = attackTail;
			attackTail->next = troop;
			attackTail = troop;
		}
	}
	
	void popAttackTroop(){
		if(attackHead == attackTail){
			free(attackHead);
			attackHead = attackTail = NULL;	
		}else{
			attackHead = attackHead->next;
			free(attackHead->prev);
		}
	}
	
	void viewCamp(int x, int y, int curIdx){
		Camp *cur = campHead;
		int i = 0;
		while(cur){
			gotoXY(x,y+i);
			if (i == curIdx){
				SetConsoleTextAttribute(hStdOut, 4);
			}else{
				SetConsoleTextAttribute(hStdOut, 6);
			}
			printf("%s x%d",cur->name,cur->quantity);
			cur = cur->next;
			i++;
		}
		SetConsoleTextAttribute(hStdOut, 7);
	}
	
	
	int getCapacity(){
		Camp *cur = campHead;
		int i = 0;
		while(cur){
			i += cur->space*cur->quantity;
			cur = cur->next;
		}
		return i;
	}
	
	int getCampLength(){
		Camp *cur = campHead;
		int i = 0;
		while(cur){
			i++;
			cur = cur->next;
		}
		return i;
	}
	
	void popAllCamp(){
		while(campHead){
			if (campHead == campTail){
				free(campHead);
				campHead = campTail = NULL;
			}else{
				campHead = campHead->next;
				free(campHead->prev);
			}
		}
	}
	
	void dequeueCamp(int *curIdx){
		Camp *curr = campHead;
		int i = 0;
		while(curr){
			if (i == *curIdx) break;
			curr = curr->next;
			i++;
		}		

		if (curr->quantity <= 1){
			if (campHead == campTail){
				free(campHead);
				campHead=campTail = NULL;
			}else if (curr == campHead){
				campHead = campHead->next;
				free(campHead->prev);
				campHead->prev = NULL;
			}else if (curr == campTail){
				campTail = campTail->prev;
				free(campTail->next);
				campTail->next = NULL;
			}else{
				curr->prev->next = curr->next;
				curr->next->prev = curr->prev;
				curr->next = curr->prev = NULL;
				free(curr);
			}
			*curIdx = 0;
		}else{
			curr->quantity -= 1;
		}
		
		
		clear(25, 71, 9, 20);
		viewCamp(71,9, *curIdx);
	}
	
	void spawnTroop(int *idx, COORD coord, Player::Node* opp, char **map){
		int i = 0;
		Camp *curr = campHead;
		while(curr){
			if (i == *idx) break;
			curr = curr->next;
			i++;
		}
		if (curr){
			Camp *troop = createCamp(curr->name, curr->category, curr->health, curr->attack,
			curr->defense, curr->space);
			pushAttackTroop(troop);
			insertBuildings(opp,attackTail,coord.X, coord.Y,map);
			dequeueCamp(idx);
		}
	}
	
	struct Train{
		char category[100], name[100];
		int capacity;
		int time;
		Train *next, *prev;
		
		void train(){
			if (this->time > 0)
			this->time -= 1;
		}
	}*queueHead = NULL, *queueTail = NULL;
	
	Train *createTrain(char category[], char name[], int time, int capacity){
		Train *newT = (Train*)malloc(sizeof(Train));
		newT->time = time;
		newT->capacity = capacity;
		strcpy(newT->category, category);
		strcpy(newT->name, name);
		newT->next = newT->prev = NULL;
		return newT;
	}
	
	
	void enqueue(Train *newQueue){
		if (!queueHead){
			queueHead = queueTail = newQueue;
		}else{
			queueTail->next = newQueue;
			newQueue->prev = queueTail;
			queueTail = newQueue;
		}
	}
	
	void dequeue(){
		if (!queueHead){
			return;
		}
		if (queueHead == queueTail){
			queueHead = queueTail = NULL;	
			return;
		}
		queueHead = queueHead->next;
		queueHead->prev = NULL;
	}
	
	
	void viewTrain(){
		Train *cur = queueHead;
		gotoXY(21,1);
		int i = 0;
		while(cur && i < 16){
			gotoXY(21,2+i);
			printf("%s",cur->name);
			gotoXY(38,2+i);
			printf("%.2d:%.2d",(cur->time)/60,
			((cur->time))%60);
			cur = cur->next;
			i++;
		}
	}
	
	void doneTrain(){
		Node *category = getNode(queueHead->category);
		InnerNode *troop = getTroop(queueHead->name, category);
		enqueueCamp(createCamp(troop->name, category->category
		,troop->health, troop->attack, troop->defense 
		, troop->space));
		dequeue();
	}
	
	void saveProgress(char name[]){
		char path[100];
		sprintf(path, "./database/%s/troops.csv", name);
		FILE *fp = fopen(path, "w");
		if (!fp){
			return;
		}
		if (!campHead){
			remove(path);
			return;
		}
		Camp *curr = campHead;
		while(curr){
			fprintf(fp,"%s,%s,%d,%d,%d,%d\n",curr->name, curr->category, 
			curr->health, curr->attack, curr->space, curr->quantity);
			curr = curr->next;
		}
		fclose(fp);
	}
	
	void readCamp(char name[]){
		char path[100];
		sprintf(path,"./database/%s/troops.csv", name);
		FILE *fp = fopen(path, "r");
		char troopName[100], category[100];
		int health, attack, space, quantity, defense;
		
		if (!fp){
			return;	
		} 
		
		while(!feof(fp)){
			fscanf(fp,"%[^,],%[^,],%d,%d,%d,%d\n", troopName, category, 
			&health, &attack, &space, &quantity);
			for (int i = 0;i < quantity;i++){
				enqueueCamp(createCamp(troopName, category,health,attack,defense,space));
			}
		}
		fclose(fp);
	}
	
	
	int getTrainCapacity(){
		int i = 0;
		Train *curr = queueHead;
		while(curr){
			i+= curr->capacity;
			curr = curr->next;
		}
		return i;
	}
	
	int getAttackCapacity(){
		int i = 0;
		Train *curr = queueHead;
		while(curr){
			i+= curr->capacity;
			curr = curr->next;
		}
		return i;
	}
}

#endif
