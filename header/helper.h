#ifndef HELPER
#define HELPER
#include <stdlib.h>

struct Position{
	int x, y;
};

int randomize(int id, int len){
	int random;
	do{
		random = rand()%len + 1;
	}while(random == id);
	return random;
}

#endif
