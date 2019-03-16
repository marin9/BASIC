#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FS_MAXSIZE	8192
#define STACKSIZE	32

char *p;
int var[26];
int sp, err, size;
char *stack[STACKSIZE];
char buff[FS_MAXSIZE+1];


int parseExpression();
void parseStatment();
void skipSpaces();


int isDigit(char c){
	if(c>='0' && c<='9') return 1;
	else return 0;
}

int isAlpha(char c){
	if(c>='A' && c<='Z') return 1;
	if(c>='a' && c<='z') return 1;
	return 0;
}

void error(char *msg){
	int ln=0;
	char *t=buff;
	if(!err){
		err=1;
		while(t!=p && *t){
			if(*t=='\n') ++ln;
			++t;
		}
		printf("\nERROR: %s: line %d.\n", msg, ln+1);
	}
}

void skipLabel(){
	char *t=p;
	skipSpaces();
	while(isAlpha(*p) || isDigit(*p)) ++p;
	if(*p==':'){
		++p;
	}else{
		p=t;
	}
}

void skipSpaces(){
	while(*p==' ' || *p=='\t') ++p;
}

int parseNumber(){
	int i=0;
	char b[16];
	while(*p>='0' && *p<='9'){
		b[i++]=*p++;
	}
	b[i]=0;
	return atoi(b);
}

int parseFactor(){
	int a;
	if(*p>='0' && *p<='9'){
		a=parseNumber();
	}else if(*p>='A' && *p<='Z'){
		a=var[(int)(*p-'A')];
		++p;
	}else if(*p=='('){
		++p;
		a=parseExpression();
		if(*p!=')') error("Missing bracket");
		else ++p;
	}else{
		error("Syntax error");
	}
	return a;
}

int parseTerm(){
	int a, b;
	a=parseFactor();
	while(1){
		if(*p=='*'){
			p++;
			b=parseFactor();
			a*=b;
		}else if(*p=='/'){
			p++;
			b=parseFactor();
			if(b!=0) a/=b;
			else error("Divide by zero");
		}else if(*p=='&'){
			p++;
			b=parseFactor();
			a=a&&b;
		}else if(*p=='|'){
			p++;
			b=parseFactor();
			a=a||b;
		}else if(*p=='%'){
			p++;
			b=parseFactor();
			a=(!a && b) || (a && !b);
		}else if(*p=='='){
			p++;
			b=parseFactor();
			a=(a==b);
		}else if(*p=='>'){
			p++;
			b=parseFactor();
			a=(a>b);
		}else if(*p=='<'){
			p++;
			b=parseFactor();
			a=(a<b);
		}else if(*p=='!'){
			p++;
			b=parseFactor();
			a=(a!=b);
		}else{
			return a;
		}
	}
}

int parseExpression(){
	int a, b;
	if(*p=='-' || *p=='+') a=0;
	else a=parseTerm();

	while(1){
    	if(*p=='-'){
      		p++;
			b=parseTerm();
			a-=b;
		}else if(*p=='+'){
			p++;
			b=parseTerm();
			a+=b;
		}else{
			return a;
		}
	}
}

void parseCR(){
	skipSpaces();
	if(*p==0) error("Try to execute out of memory");
	else if(*p=='\n') ++p;
	else error("Syntax error");
}

void parseString(){
	if(*p=='"'){
		++p;
		while(*p!='"' && *p!='\n' && *p!='\0'){
			printf("%c", *p++);
		}
		if(*p!='"') error("Syntax error");
		else ++p;
	}
}

void parsePrint(){
	while(1){
		skipSpaces();
		if(*p=='"') parseString();
		else printf("%d", parseExpression());
		skipSpaces();
		if(*p==',') ++p;
		else break;
	}
	if(*p==';') ++p;
	else printf("\n");
}

void parseEnd(){
	printf("\n");
	if(*p!='\n' && *p!=0) error("Syntax error");
	else err=1;
}

void parseInput(){
	int n;
	if(*p>='A' && *p<='Z'){
		scanf("%d", &n);
		var[(int)(*p-'A')]=n;
		++p;
	}else{
		error("Syntax error");
	}
}

