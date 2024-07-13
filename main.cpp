#include "./header/frame.h"
#include "./header/init.h"
#include "./controller/mainMenuController.h"

int main() {
	setup();
	MainMenu::start();
	return 0;
}

