#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "basic.h"

enum{
  RESUME,
  FINISH,
  UNKN_STAT,
  OUT_OF_MEM,
  NO_GOSUB,
  SYN_ERR,
  EXP_VAL,
  ST_OVF
};

static char *err_msg[7]={
  "Program finish.",
  "Unknown statment.",
  "Try execute out of memory.",
  "Return without GOSUB.",
  "Syntax error.",
  "Ilegal expression value.",
  "Stack overflow."
};

static int err;
static int var[26];
static int stack[STACK_SIZE];
static int sp;
static int pc;



void basic(char *prog){
  int i;
  for(i=0;i<26;++i) var[i]=0;
  err=0;
  pc=0;
  sp=0;

  while(!err){
    if(!parseStatment(prog+pc*LINE_SIZE)) err=UNKN_STAT;
    if(pc>=MAX_LINES) err=OUT_OF_MEM;
  }

  if(err>1) printf("\n %sLine: %d: %s\n", RED, pc, err_msg[err-1]);
  else printf("\n %s%s\n", GREEN, err_msg[err-1]);
  printf("%s", WHITE);
}


int parseStatment(char *t){
  while(*t==' ') ++t;
  if(strncmp(t, "PRINT ", 6)==0) parsePrint(t+6);
  else if(strncmp(t, "IF ", 3)==0) parseIf(t+3);
  else if(strncmp(t, "GOTO ", 5)==0) parseGoto(t+5);
  else if(strncmp(t, "INPUT ", 6)==0) parseInput(t+6);
  else if(strncmp(t, "LET ", 4)==0) parseLet(t+4);
  else if(strncmp(t, "GOSUB ", 6)==0) parseGosub(t+6);
  else if(strncmp(t, "RETURN", 6)==0) parseReturn(t+6);
  else if(strncmp(t, "END", 3)==0) parseEnd(t+3);
  else if(strncmp(t, "REM", 3)==0) ++pc;
  else if(strcmp(t, "")==0) ++pc;
  else return 0;
  return 1;
}

int parsePrint(char *t){
  int value;
	if(parseString(&t));
	else if(parseExpression(&t, &value)) printf("%d", value);
  else{
    err=SYN_ERR;
    return 0;
  }

  while(*t==' ') ++t;
	while(*t==','){
    ++t;
    if(parseString(&t));
    else if(parseExpression(&t, &value)) printf("%d", value);
    else{
      err=SYN_ERR;
      return 0;
    }
		while(*t==' ') ++t;
  }
  printf("\n");
  if(*t!=0){
    err=SYN_ERR;
    return 0;
  }else{
    ++pc;
    return 1;
  }
}

int parseIf(char *t){
  int value;

  if(!parseExpression(&t, &value)){
    err=SYN_ERR;
    return 0;
  }

  if(value){
    if(!parseStatment(t)){
      err=SYN_ERR;
      return 0;
    }
  }else{
    ++pc;
  }
  return 1;
}

int parseGoto(char *t){
  int value;
  while(*t==' ') ++t;

  if(!parseExpression(&t, &value)) err=SYN_ERR;
  else{
    if(value<0 || value>=MAX_LINES) err=EXP_VAL;
    else if(*t!=0) err=SYN_ERR;
    else{
      pc=value;
      return 1;
    }
  }
  return 0;
}

int parseInput(char *t){
  int value;
  char buff[LINE_SIZE]={0};
  char *p=buff;
  printf(" ");
  fgets(buff, LINE_SIZE, stdin);

  while(*t==' ') ++t;
  if(*t>='A' && *t<='Z'){
    int v=*t-'A';
    ++t;

    if(parseExpression(&p, &value)){
      var[v]=value;
    }else{
      var[v]=-1;
    }
  }

  while(*t==','){
    ++t;
    while(*t==' ') ++t;
    if(*t>='A' && *t<='Z'){
      int v=*t-'A';
      ++t;
      if(parseExpression(&p, &value)){
        var[v]=value;
      }else{
        var[v]=-1;
      }
    }
  }

  if(*t!=0){
    err=SYN_ERR;
    return 0;
  }else{
    ++pc;
    return 1;
  }
}