void parseLet(){
	int v;
	if(*p>='A' && *p<='Z'){
		v=(int)(*p-'A');
		++p;
		if(*p!='=') error("Syntax error");
		++p;
		var[v]=parseExpression();
	}else{
		error("Syntax error");
	}	
}

void parseIf(){
	if(!parseExpression()){
		while(*p!='\n' && *p!='\0') ++p;
		return;
	}
	skipSpaces();		
	if(strncmp(p, "THEN ", 5)!=0){
		error("Syntax error");
		return;
	}
	p+=5;
	skipSpaces();
	parseStatment();
}

void parseGoto(){
	int ln, i=0;
	char *t=p;
	char label[32];
	while(isAlpha(*p) || isDigit(*p)){
		label[i++]=*p++;
	}
	label[i++]=':';
	label[i]=0;
	skipSpaces();
	if(*p!=0 && *p!='\n'){
		error("Syntax error");
		return;
	}

	p=buff;
	ln=0;
	while(1){
		skipSpaces();
		if(strncmp(p, label, i)==0){
			skipLabel();
			skipSpaces();
			parseStatment();
			return;
		}
		while(*p!='\n' && *p!='\0') ++p;
		if(*p==0){
			p=t;
			error("Label not found");
			break;
		}else{
			++ln;
			++p;
		}
	}
}

void parseGosub(){
	char *t=p;
	while(*t!='\n' && *t!=0) ++t;
	stack[sp++]=++t;
	if(sp>=STACKSIZE){
		error("Stack overflow");
		return;
	}
	parseGoto();
}

void parseRet(){
	if(*p!='\n' && *p!=0){
		error("Syntax error");
		return;
	}
	--sp;
	if(sp<0){
		error("RET without GOSUB");
		return;
	}
	p=stack[sp];
	skipLabel();
	parseStatment();
}

void parseStatment(){
	skipSpaces();
	if(strncmp(p, "REM ", 4)==0){
		p+=4;
		while(*p!='\n' && *p!='\0') ++p;
	}else if(strncmp(p, "PRINT ", 6)==0){
		p+=6;
		skipSpaces();
		parsePrint();
	}else if(strncmp(p, "INPUT ", 6)==0){
		p+=6;
		skipSpaces();
		parseInput();
	}else if(strncmp(p, "GOTO ", 5)==0){
		p+=5;
		skipSpaces();
		parseGoto();
	}else if(strncmp(p, "GOSUB ", 6)==0){
		p+=6;
		skipSpaces();
		parseGosub();
	}else if(strncmp(p, "IF ", 3)==0){
		p+=3;
		skipSpaces();
		parseIf();
	}else if(strncmp(p, "LET ", 4)==0){
		p+=4;
		skipSpaces();
		parseLet();
	}else if(strncmp(p, "RET", 3)==0){
		p+=3;
		skipSpaces();
		parseRet();
	}else if(strncmp(p, "END", 3)==0){
		p+=3;
		skipSpaces();
		parseEnd();
	}
}


int main(int argc, char **argv){
	FILE *f;
	if(argc!=2){
		printf("WARNING: Illegal arguments. Usage: tb [file]\n");
		return 1;
	}
	if(!(f=fopen(argv[1], "rb"))){
		printf("WARNING: File not exist.\n");
		return 2;
	}

	size=fread(buff, 1, 8192, f);
	fclose(f);
	buff[size]=0;
	p=buff;
	err=0;
	sp=0;

	while(!err){
		skipLabel();
		parseStatment();
		parseCR();
	}
	return 0;
}

/*	GRAMMAR
line := (label:)? statment? CR
statment := PRINT (str|expr) (, (str|expr))* ;?
			IF expr THEN statment
			GOTO label
			INPUT var
			LET var=expr
			GOSUB label
			REM ...
			RET
			END

expr := (+|-|e) term ((+|-) term)*
term := factor ((*|/) factor)*
factor := var|num|(expr)
str := "..."
label := ...
*/

