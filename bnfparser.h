#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef _BNFPARSER_H
#define _BNFPARSER_H


/* Globale Variablen */
extern char sym;
extern FILE *file;

/* Variablendeklarationen */
static struct litlist* llist=0;
static struct litlist* lsentinel=0;



/* Prototypen */


void *ma(size_t);
void getsym(void);
void getsymg(void);
char bnfscanner(void);
char scanner(void);
char findlit(char* literal);
int findlitg(char* literal);

#endif

