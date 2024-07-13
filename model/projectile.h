#ifndef PROJECTILE
#define PROJECTILE
#include "../header/pathFinding.h"
#include "./troop.h"
#include "../header/buildings.h"
#include "../header/eucledianDist.h"
#include "../header/pathFinding.h"

struct Bullet{
	Position pos;
	int attack;
	bool firstMove, hit;
	Troop::Camp *target;
	PathFinding::Node *nodeMap[35][31];	
	Bullet *next, *prev;
} *bulletHead = NULL, *bulletTail = NULL;

Bullet *createBullet(int x, int y, int attack){
	Bullet *newB = (Bullet*)malloc(sizeof(Bullet));
	newB->target = NULL;
	newB->pos = {x,y};
	newB->attack = attack;
	newB->next = newB->prev = NULL;
	newB->firstMove = true;
	newB->hit = false;
	return newB;
}

void inputMapBullet(Bullet *b, char **map){
	for(int i = 0; i < 31; i ++){
		for(int j = 0; j < 35; j++){
			b->nodeMap[j][i] = PathFinding::constructorNode(map[i][j], false, 
			NULL,i,j);
		}
	}
}

void pushBullet(Bullet *b){
	if (!bulletHead) bulletHead = bulletTail = b;
	else{
		bulletTail->next = b;
		b->prev = bulletTail;
		bulletTail = b;
	}
}

void popBulletHead(){
	if (bulletHead == bulletTail){
		bulletHead = bulletTail = NULL;
		free(bulletHead);
		return;
	}
	bulletHead = bulletHead->next;
	free(bulletHead->prev);
}

void spawnBullet(Troop::Camp *troop, Buildings::Defense *def, char **map){
	if(eucledianDist(troop->pos, def->b.pos) < def->range){
		pushBullet(createBullet(def->b.pos.x, def->b.pos.y, def->b.attack));
		inputMapBullet(bulletTail, map);
		bulletTail->target = troop;
		PathFinding::dijkstra(bulletTail->nodeMap, bulletTail->target->pos, bulletTail->pos);
	}
}

void popAllBullet(){
	while(bulletHead){
		popBulletHead();
	}
}

#endif
