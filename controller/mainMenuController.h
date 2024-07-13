#include "../view/mainMenu.h"
#include "../header/events.h"
#include "../view/exitMenu.h"
#include "../view/howToPlayMenu.h"
#include "../header/window.h"
#include "../model/player.h"
#include "../model/troop.h"
#include "./loadMenuController.h"
#include "../header/buildings.h"
#include "../controller/shopController.h"
#include "./gameController.h"
#include <string.h>

#ifndef MAINMENUCONTROLLER
#define MAINMENUCONTROLLER

bool exited = false;

bool isAlpha(char a){
	return (a >= 'A' && a <= 'Z') || (a >= 'a' && a <= 'z');
}


bool isSpace(char a){
	return a == ' ';
}

bool isNum(char a){
	return a >= '0' && a <= '9';
}
bool unique(char *name){
	FILE *fp = fopen("./database/players.csv","r");
	if (!fp){
		return 1;
	}
	char nameCompare[30];
	int a, b, c;
	while(!feof(fp)){
		fscanf(fp,"%[^,],%d,%d,%d\n",nameCompare, &a, &b, &c);
		if (strcmp(nameCompare, name) == 0) return 0;
	}
	fclose(fp);
	return 1;
}

int validateName(char *name){
	int len = strlen(name);
	if (len < 5 || len > 20){
		return 1;
	}
	bool spaces = true;
	for (int i = 0;i < len;i++){
		if (!isAlpha(name[i]) && !isNum(name[i]) && !isSpace(name[i])){
			return 2;
		}
		if (!isSpace(name[i])){
			spaces = false;
		}
	}
	if (spaces){
		return 3;
	}
		
	if (!unique(name)){
		return 4;
	}
	return 0;
}

void exitMenu(){
	SetConsoleTextAttribute(hStdOut, 7);
	clear(97, 1, 1, consoleSize.Y - 2);
	modes::keyInput();
	exit(&exited);
}

char errors[][60] = {
	"Your name must be 5-20 characters",
	"Your name must not contains symbols",
	"Your name must not be empty",
	"Your name must be unique",
};

void playMenu(){
	modes::keyInput();
	clear(97, 1, 1, consoleSize.Y - 2);
	gotoXY(30,17);
	char name[100];
	char str[] ="Input your vilage\'s name : "; 
	printf("%s", str);
	do{
		scanf("%[^\n]",name); getchar();
		int valid = validateName(name);
		gotoXY(30, 19);
		for (int i = 0;errors[1][i] != '\0';i++){
			printf(" ");
		}
		
		if (valid != 0){
			gotoXY(30, 19);
			SetConsoleTextAttribute(hStdOut, 4);
			printf("%s", errors[valid-1]);
			int len = strlen(name);
			for (int i = 0; i < len;i++){
				gotoXY(30 + strlen(str) + i, 17);
				printf(" ");
			}
			gotoXY(30 + strlen(str), 17);
			SetConsoleTextAttribute(hStdOut, 7);
		}
	}while(validateName(name));
	Player::insertToFile(name,30,1);
	GameController::player = Player::createPlayer(name, 30,0,1);
	Player::root = Player::insert(Player::root, GameController::player);
	
	clear(98, 1, 1, consoleSize.Y - 2);
	GameController::start(name);
	modes::mouseInput();
	
	SetConsoleTextAttribute(hStdOut, 7);
	clear(98, 1, 1, consoleSize.Y - 2);
	mainMenu();
}

bool howToPlayListen(COORD coord){
	if (HowToPlay::backBtn->isValid(coord)){
		return 1;
	}
	if (HowToPlay::generalBtn->isValid(coord)){
		HowToPlay::showMenu(HowToPlay::generalBtn);
	}
	if (HowToPlay::defenseBtn->isValid(coord)){
		HowToPlay::showMenu(HowToPlay::defenseBtn);
	}
	if (HowToPlay::attackBtn->isValid(coord)){
		HowToPlay::showMenu(HowToPlay::attackBtn);
	}
	if (HowToPlay::troopBtn->isValid(coord)){
		HowToPlay::showMenu(HowToPlay::troopBtn);
	}
	return 0;
}

void howToPlayMenu(){	
	clear(98, 1, 1, consoleSize.Y - 2);
	HowToPlay::menu();
	
	while(true){
		COORD coord = leftClickListener();
		if (howToPlayListen(coord)){
			break;
		}
	}
	SetConsoleTextAttribute(hStdOut, 7);
	clear(98, 1, 1, consoleSize.Y - 2);
	mainMenu();
}

void loadMenu(){
	clear(98, 1, 1, consoleSize.Y - 2);
	LoadMenuController::exitedLoad = false;
	LoadMenuController::start();
	SetConsoleTextAttribute(hStdOut, 7);
	clear(98, 1, 1, consoleSize.Y - 2);
	mainMenu();
}

void eventListener(COORD coord){
	
	if (exitBtn->isValid(coord)){
		exitMenu();
	}
	if (playBtn->isValid(coord)){
		playMenu();
	}
	if (htpBtn->isValid(coord)){
		howToPlayMenu();
	}
	if (loadBtn->isValid(coord)){
		loadMenu();	
	}
}

namespace MainMenu{
	void start(){
		Player::readFromFile();
		Troop::readFromFile();
		ShopController::size = Buildings::readAllDefense();
		mainMenu();
		while(!exited){
			COORD coord = leftClickListener();
			eventListener(coord);			
		}
		
	}
}

#endif
