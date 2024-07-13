#include "../header/helper.h"
#include <vector>

#ifndef MAZEGENERATION
#define MAZEGENERATION


namespace EllerBase{
	
	char **map;
	
	void initMap(){
		map = (char**)malloc(sizeof(char*)*35);
		for (int i = 0;i < 31;i++){
			map[i] = (char*)malloc(sizeof(char)*40);
			for (int j = 0;j < 35;j++){
				map[i][j] = 219;
			}
			map[i][35] = '\0';/*width*/
		}
	}
	
	struct Set{
		int parent;
		int count;
	};
	
	int choice(){
		return rand()%2;
	}
	
	char** eller(){
		initMap();
		Set sets[17];
		for (int i = 1;i < 31;i+=2){
			int cur = 1;
			
			//Step 1: bikin unique set
			for (int j = 0;j < 17;j++){
				
				if (map[i-1][j*2 + 1] == -37){
					sets[j] = {cur,1};
				}
				cur++;
				map[i][j*2+1] = ' ';
				sets[j].count = 1;
			}
			
			if (i == 29){
				for (int j = 0;j < 16;j++){
					if (sets[j].parent != sets[j+1].parent){
						map[i][(j+1)*2] = ' ';
					}
				}
				break;
			}
			
			//Step 2: bikin right wall (connection)
			for (int j = 0;j < 16;j++){
				if (sets[j].parent != sets[j+1].parent){
					if (choice() || i == 29){
						//Union Sets
						sets[sets[j].parent - 1].count += 1;
						sets[sets[j+1].parent - 1].count = 0;
						sets[j+1].parent = sets[j].parent;
						map[i][(j+1)*2] = ' '; 
						
					}
				}
			}
			//Step 3: Create Bottom Wall (Passage)
			for (int j = 0;j < 17;j++){
				if (sets[j].count > 0){
					map[i+1][j*2+1] = ' ';
				}
			}
		}	
		return map;
	}

}

namespace PlayerBase{
	char **mazeMap;
	Position maze[31][35];
	
	void initMap(){
		mazeMap = (char**)malloc(sizeof(char*)*35);
		for (int i = 0;i < 31;i++){
			mazeMap[i] = (char*)malloc(sizeof(char)*40);
			for (int j = 0;j < 35;j++){
				mazeMap[i][j] = 219;
			}
			mazeMap[i][35] = '\0';
		}
	
	}
	
	char **loadMap(char name[]){
		char path[50];
		sprintf(path, "./database/%s/map.csv", name);		
		char **tempMap;
		
		FILE *fp = fopen(path, "r");
		tempMap = (char**)malloc(sizeof(char*)*35);
		for (int i = 0;i < 31;i++){
			tempMap[i] = (char*)malloc(sizeof(char)*40);
			for (int j = 0;j < 35;j++){
				if (j > 0){
					fscanf(fp,",");
				}
				char read;
				fscanf(fp,"%c",&tempMap[i][j]);
			}
			tempMap[i][35] = '\0';
			fscanf(fp,"\n");
		}
		fclose(fp);
		return tempMap;
	}
	
	void createMaze(){
		for (int i = 0;i < 31;i++){
			for (int j = 0;j < 35;j++){
				maze[i][j] = {i,j};				
			}
		}
	}
	int dirX[] = {2,0,-2,0};
	int dirY[] = {0,2,0,-2};	
	
	char** prim(){
		createMaze();
		initMap();
		std::vector <Position> queue;
		
		int mazeHeight = 31, mazeWidth = 35;
	
		queue.push_back(maze[1][1]);
		bool visited[31][35] = {0};
		bool first = true;
		int dir = 4;
		
		while (queue.size()){
			int randPos = rand()%queue.size();
			Position temp = queue[randPos];
			queue.erase(queue.begin() + randPos);
			
			if (visited[temp.x][temp.y]) continue;
			
			visited[temp.x][temp.y] = true;
			mazeMap[temp.x][temp.y] = ' ';
			int randDir, valid;
				
			if (first){
				first = false;
			}else{
				do{
					valid = true;
					randDir = rand()%dir;
					if (temp.x-dirX[randDir] < 1 
						|| temp.y-dirY[randDir] < 1 
						|| temp.x-dirX[randDir] >= mazeHeight-1 
						|| temp.y-dirY[randDir] >= mazeWidth-1
						|| mazeMap[temp.x-dirX[randDir]][temp.y - dirY[randDir]] != ' ')
						valid = false;
					else{
						valid = true;
					}
				}while (!valid);
				mazeMap[temp.x-dirX[randDir]/2][temp.y - dirY[randDir]/2] = ' ';
			}
			for (int i = 0;i < dir;i++){
				if (!visited[temp.x-dirX[i]][temp.y-dirY[i]] && temp.x-dirX[i] > 0 && temp.y-dirY[i] > 0 
					&& temp.x-dirX[i] < mazeHeight-1 && temp.y-dirY[i] < mazeWidth-1 )
				queue.push_back(maze[temp.x-dirX[i]][temp.y-dirY[i]]);
			}
		}
//		for (int i = 0;i < 31;i++){
//			for (int j = 0;j < 35;j++){
//				char temp = mazeMap[i][j];
//				mazeMap[i][j] = mazeMap[j][i];
//				mazeMap[j][i] = temp;
//			}
//		}
		return mazeMap;
	}
}

#endif
