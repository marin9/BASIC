#pragma once

#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define WHITE   "\x1b[37m"

#define LINE_SIZE	64
#define MAX_LINES	65536
#define PROG_SIZE	(LINE_SIZE*MAX_LINES)

#define STACK_SIZE  128


void basic(char *prog);
int parseStatment(char *t);
int parsePrint(char *t);
int parseIf(char *t);
int parseGoto(char *t);
int parseInput(char *t);
int parseLet(char *t);
int parseGosub(char *t);
int parseReturn(char *t);
int parseEnd(char *t);

int parseString(char **p);
int parseBr(char **p, int *res);
int parseVar(char **p, int *res);
int parseTerm(char **p, int *res);
int parseNumber(char **p, int *res);
int parseExpression(char **p, int *res);
