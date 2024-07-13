#ifndef FIBONACCIPRIME
#define FIBONACCIPRIME

int prime(int n){
	bool prime = true;
	
	if (n == 1){
		return false;
	}
	
	for (int i = 2;i <= n/2;i++){
		if (n%i == 0){
			prime = false;
		}
	}
	
	return prime;
		
}

int fibo(int n, int memo[]){
	if (n <= 1){
		return 1;
	}
	if (memo[n-1] != 0){
		return memo[n-1];
	}
	
	return memo[n-1] = fibo(n-1, memo) + fibo(n-2, memo);	
}

int fiboPrime(int lvl){
	
	//2, 3, 5, 13
	//f(3), f(4), f(5), f(...)
	int i = 5;
	int f;
	
	while(lvl > 0){
		int memo[i] = {0};
		f = fibo(i, memo);
		if (prime(f)){
			lvl--;
		}
		i++;
	}
	
	return f;
	
	
}

#endif
