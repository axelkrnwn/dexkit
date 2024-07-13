#ifndef HOWTOPLAY
#define HOWTOPLAY

namespace HowToPlay{
	
	Button *backBtn, *generalBtn, *defenseBtn, 
	*troopBtn, *attackBtn;
	
	void initializeComponent(){
		backBtn 	= createButton("Back",2,1,8,3,7);
		generalBtn 	= createButton("General",2,5,11,3,7);
		defenseBtn 	= createButton("Defense",2,9,11,3,7);
		troopBtn 	= createButton("Troops",2,13,11,3,7);
		attackBtn 	= createButton("Attack",2,17,11,3,7);
	}
	
	void printComponents(){
		backBtn->print();
		generalBtn->print();
		defenseBtn->print();
		troopBtn->print();
		attackBtn->print();
	}
	
	void showMenu(Button *btn){
		clear(consoleSize.X - 19,18,3,consoleSize.Y-4);
		char path[100];
		sprintf(path, "./asset/%s.txt", btn->text);
		FILE *fp = fopen(path,"r");
		int i = 0;
		while(!feof(fp)){
			gotoXY(18,i+5);
			char line[100];
			fgets(line,sizeof(line),fp);
			printf("%s",line);
			i++;
		}
		fclose(fp);
	}
	
	void menu(){
		initializeComponent();
		printComponents();
		showMenu(generalBtn);
	}
	
}

#endif
