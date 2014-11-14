#include <errno.h>
#include "typdek.h"
#include "bnfparser.h"

void getsym(void)
{ /* liest das nächste Symbol aus der .bnf-Datei ein */
	do {
		sym=bnfscanner();
		} 
	while (sym==' ' || sym == '\n');
	
}

void getsymg(void)
{ /* liest das nächste Symbol aus der .bnf-Datei ein */
	do {
		sym=scanner();
		} 
	while (sym==' ' || sym == '\n');
	
}

char bnfscanner(void)
{
	int i;
	fpos_t position;
	char literal[30];
	char c;
	char retsym;
	static char sym;
	static char symv=' ';
	static char symvv=' ';
	int litflag=0;
		
	fgetpos(file,&position);
	if (errno > 0)
	{
		symvv=symv;
		symv=sym;
	}
	sym=fgetc(file);
	if ((symvv!='"') && (symv=='"') && (sym!=' '))
	{
		litflag=1;
		i=-1;
		c=sym;
		do 
		{
			i++;
			literal[i]=c;
			c=fgetc(file);
			
		} while (((c!='"') || i<0) && i<=29 );
		literal[i+1]=0;
		if (c!='"')
			return(literal[0]);
		fseek(file,(-1),SEEK_CUR);
		retsym=findlit(literal);
	}
	if (litflag)
	{
		printf("%s",literal);
		switch (strlen(literal))
		{
		case 1:
			break;
		case 2:
			symvv=symv;
			symv=literal[0];
			sym=literal[1];
			break;
		default:
			sym=literal[strlen(literal)-1];
			symv=literal[strlen(literal)-2];
			symvv=literal[strlen(literal)-3];
			break;
		}
	}
	else
	{
		retsym=sym;
		printf("%c",sym);
	}
	return(retsym);
}

char scanner(void)
{
	int i,j;
	fpos_t position;
	char literal[30];
	char sym, symv;
	int litflag=0;
	j=0;
	// if (file->_ptr)
	// produziert einen Compile-Fehler
	// Hauptach es kompiliert ...
	if(1)
	{
		fseek(file,(-1),SEEK_CUR);
		symv=fgetc(file);
	}
	fgetpos(file, &position);
	sym=fgetc(file);
	if (sym==EOF)
		return(sym);
	if (sym!=' ' && sym!='\n')
	{
		litflag=1;
		if(fseek(file,(-1),SEEK_CUR))
			printf("Fehler0!");
		for (i=0;(i<=29) && (literal[i]!=EOF);i++)
		{
			literal[i]=fgetc(file);

		} 
		for (;!(findlitg(literal)) && (i>=0);i--) 
		{
			literal[i+1]=0;
		} 
		//position+= strlen(literal);
		//geht nicht, also:
		fseek(file, strlen(literal), SEEK_CUR);
		fsetpos(file, &position);
			
		if (findlitg(literal))
			sym=findlit(literal);
		else 
		{
			printf("%c", literal[0]);
			return(64); /* Symbol ist (hoffentlich) nicht benutzt */
		}
	}
	if (litflag)
		printf("%s",literal);
	else 
		printf("%c",sym);
	return(sym);
}

char findlit(char* literal)
{
	struct litlist* h1;
	char temp;
	if (!lsentinel)
	{
		lsentinel = (struct litlist*)ma(sizeof(struct litlist));
		lsentinel->next=0; lsentinel->sym=65;
		llist=lsentinel;
	}
	h1=llist;         
	strcpy(lsentinel->lit,literal);  
	temp=lsentinel->sym;
	while (strcmp(h1->lit,literal)) 
	{
		temp=h1->sym;
		h1=h1->next;  
	
	}
	if (h1==lsentinel)                 
		{
		lsentinel = (struct litlist*)ma(sizeof(struct litlist)); 
		h1->next = lsentinel;  
		h1->sym=temp+1;      
		}
	return (h1->sym); 	
	
}

int findlitg(char* literal)
{
	struct litlist* h1;
	char temp;
	h1=llist;
	strcpy(lsentinel->lit,literal);  
	while (strcmp(h1->lit,literal)) 
	{
		temp=h1->sym;
		h1=h1->next;  
	
	}
	if (h1==lsentinel)                 
		return(0);
	else
		return (1); 	
	
}

void *ma(size_t g)
{
	void *p;
	static struct freelist* fhead;
	static struct freelist* ftail;
	struct freelist* temp; 
	/* Letzter Aufruf und free(all Space) ? */
	if (!g)
	{	
		temp=fhead->next; 
		while (temp)
		{
			temp=fhead->next;
			free(fhead->pointer);
			free(fhead);
			fhead=temp;
		}
		return NULL;
	}
	p = malloc(g);  
	/* Erster Aufruf ? Wenn ja, dann Kopf der Liste erstellen */
	if (!fhead)
	{
		fhead=(struct freelist*)malloc(sizeof(struct freelist));
		fhead->next=0;
		fhead->pointer=p;
		ftail=fhead;
	}
	/* Wenn nein: Lister erweitern ! */
	else
	{
		temp=(struct freelist*)malloc(sizeof(struct freelist));
		temp->next=0;
		temp->pointer=p;
		ftail->next=temp;
		ftail=temp;
	}
		
	return p;
}


