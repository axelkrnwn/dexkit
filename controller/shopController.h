#include "../view/shopMenu.h"
#include "../header/buildings.h"
#include "../model/player.h"
#include "./attackMenuController.h"

#ifndef SHOPCONTROLLER
#define SHOPCONTROLLER

namespace ShopController{
	
	Player::Node *player;
	bool exitedShop;
	int min;
	int max;
	Buildings::Defense *def;
	
	int size = 0;
	
	void createCards(){
		int count = 1;
		for (int i = 0;i < 10;i++){
			Buildings::Defense *curr = Buildings::allDef[i];
			if (curr){
				while(curr){
					if (count >= min && count <= max){
						Shop::initializeCard(curr->symbol,count - min, i);
					}
					curr = curr->next;
					count++;
				}
			}
		}	
	}
	
	bool validateQuantity(Defense *defense){
		Defense *curr = Buildings::playerDef[defense->lvlRequired - 1];
		int count = 0;
		
		while(curr){
			if (strcmp(curr->name, defense->name) == 0) count++;
			curr = curr->next;
		}
		
		return count >= defense->max_quantity - (6 - player->level);
	}
	
	void eventListener(COORD coord){
		for (int i = 0;i < 4;i++){
			if (Shop::cards[i]->isValid(coord)){
				def = getDefense(Shop::cards[i]->tableIdx, Shop::cards[i]->symbol,
				Buildings::allDef);
				Shop::showDetail(def);
			}
		}
		if (Shop::backBtn->isValid(coord)){
		 	exitedShop = true;
		 	def = NULL;
		}
		if (Shop::prevBtn->isValid(coord)){
		 	if (min > 1){
		 		min--;
		 		max--;
		 		createCards();
				Shop::printCards();
			}
		}
		if (Shop::nextBtn->isValid(coord)){
		 	if (max < size){
		 		min++;
		 		max++;
		 		createCards();
				Shop::printCards();
			}
		}
		if (Shop::buyBtn->isValid(coord)){
			char error[100];
			if (!def){
				sprintf(error, "you have to choose a defense");
				Shop::printErrorMessage(error);
				return;
			}else if (def->price > player->gold){
		 		sprintf(error, "you needs %d golds more to buy this",def->price - player->gold);
				Shop::printErrorMessage(error);
				return;
			}else if(player->level < def->lvlRequired){
				sprintf(error, "you must be at level %d to buy this    ",def->lvlRequired);
				Shop::printErrorMessage(error);	
				return;			
			}
			else if (validateQuantity(def)){
				sprintf(error, "defense\'s quantity already maximum    ",def->lvlRequired);
				Shop::printErrorMessage(error);	
				return;	
			}
			else{
		 		player->gold -= def->price;
		 		exitedShop = true;	
			}
			
		}
	}

	Buildings::Defense *start(){
		SetConsoleTextAttribute(hStdOut, 7);
		min = 1;
		max = 4;
		
		modes::mouseInput();
		exitedShop = false;
		clear(98, 1, 1, consoleSize.Y - 2);
		def = NULL;
		gotoXY(0,0);
		Shop::shopMenu();
		createCards();
		Shop::printCards();
		
		while(!exitedShop){
			COORD coord = leftClickListener();
			eventListener(coord);	
			if (Buildings::collectGold(player)){
			}		
			AttackController::trainProcess();
			Sleep(1000);
		}
		
		return def;
		
	}
}

#endif
