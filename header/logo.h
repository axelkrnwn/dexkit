#include <stdio.h>
#include <windows.h>

void logo(int startX, int startY){
	FILE *fp = fopen("./asset/logo.txt","r");
	char line[100];
	int i = 0;
	while(!feof(fp)){
		gotoXY(startX, startY + i);
		fgets(line,sizeof(line),fp);
		printf("%s",line);
		i++;
	}
	fclose(fp);
}

