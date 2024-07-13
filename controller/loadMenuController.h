#include "../view/loadMenu.h"
#include "../header/consoleHelper.h"
#include "../controller/gameController.h"

#ifndef LOADMENUCONTROLLER
#define LOADMENUCONTROLLER

#include <conio.h>
int page;
int curr;

namespace LoadMenuController{
	bool exitedLoad;
	int size = 0;
	int selected = -1;
	
	
	void createPage(int min, int max){
		clear(50, 30, 10, 10);
		Player::pagination(Player::root, min, max);
	}
	
	void eventListener(COORD coord){
		if (coord.X >= LoadMenuView::back->pos.x && coord.Y >= LoadMenuView::back->pos.y &&
		coord.X <= LoadMenuView::back->pos.x + LoadMenuView::back->size.x
		 && coord.Y <= LoadMenuView::back->pos.y + LoadMenuView::back->size.y){
		 	exitedLoad = true;
		}
		if (coord.X >= LoadMenuView::prev->pos.x && coord.Y >= LoadMenuView::prev->pos.y &&
		coord.X <= LoadMenuView::prev->pos.x + LoadMenuView::prev->size.x
		 && coord.Y <= LoadMenuView::prev->pos.y + LoadMenuView::prev->size.y){
			if (page > 1){
				page--;
				createPage((page-1)*10 + 1,(page)*10);
			}
		}
		if (coord.X >= LoadMenuView::next->pos.x && coord.Y >= LoadMenuView::next->pos.y &&
		coord.X <= LoadMenuView::next->pos.x + LoadMenuView::next->size.x
		 && coord.Y <= LoadMenuView::next->pos.y + LoadMenuView::next->size.y){
			if (page < size/10 + 1){
				page++;
				createPage((page-1)*10 + 1,(page)*10);
			}
		}
		if (coord.X >= 30 && coord.X <= 60 && coord.Y >= 10 && coord.Y <= 20){
			if (coord.Y - 9 == selected){
				
				SetConsoleTextAttribute(hStdOut,3);
				LoadMenuView::printConfirmation();
				SetConsoleTextAttribute(hStdOut,7);
				
				char n;
				modes::keyInput();
				do{
					n = getch();
				}while(n != 'y' && n != 'n' && n != 'Y' && n != 'N');
				
				if (n == 'y' || n == 'Y'){
					modes::mouseInput();
					clear(97, 1, 1, consoleSize.Y - 2);
					GameController::load(selected + 10*(page - 1));
					exitedLoad = true;
				}else{
					LoadMenuView::cancelConfirmation();
					modes::mouseInput();
					LoadMenuView::createPoint(selected + 9, ' ');
					selected = -1;
				}
			}else if (coord.Y - 9 + (page-1) <= size && coord.Y - 9 > 0){
				if (selected > 0){
					LoadMenuView::createPoint(selected + 9, ' ');
				}
				selected = coord.Y - 9;
				LoadMenuView::createPoint(coord.Y, '<');
			}
		}
	}	

	void start(){
		page = 1;
		curr = 0;
		size = Player::getSize();
		selected = -1;
		
		LoadMenuView::loadMenu();
		if (size > 0){
			createPage((page-1)*10 + 1,(page)*10);
		}else{
			
		}
		while(!exitedLoad){
			COORD coord = leftClickListener();
			eventListener(coord);			
		}
	}
}

#endif
