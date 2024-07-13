#include "../view/attackMenu.h"
#include "../view/gameLobbyMenu.h"
#include "../model/player.h"
#include "./pvpController.h"
#include "./pveController.h"

#ifndef ATTACKCONTROLLER
#define ATTACKCONTROLLER

namespace AttackController{
	
	bool exitAttack;
	Player::Node *p;
	
	void pve(){
		char str[100];
		Attack::clearErrorMsg();
		if (Troop::campHead == NULL){
			sprintf(str, "You must train troops");
			Attack::printErrorMessage(str);
			return;
		}		
		SetConsoleTextAttribute(hStdOut, 7);
		clear(98, 1, 1, consoleSize.Y - 2);
		Pve::start(p);
		SetConsoleTextAttribute(hStdOut, 7);
		exitAttack = true;
	}
	
	void pvp(){
		char str[100];
		Attack::clearErrorMsg();
		if (Troop::campHead == NULL){
			sprintf(str, "You must train troops");
			Attack::printErrorMessage(str);
			return;
		}else if (p->level*50 > p->gold){
			sprintf(str, "Your gold must be at least %d", p->level*50);
			Attack::printErrorMessage(str);
			return;
		}
		
		SetConsoleTextAttribute(hStdOut, 7);
		clear(98, 1, 1, consoleSize.Y - 2);
		Pvp::start(p);
		SetConsoleTextAttribute(hStdOut, 7);
		exitAttack = true;
	}

	void initTroopLabel(){
		Troop::Node *curr = Troop::head;
		for (int i = 0;i < 3;i++){
			Attack::types[i] = createLabel(curr->category,2,2 + 3*i);
			Troop::InnerNode *cur = curr->head;
			while(cur){
				Attack::troops[i] = createLabel(cur->name,2,3*(i+1));
				cur = cur->next;
			}
			curr = curr->next;
		}
		
	}
	
	void showCampCapacity(){
		gotoXY(70,7);
		printf("Available Train Slot");
		gotoXY(70,8);
		printf("%.3d", p->troopCapacity - Troop::getCapacity() - Troop::getTrainCapacity());
		gotoXY(70,10);
		printf("Your Troops Total");
		gotoXY(70,11);
		printf("%.3d/%.3d",Troop::getCapacity(), p->troopCapacity);
	}
	
	void train(char category[], char name[]){
		Troop::Node *node = Troop::getNode(category);
		Troop::InnerNode *innerNode = Troop::getTroop(name, node);	
		char str[100];
		sprintf(str, "You need %d more level(s) to unlock", innerNode->levelRequired - p->level);
		Attack::clearErrorMsg();
		if (p->level < innerNode->levelRequired){
			Attack::printErrorMessage(str);
			return;
		}
		sprintf(str, "The camp is full");
		Attack::clearErrorMsg();
		
		if (p->troopCapacity < Troop::getCapacity() + innerNode->space || 
		Troop::getTrainCapacity() + innerNode->space >
		 p->troopCapacity - Troop::getCapacity()){
			Attack::printErrorMessage(str);
			return;
		}
		Troop::enqueue(Troop::createTrain(category,name,innerNode->time, innerNode->space));
		Troop::viewTrain();
		showCampCapacity();	
	}
	
	
	void onDoneTrain(){
		if (Troop::queueHead){
			if (Troop::queueHead->time <= 0){
				
			}
			Troop::viewTrain();
		}
		
	}
	
	void trainProcess(){
		if (Troop::queueHead){
			Troop::queueHead->train();
			if (Troop::queueHead->time <= 0){
				Troop::doneTrain();
				if (!exitAttack){
					SetConsoleTextAttribute(hStdOut, 7);
					clear(22,21,2,17);
					clear(22,44,2,17);
					Troop::viewCamp(44,2, -1);
					clear(25, 70, 7, 6);
					showCampCapacity();
				}
			}
			if (!exitAttack){
				Troop::viewTrain();
			}
		}
	}
	
	void eventListener(COORD coord){
		if (Attack::backBtn->isValid(coord)){
		 	exitAttack = true;
		}
		if (Attack::pvpBtn->isValid(coord)){
			pvp();
		}
		if (Attack::pveBtn->isValid(coord)){
			pve();
		}
		if (coord.X >= 2 && coord.X < 22){
			int categoryIdx = -1, nameIdx = -1;
			for (int i = 0;i < 3;i++){
				if (coord.Y > Attack::types[i]->pos.y){
					categoryIdx = i;
				}
			}
			for (int i = 0;i < 3;i++){
				if (coord.Y == Attack::troops[i]->pos.y){
					nameIdx = i;
				}
			}
			if (categoryIdx == -1 || nameIdx == -1){
				return;
			}
			train(Attack::types[categoryIdx]->text,
			Attack::troops[nameIdx]->text);
			showCampCapacity();
		}
	}
	
	
	void start(Player::Node *player){
		SetConsoleTextAttribute(hStdOut, 7);
		exitAttack = false;
		modes::mouseInput();
		p = player;
		clear(98, 1, 1, consoleSize.Y - 2);
		initTroopLabel();
		Attack::attackMenu();
		Troop::viewTroop(p->level);
		GameLobbyView::showPlayer(player);
		Troop::viewCamp(44,2,-1);
		showCampCapacity();
		
		while(!exitAttack){
			COORD coord = leftClickListener();
			if (Buildings::collectGold(player)){
				GameLobbyView::showPlayer(player);
			}	
			eventListener(coord);
			Sleep(1000);		
			trainProcess();
		}
		
	}
	
}

#endif
