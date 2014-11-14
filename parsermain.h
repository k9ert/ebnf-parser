#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#ifndef _BNFMAIN_H
#define _BNFMAIN_H

#define EBNFDEFAULT "test.bnf"
#define PARSEDEFAULT "test.txt"

/* Variablendeklarationen */



char sym;
FILE *file;
struct header* list;     /* Pointer zum ersten Element der "Merkliste" */
struct header* sentinel; /* Pointer zum letzten Element der "Merkliste" */
struct header* h;
struct node* q;
struct node* r;
struct node* s;
int noerr;






/* Prototypen ! Prototypen ! */

void error(char);
int parse(struct header* goal,int* match);
struct header* find(char s);
void term(struct node** p, struct node** q, struct node** r, struct node** s);
void expression(struct node** p, struct node** q, struct node** r, struct node** s);
void factor(struct node** p, struct node** q, struct node** r, struct node** s);
void link(struct node* p, struct node* q);
void getsym(void);
char bnfscanner(void);
void getsymg(void);
char scanner(void);
char findlit(char*);
int findlitg(char*);


#endif

