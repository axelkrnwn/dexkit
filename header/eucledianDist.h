#ifndef EUCLEDIANDIST
#define EUCLEDIANDIST
#include <math.h>

int eucledianDist(Position src, Position dest){
	return sqrt(pow(dest.x - src.x,2) + pow(dest.y - src.y,2));
}

#endif
