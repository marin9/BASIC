#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "basic.h"
#include "fs.h"


static char file_buffer[PROG_SIZE];
static char input_buffer[LINE_SIZE];


int getLine();
void parseCommand();
void exeList(char *p);
void exeHelp();
void addLine(int num, char *text);
int getKeyword(char *src, char *dst);
int isAlpha(char c);
int isDigit(char c);
int isNumber(char *p);



int main(){
  memset(file_buffer, 0, PROG_SIZE);
  printf(" BASIC ready.\n");
  while(1){
    printf(">");
	if(getLine()) parseCommand();
  }
  return 0;
}

int getLine(){
	int i;
   fgets(input_buffer, LINE_SIZE, stdin);
	for(i=0;i<LINE_SIZE;++i){
		if(input_buffer[i]=='\n'){
			input_buffer[i]=0;
			return i;
		}
	}
	return 0;
}

void parseCommand(){
  char buff[16];
  int x=getKeyword(input_buffer, buff);

  if(strcmp(buff, "CLEAR")==0) memset(file_buffer, 0, PROG_SIZE);
  else if(strcmp(buff, "HELP")==0) exeHelp();
  else if(strcmp(buff, "LIST")==0) exeList(input_buffer+x);
  else if(strcmp(buff, "FILES")==0) fs_listFiles();
  else if(strcmp(buff, "TYPE")==0) fs_typeFile(input_buffer+x);
  else if(strcmp(buff, "LOAD")==0) fs_loadFile(input_buffer+x, file_buffer);
  else if(strcmp(buff, "SAVE")==0) fs_saveFile(input_buffer+x, file_buffer);
  else if(strcmp(buff, "DELETE")==0) fs_removeFile(input_buffer+x);
  else if(strcmp(buff, "RUN")==0) basic(file_buffer);
  else if(strcmp(buff, "EXIT")==0) exit(0);
  else if(isNumber(buff)) addLine(atoi(buff), input_buffer+x);
  else printf("%s Unknown command.\n%s", YELLOW, WHITE);
}

void exeList(char *p){
  int n1=0, n2=MAX_LINES-1;
  char buff[16];

  p=p+getKeyword(p, buff);
  if(buff[0] && isNumber(buff)){
    n1=atoi(buff);
  }
  p=p+getKeyword(p, buff);
  if(buff[0] && isNumber(buff)){
    n2=atoi(buff);
  }

  for(;n1<=n2;++n1){
    if(file_buffer[n1*LINE_SIZE]){
	  printf(" %d\t%s\n", n1, file_buffer+n1*LINE_SIZE);
    }
  }
}

void exeHelp(){
  printf(" Commands:\n");
  printf(" FILES \t\t\tList directory.\n");
  printf(" TYPE file_name\t\tPrint file.\n");
  printf(" LOAD file_name\t\tLoad file.\n");
  printf(" SAVE file_name\t\tSave file.\n");
  printf(" DELETE file_name\tDelete file.\n");
  printf(" RUN \t\t\tExecute loaded program.\n");
  printf(" CLEAR \t\t\tClear current file.\n");
  printf(" LIST {n1} {n2}\t\tPrint current file.\n");
  printf(" INFO \t\t\tPrint system informations.\n");
  printf(" HELP \t\t\tShow help.\n");
  printf(" EXIT \t\t\tExit from basic.\n");
  printf(" Max lines: %d\n", MAX_LINES);
}

void addLine(int num, char *text){
  if(num>=MAX_LINES || num<0){
    printf("%s Ilegal number.\n%s", YELLOW, WHITE);
    return;
  }
  if(*text==0){
    file_buffer[LINE_SIZE*num]=0;
    return;
  }

  int i=0;
  while(*text){
    file_buffer[LINE_SIZE*num+i]=*text;
    ++i;
    ++text;
  }
  file_buffer[LINE_SIZE*num+i]=0;
}

int getKeyword(char *src, char *dst){
  char *tmp=src;
  while(*src==' ') ++src;
  while((isAlpha(*src) || isDigit(*src))){
    *dst++=*src++;
  }
  *dst=0;
  while(*src==' ') ++src;
  return src-tmp;
}

int isAlpha(char c){
	if(c>='A' && c<='Z') return 1;
	if(c>='a' && c<='z') return 1;
	return 0;
}

int isDigit(char c){
	return (c>='0' && c<='9');
}

int isNumber(char *p){
	while(isDigit(*p)) ++p;
	if(*p==0 || *p==' ') return 1;
	else return 0;
}
