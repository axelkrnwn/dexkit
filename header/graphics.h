#include <stdlib.h>
#include <string.h>
#include "./helper.h"
#include "./consoleHelper.h"

#ifndef graphics
#define graphics

namespace component{
	struct Button{
		char text[100];
		Position pos;
		Position size;
		int color;
			
		void print(){
			gotoXY(this->pos.x,this->pos.y);
			printf("%c%c%c",201,205,205);
			int len = strlen(this->text);
			for (int i = 0;i < len;i++){
				printf("%c",205);
			}
			printf("%c%c%c",205,205,187);
			gotoXY(this->pos.x,this->pos.y + 1);
			printf("%c  %s  %c\n",186, this->text,186);
			gotoXY(this->pos.x,this->pos.y + 2);
			printf("%c%c%c",200,205,205);
			for (int i = 0;i < len;i++){
				printf("%c", 205);
			}
			printf("%c%c%c",205,205,188);
		}	
		
		bool isValid(COORD coord){
			if (coord.X >= this->pos.x && 
				coord.Y >= this->pos.y &&
				coord.X <= this->pos.x + this->size.x && 
				coord.Y <= this->pos.y + this->size.y){
				return true;
			}
			return false;
		}
			
	};
	
	Button *createButton(const char text[], int x, int y, int width, int height
	, int color){
		Button *b = (Button*)malloc(sizeof(Button));
		strcpy(b->text, text);
		b->pos.x = x;
		b->pos.y = y;
		b->color = color;
		b->size.x = width;
		b->size.y = height;
		return b;
	}
	
	struct Label{
		char text[100];
		Position pos;
		
		void print(){
			gotoXY(this->pos.x,this->pos.y);
			printf("%s", this->text);
		}
	};
	
	Label *createLabel(char text[], int x, int y){
		Label *l = (Label*)malloc(sizeof(Label));
		strcpy(l->text, text);
		l->pos.x = x;
		l->pos.y = y;
		return l;
	}
	
	struct Card{
		char symbol;
		Position pos;
		Position size;
		int tableIdx;
		
		bool isValid(COORD coord){
			if (coord.X >= this->pos.x && 
				coord.Y >= this->pos.y &&
				coord.X <= this->pos.x + this->size.x && 
				coord.Y <= this->pos.y + this->size.y){
				return true;
			}
			return false;
		}
		
		void print(){		
			for (int i = 0;i < size.y;i++){
				for (int j = 0;j < size.x;j++){
					gotoXY(this->pos.x + j,this->pos.y + i);
					if (i == 0 && j == 0){
						printf("%c",201);
					}else if (i == 0 && j == size.x-1){
						printf("%c",187);
					}else if (i == size.y-1 && j == 0){
						printf("%c",200);
					}else if (i == size.y-1 && j == size.x-1){
						printf("%c",188);
					}else if (i == 0 || i == size.y - 1){
						printf("%c",205);
					}else if (j == 0 || j == size.x - 1){
						printf("%c",186);
					}else if (j == size.x/2 && i == size.y/2){
						printf("%c",this->symbol);
					}
				}
			}	
			
		}
	};
	
	Card *createCard(char text, int x, int y, int w, int h, int tableIdx){
		Card *newC = (Card*)malloc(sizeof(Card));
		newC->symbol = text;
		newC->pos.x = x;
		newC->pos.y = y;
		newC->size.x = w;
		newC->size.y = h;
		newC->tableIdx = tableIdx;
		
		return newC;
	}
	
	
}

#endif

