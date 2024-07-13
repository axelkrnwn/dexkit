#ifndef MINIMUMOPERATION
#define MINIMUMOPERATION
#include <math.h>

int greaterOrLess(int trophy, int add, int memo[]){
	
	for (int i = 2;i <= trophy;i++){
		memo[i] = 2147483647;
		if (i % add == 0){
			int x = memo[i / add];
			if (x + 1 < memo[i]){
				memo[i] = x + 1;
			}
		}
		int x = memo[i - 1];
		if(x + 1 < memo[i]){
			memo[i] = x + 1;
		}
	}
	return memo[trophy];
}

int draw(int total, int levelPlayer, int levelEnemy, int memo[]){
	
	for (int i = 2;i <= total;i++){
		memo[i] = 2147483647;
		if (i % 2 == 0){
			int curr = memo[i/2] + 1;
			if (curr < memo[i]){
				memo[i] = curr;
			}
		}
		if (i - levelPlayer >= 1){
			int curr = memo[i - levelPlayer] + 1;
			if (curr < memo[i]){
				memo[i] = curr;
			}
		}
		if (i - levelEnemy >= 1){
			int curr = memo[i - levelEnemy] + 1;
			if (curr < memo[i]){
				memo[i] = curr;
			}
		}
		int x = memo[i - 1];
		if(x + 1 < memo[i]){
			memo[i] = x + 1;
		}
		
	}
	return memo[total];
}

int minimumOperation(int trophyPlayer, int trophyEnemy, 
int levelPlayer, int levelEnemy){
	
	if(trophyPlayer != trophyEnemy){
		int memo[abs(trophyPlayer - trophyEnemy) + 1];
		memo[1] = 1;
		return draw(abs(trophyPlayer - trophyEnemy),levelPlayer,levelEnemy, memo);
	}else{
		int memo[trophyPlayer + trophyEnemy + 1];
		memo[1] = 1;
		return draw(trophyPlayer + trophyEnemy, levelPlayer, levelEnemy, memo);
	}
}

#endif
