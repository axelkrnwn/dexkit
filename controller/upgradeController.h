#include "../model/player.h"
#include "../header/fiboPrime.h"
#include "../view/upgradeMenu.h"
#include "../header/buildings.h"
#include "../model/troop.h"
#include "./attackMenuController.h"

#ifndef UPGRADEMENUCONTROLLER
#define UPGRADEMENUCONTROLLER

namespace UpgradeController{
	
	bool exitedUpgrade;
	int price;
	
	void eventListener(COORD coord, Player::Node *player, int price){
		if (Upgrade::cancelBtn->isValid(coord)){
			exitedUpgrade = true;
		}	
		if (Upgrade::acceptBtn->isValid(coord)){
			if (price > player->gold){
				Upgrade::printErrorMessage();
				return;
			}
			player->gold -= price;
			player->level += 1;
			player->troopCapacity += 3;
			exitedUpgrade = true; 
		}
	}
	
	void start(Player::Node *player){
		SetConsoleTextAttribute(hStdOut, 7);
		exitedUpgrade = false;
		if (player->level >= 6){
			frame();
			gotoXY(35,17);
			printf("You\'re already at maximum level!");
			
			Upgrade::backBtn = createButton("Back",45,20,8,3,7);
			Upgrade::backBtn->print();
			COORD coord;
			do{
			 	coord = leftClickListener();
			 	if (Buildings::collectGold(player)){
				}
				Sleep(1000);
			}while(!Upgrade::backBtn->isValid(coord));
			return;
		}
		
		price = fiboPrime(player->level);
		Upgrade::upgradeMenu(price, player->level, Buildings::allDef[player->level]);
		while(!exitedUpgrade){
			COORD coord = leftClickListener();
			if (Buildings::collectGold(player)){
			}
			AttackController::trainProcess();
			eventListener(coord, player, price);			
			Sleep(1000);
		}
	}
}

#endif
