#include "../model/player.h"
#include "./attackMenuController.h"
#ifndef LEADERBOARDCONTROLLER
#define LEADERBOARDCONTROLLER

namespace LeaderboardController{
	
	int currIdx;
	int size;
	Player::Node *player;
	
	void swap(Player::Node *a, Player::Node *b){
		Player::Node temp = *a;
		*a = *b;
		*b = temp;
	}
	
	void heapify(int index, Player::Node arr[]){
		int resIndex = index, right = index*2, left = index*2+1;
		
		if (left <= size && arr[left].trophy > arr[resIndex].trophy){
			resIndex = left;
		}
		if (right <= size && arr[right].trophy > arr[resIndex].trophy){
			resIndex = right;
		}
		if (resIndex == left/2){
			return;
		}
		swap(&arr[resIndex],&arr[index]);
		heapify(resIndex,arr);
	}
	
	void insertHeap(Player::Node arr[]){
		
		for (int i = 1;i <= size;i++){
			Player::Node p = *Player::search(Player::root,i);
//			if (!p){
//				return;	
//			}
			int idx = currIdx;
			arr[idx] = p;
			while(idx > 1 && arr[idx].trophy > arr[idx/2].trophy){
				swap(&arr[idx], &arr[idx/2]);
				idx /= 2;
			}
			currIdx++;
		}
	}
	
	void heapSort(){
		frame();
		Player::Node arr[size + 1];
		gotoXY(15,15);
		insertHeap(arr);
		int total = size;
		for (int i = size;i >= 1;i--){
			
			int idx = 1;
			
			Player::Node temp = arr[i];
			arr[i] = arr[1];
			arr[1] = temp;
			
			size--;
			heapify(idx,arr);
		}
		
		for (int i = total;i > total-10;i--){
			gotoXY(30,10 +total - i + 1);
			printf("%d. %s - %d", total - i + 1, arr[i].name,arr[i].trophy);
		}
	
	}
	
	void start(){
		SetConsoleTextAttribute(hStdOut, 7);
		size = Player::getSize();
		currIdx = 1;
		heapSort();
		gotoXY(30, 22);
		Button* backBtn = createButton("Back",40,24,8,3,7);
		backBtn->print();
		COORD coord;
		do{
			coord = leftClickListener();
			if (Buildings::collectGold(player)){
			}
			AttackController::trainProcess();
			Sleep(1000);	
		}while(!backBtn->isValid(coord));
	}
}

#endif
