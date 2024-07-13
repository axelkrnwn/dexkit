#ifndef PATHFINDING
#define PATHFINDING
#include <vector>
#include "./eucledianDist.h"
using namespace std;

int V[] = {0,-1,  0, 1};
int H[] = {1, 0,-1,  0};

namespace PathFinding{
	
	struct Node{
		Position pos;
		Node *before, *next;
		bool visited;
		char value;
		int weight;
	} *nodeMap[35][31];
	
	Node *constructorNode(char value, bool visited, Node *before,int x, int y){
	  Node *newNode = (Node*)malloc(sizeof(Node));
	  newNode->pos = {x,y};
	  newNode->value = value;
	  newNode->visited = visited;
	  newNode->before = NULL;
	  newNode->weight = 1;
	  return newNode;
	}
	
	
	void printMap(Node map[35][31]){
			
		for (int i = 0;i < 31;i++){
			for (int j = 0;j < 31;j++){
				gotoXY(10 + j,i + 2); 
				printf("%c", map[i][j].value);
				
			}
		}
	}
	
	
	bool isValid(Node *map[35][31],Position pos){
		if(pos.x < 0 || pos.y < 0 || pos.x >= 35 || pos.y >= 31 
		|| map[pos.x][pos.y]->visited || map[pos.x][pos.y]->value != ' ') 
			return false;
		
		return true;
	}
	int i = 0;
	void backTrack(Node *curr){
		if(curr->before == NULL) return;
		gotoXY(curr->pos.y+10, curr->pos.x + 2);
		printf(".");
		gotoXY(50,i++);
		printf("%d %d", curr->pos.x, curr->pos.y);
		Sleep(200);
		backTrack(curr->before);
	}
	
//attack
	void dfs(Node* map[35][31],Position x, Position y){
		
		vector<Position> queue;
		map[x.x][x.y]->visited = true;
		queue.push_back(x);
		bool found = false;
		
		while(queue.size() > 0){
			Position curr = queue[0];
			queue.erase(queue.begin());
	
			if(curr.x == y.x && curr.y == y.y){
				found = true;
				break;
			}
			for(int i = 0; i < 4; i++){
				Position newPos = {
					curr.x + V[i],
					curr.y + H[i]
				};
				if(!isValid(map,newPos)) 
				continue;
				
				queue.push_back(newPos);
				map[newPos.x][newPos.y]->visited = true;
				map[newPos.x][newPos.y]->before = map[curr.x][curr.y];
			}
		}				
	}
	
	void inputMap(char **map){
		for(int i = 0; i < 31; i ++){
			for(int j = 0; j < 35; j++){
				nodeMap[j][i] = PathFinding::constructorNode(map[i][j], false, 
				NULL,i,j);
			}
		}
	}
	
	int getWeight(Position x, Position y){
	
		vector<Position> queue;
		nodeMap[x.x][x.y]->visited = true;
		queue.push_back(x);
		bool found = false;
		
		
		while(queue.size() > 0){

			int chosen = 0;			
			for (int i = 1;i < queue.size();i++){
				Position cur = queue[i];
				if (nodeMap[queue[chosen].x][queue[chosen].y]->weight > nodeMap[cur.x][cur.y]->weight){
					chosen = i;
				}		
			}
			Position curr = queue[chosen];
			queue.erase(queue.begin() + chosen);
	
			if(curr.x == y.x && curr.y == y.y){
				found = true;
				break;
			}
			
			for(int i = 0; i < 4; i++){
				Position newPos = {
					curr.x + V[i],
					curr.y + H[i]
				};
				if(!isValid(nodeMap,newPos)) 
				continue;
				
				queue.push_back(newPos);
				nodeMap[newPos.x][newPos.y]->visited = true;
				nodeMap[newPos.x][newPos.y]->weight = nodeMap[curr.x][curr.y]->weight + 1;
			}
		}
		return nodeMap[y.x][y.y]->weight;			
	}
	
//defense	
	void dijkstra(Node* map[35][31],Position x, Position y){
		vector<Position> queue;
		map[x.x][x.y]->visited = true;
		queue.push_back(x);
		bool found = false;
		
		
		while(queue.size() > 0){

			int chosen = 0;			
			for (int i = 1;i < queue.size();i++){
				Position cur = queue[i];
				if (map[queue[chosen].x][queue[chosen].y]->weight > map[cur.x][cur.y]->weight){
					chosen = i;
				}		
			}
			Position curr = queue[chosen];
			queue.erase(queue.begin() + chosen);
	
			if(curr.x == y.x && curr.y == y.y){
				found = true;
				break;
			}
			
			for(int i = 0; i < 4; i++){
				Position newPos = {
					curr.x + V[i],
					curr.y + H[i]
				};
				if(!isValid(map,newPos)) 
				continue;
				
				queue.push_back(newPos);
				map[newPos.x][newPos.y]->visited = true;
				map[newPos.x][newPos.y]->weight = map[curr.x][curr.y]->weight + 1;
				map[newPos.x][newPos.y]->before = map[curr.x][curr.y];
			}
		}
	}
	//destroyer
	void astar(Node *map[35][31], Position x, Position y){
		vector<Position> queue;
		map[x.x][x.y]->visited = true;
		queue.push_back(x);
		bool found = false;
		
		
		while(queue.size() > 0){
			
			int chosen = 0;			
			for (int i = 1;i < queue.size();i++){
				Position cur = queue[i];
				if (eucledianDist(cur, y) < eucledianDist(y,queue[chosen])){
					chosen = i;
				}		
			}
			Position curr = queue[chosen];
			queue.erase(queue.begin() + chosen);
	
			if(curr.x == y.x && curr.y == y.y){
				found = true;
				break;
			}
			
			for(int i = 0; i < 4; i++){
				Position newPos = {
					curr.x + V[i],
					curr.y + H[i]
				};
				if(!isValid(map,newPos)) 
				continue;
				
				queue.push_back(newPos);
				map[newPos.x][newPos.y]->visited = true;
				map[newPos.x][newPos.y]->before = map[curr.x][curr.y];
			}
		}
	}
}


#endif
