#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include "./consoleHelper.h"
#include "./modes.h"
#include "./window.h"

void setup(){
	srand(time(0));
	createWindow();
	modes::mouseInput();
}
