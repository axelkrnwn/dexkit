#ifndef COINCHANGE
#define COINCHANGE

typedef unsigned long long int ULL;


int getNumbers(ULL comb){
	int total = 0;
	while(comb > 0){
		total += comb%10;
		comb /= 10;
	}
	return total%10;
}

ULL coinChange(int trophy, int level){
	ULL memo[trophy+1] = {0};
	memo[0] = 1;
	
	int coins[level] = {0};
	for (int i = 1;i <= level;i++){
		coins[i-1] = i;
	}
	for (int i = 0;i < level;i++){
		for (int j = 0;j <= trophy;j++){
			if (coins[i] <= j){
				memo[j] += memo[j - coins[i]];
			}
		}
	}
	return getNumbers(memo[trophy]);
}




#endif
