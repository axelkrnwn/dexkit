#ifndef WINMENU
#define WINMENU

namespace Win{
	
	Button *backBtn;
	int star;
	
	void showVictory(){
		FILE *fq = fopen("./asset/victory.txt","r");
			int i = 0;
			while(!feof(fq)){	
				char str[100];
				fscanf(fq, "%[^\n]\n", str);
				gotoXY(10, 4 + i);
				printf("%s", str);
				i++;
			}
		fclose(fq);
	}
	
	void showDefeat(){
		FILE *fq = fopen("./asset/defeat.txt","r");
			int i = 0;
			while(!feof(fq)){	
				char str[100];
				fscanf(fq, "%[^\n]\n", str);
				gotoXY(17, 4 + i);
				printf("%s", str);
				i++;
			}
		fclose(fq);
	}
	
	
	void showStar(){
		for (int s = 0;s < 3;s++){
			if (s < star){
				SetConsoleTextAttribute(hStdOut, 6);
			}else{
				SetConsoleTextAttribute(hStdOut, 8);			
			}
			
			FILE *fp = fopen("./asset/star.txt","r");
			int i = 0;
			while(!feof(fp)){
				char str[100];
				fgets(str,20,fp);
				gotoXY(15 + s*25, 14 + i);
				printf("%s", str);
				i++;
			}
			fclose(fp);
		}
		SetConsoleTextAttribute(hStdOut, 7);
	}
	
	void initializeComponent(){
		backBtn = createButton("back",44,31,8,3,7);
	}
	
	void printComponent(){
		backBtn->print();
	}
	
	void menu(int total){
		star = total;
		initializeComponent();
		frame();
		printComponent();
		showStar();	
		if(star > 0){
			showVictory();
		}else{
			showDefeat();
		}
	}
	
}

#endif
