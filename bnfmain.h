#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef _BNFMAIN_H
#define _BNFMAIN_H

#define EBNFDEFAULT "test.bnf"

struct node {
char type;
union {
      char tsym;
      struct header* nsym;
      } head;
struct node* suc;
struct node* alt;
};

struct header {
char sym;
struct node* entry;
struct header* suc;
};


int parse(struct header* goal,int* match);


#ifndef _VAR
#define _VAR

char sym;
FILE* febnffile;
struct header* list;     /* Pointer zum ersten Element der "Merkliste" */
struct header* sentinel; /* Pointer zum letzten Element der "Merkliste" */
struct header* h;
struct node* q;
struct node* r;
struct node* s;
int noerr;
#endif

#endif