int parseLet(char *t){
  int sim, value;
  while(*t==' ') ++t;

  if(*t>='A' && *t<='Z'){
    sim=*t-'A';
    ++t;
  }else{
    err=SYN_ERR;
    return 0;
  }

  if(*t=='=') {
    ++t;
  }else{
    err=SYN_ERR;
    return 0;
  }

  if(!parseExpression(&t, &value)){
    err=SYN_ERR;
    return 0;
  }else{
    var[sim]=value;
  }

  while(*t==' ') ++t;
  if(*t!=0){
    err=SYN_ERR;
    return 0;
  }else{
    ++pc;
    return 1;
  }
}

int parseGosub(char *t){
  int value;
  if(!parseExpression(&t, &value)) err=SYN_ERR;
  else{
    while(*t==' ') ++t;
    if(value<0 || value>=MAX_LINES) err=EXP_VAL;
    else if(sp>=STACK_SIZE) err=ST_OVF;
    else if(*t!=0) err=SYN_ERR;
    else{
      stack[sp++]=pc;
      pc=value;
      return 1;
    }
  }
  return 0;
}

int parseReturn(char *t){
  while(*t==' ') ++t;
  if(*t==0){
    --sp;
    if(sp<0) err=NO_GOSUB;
    else{
      pc=stack[sp];
      return 1;
    }
  }else{
    err=SYN_ERR;
  }
  return 0;
}

int parseEnd(char *t){
  while(*t==' ') ++t;
  if(*t==0){
    err=FINISH;
    return 1;
  }else{
    err=SYN_ERR;
    return 0;
  }
}



int parseString(char **p){
  char *t=*p;

  while(*t==' ') ++t;
	if(*t!='\"') return 0;
	++t;
  printf(" ");
	while(*t!='\"' && *t!='\0') printf("%c", *t++);

	if(*t!='\"') return 0;
	*p=++t;
	return 1;
}

int parseNumber(char **p, int *res){
	int x=0;
	char buff[16]={0};

	while((**p)>='0' && (**p)<='9'){
		buff[x]=**p;
		++(*p);
		++x;
	}

	*res=atoi(buff);
	if(buff[0]==0)	return 0;
	else return 1;
}

int parseVar(char **p, int *res){
	if((**p)>='A' && (**p)<='Z'){
		*res=var[(**p)-'A'];
		++(*p);
		return 1;
	}else{
		return 0;
	}
}

int parseBr(char **p, int *res){
	char *t=*p;

	if(*t=='('){
		++t;
		if(!parseExpression(&t, res)){
      err=SYN_ERR;
      return 0;
    }
	}else{
		return 0;
	}

	if(*t==')'){
		*p=++t;
		return 1;
	}else{
		return 0;
	}
}

int parseTerm(char **p, int *res){
	char *t=*p;
	if(parseNumber(&t, res));
	else if(parseVar(&t, res));
	else if(parseBr(&t, res));
	else return 0;

	while(*t=='*' || *t=='/' ||
        *t=='&' || *t=='|' || *t=='%' ||
        *t=='=' || *t=='>' || *t=='<' || *t=='!'){
		int tmp;
		int op=*t++;

		if(parseNumber(&t, &tmp));
		else if(parseVar(&t, &tmp));
		else if(parseBr(&t, &tmp));
		else return 0;

		if(op=='*') *res=*res * tmp;
		else if(op=='/') *res=*res / tmp;
    else if(op=='=') *res=*res == tmp;
    else if(op=='!') *res=*res != tmp;
    else if(op=='>') *res=*res > tmp;
    else if(op=='<') *res=*res < tmp;
    else if(op=='&') *res=*res && tmp;
    else if(op=='|') *res=*res || tmp;
    else if(op=='%') *res=(!(*res) && tmp) || (*res && !tmp);
		else return 0;
	}
	*p=t;
	return 1;
}

int parseExpression(char **p, int *res){
	char *t=*p;
	int neg=0;

	while(*t==' ') ++t;
	if(*t=='-'){
		neg=1;
		++t;
	}else if(*t=='+'){
		neg=0;
		++t;
	}

	if(!parseTerm(&t, res)) return 0;
	if(neg) *res=-(*res);

	while(*t=='-' || *t=='+'){
		int tmp;
		int op=*t++;

		if(!parseTerm(&t, &tmp)) return 0;

		if(op=='+') *res=*res+tmp;
		else if(op=='-') *res=*res-tmp;
		else return 0;
	}
	*p=t;
	return 1;
}
