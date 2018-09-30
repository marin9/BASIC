#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "basic.h"
#include "fs.h"


void fs_listFiles(){
	DIR *d;
	struct dirent *dir;
	d=opendir(".");
	if(d){
		while((dir=readdir(d))!=NULL){
			if(strstr(dir->d_name, ".bas")!=NULL)
				printf(" %s\n", dir->d_name);
		}
		closedir(d);
	}
}

void fs_typeFile(char *file){
	char tmp[10+LINE_SIZE];
	FILE *f=fopen(file, "r");
	if(f==NULL){
		printf("%s File read fail.\n%s", RED, WHITE);
		return;
	}

	while(fgets(tmp, 10+LINE_SIZE, f)){
		printf(" %s", tmp);
	}
	fclose(f);
}

void fs_loadFile(char *file, char *buffer){
	FILE *f=fopen(file, "r");
	if(f==NULL){
		printf("%s File read fail.\n%s", RED, WHITE);
		return;
	}

	char tmp[10+LINE_SIZE];
	while(fgets(tmp, 10+LINE_SIZE, f)){
		int i=0, x=0;
		int lnum=atoi(tmp);

		while(tmp[i]==' ' || tmp[i]=='\t' || (tmp[i]>='0' && tmp[i]<='9')) ++i;
		while(tmp[i]!='\n'){
			buffer[lnum*LINE_SIZE+x]=tmp[i];
			++x;
			++i;
		}
		buffer[lnum*LINE_SIZE+x]=0;
	}
	fclose(f);
	printf("%s File read ok.\n%s", GREEN, WHITE);
}

void fs_saveFile(char *file, char *buffer){
	FILE *f=fopen(file, "w");
	if(f==NULL){
		printf("%s File write fail.\n%s", RED, WHITE);
		return;
	}
	int i;
	for(i=0;i<MAX_LINES;++i){
		if(buffer[i*LINE_SIZE]){
			char tmp[10+LINE_SIZE];
			sprintf(tmp, "%d\t%s\n", i, buffer+i*LINE_SIZE);
			fwrite(tmp, strlen(tmp), 1, f);
		}
	}
	fclose(f);
	printf("%s File write ok.\n%s", GREEN, WHITE);
}

void fs_removeFile(char *file){
	if(remove(file)){
		printf("%s File remove fail.\n%s", RED, WHITE);
	}else{
		printf("%s File removed.\n%s", GREEN, WHITE);
	}
}
