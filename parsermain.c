#include "typdek.h"
#include "parsermain.h"
#include "bnfparser.h"

int main(void)
{
	int match;
	char ebnfdfname[80]=EBNFDEFAULT;
	char ebnffname[80];
	char parsefdname[80]=PARSEDEFAULT;
	char parsefname[80];
	
	printf("General EBNF-Parser-Parser\n");
	printf("programmed in C by Kim Neunert\n");
	printf("Template in Modula2 by Wirth\n");
	sentinel = (struct header*)ma(sizeof(struct header));  /* Erstellung des Listenendes */
	list = sentinel; /* Listenanfang = Listenende */
	
	printf("EBNF-File [%s]: ",ebnfdfname);
	gets(ebnffname);
	printf("\n");
	
	printf("Parse-File [%s]: ",parsefdname);
	gets(parsefname);
	printf("\n");
	
	if (ebnffname[0] != 0)
	{
		strcpy(ebnfdfname,ebnffname);
		if (ebnfdfname[strlen(ebnfdfname)-1] !='f')
			strncat(ebnfdfname,".bnf",4);
	}
	if (parsefname[0] != 0)
	{
		strcpy(parsefdname,parsefname);
		if (parsefdname[strlen(parsefdname)-1] !='t')
			strncat(parsefdname,".txt",4);
	}
	if (!(file=fopen(ebnfdfname, "r")))
	{
		printf("Error opening ebnf-File: %s\n", ebnfdfname);
		exit(EXIT_FAILURE);
	}
	getsym();
	do 
	{
		if (('A' <= sym) && (sym <= 'Z'))
		{
			h=find(sym);     /* Element bereits aufgenommen ? */
			getsym();
		}
		else 
			error('i');
		if (sym == '=')
			getsym();
		else
			error('=');
		expression(&(h->entry), &q, &r, &s);
		link(r,NULL); 
		if (sym == '.')
			getsym();
		else 
			error('.');
	} while (sym != '$');
	
	printf("\n\nEBNF Parsed successfully !\n");
	h=list;
	while (h != sentinel)
	{
		if (!(h->entry))
		{
			printf("undefinded symbol: %c\n",h->sym);
			exit(0);
		}
		h=h->suc;
	}
	getsym();
	h=find(sym);
	printf(" is the Start-symbol !\n\n");
	
	fclose(file);
	if (!(file=fopen(parsefdname, "r")))
	{
		printf("Error opening Parse-File: %s\n", parsefdname);
		exit(0);
	}

	getsymg();
	noerr=parse(h,&match);
	if (noerr && (sym==EOF))
		printf("\n\nParsed sucessfully !\n");
	else
		printf("<--incorrect !\n");
		
	
	ma(0);
return 0;
}

void expression(struct node** p, struct node** q, struct node** r, struct node** s)
{
	struct node* q1;
	struct node* s1;
	term(p, q, r, s);
	while (sym == '|') 
	{
		getsym();
		term(&((*q)->alt), &q1, &((*s)->suc), &s1);
		*q=q1;
		*s=s1;
	}
}

void term(struct node** p, struct node** q, struct node** r, struct node** s)
{
	struct node* p1;
	struct node* q1;
	struct node* r1;
	struct node* s1;

	factor(p, q, r, s);
	while ((('A' <= sym) && (sym <= 'Z')) || (sym == '(' || sym =='[' || sym =='{' || sym =='"')) 
	{
		factor(&p1, &q1, &r1, &s1); 
		link(*r,p1);
		*r=r1;
		*s=s1;
	}
}

void factor(struct node** p, struct node** q, struct node** r, struct node** s)
{
	struct node* a;
	struct header* h;
	if (('A' <= sym) && (sym <= 'Z'))    /* nichtterminales Symbol auf der rechten Seite ? */
	{
		a = (struct node*)ma(sizeof(struct node));  /* Ok, Platz schaffen ... */
		h=find(sym);         /* Symbol schon mal aufgetaucht ? */
		a->type='p';         /* 'p' für pointer (nicht terminales Symbol) */
		(*a).head.nsym=h;    /* Symbolname auf Listeneintrag zeigen lassen */
		(*a).alt=NULL;       /* vorerst keine Alternative bzw. Nachfolger */
		(*a).suc=NULL;
		*p=a; *q=a; *r=a; *s=a;
		getsym();
    }
	else
		switch (sym){
			case '"':
				getsym();     /* Aha ein Terminalsysmbol ! */
				a = (struct node*)ma(sizeof(struct node)); /* Platz schaffen */
				a->type='t';  /* 't' für terminales Symbol */
				(*a).head.tsym=sym;  /* Symbol selber eintragen */
				(*a).alt=NULL;       /* noch keine Alternative bzw. Folge */
				(*a).suc=NULL; 
				*p=a; *q=a; *r=a; *s=a;
				getsym();
				if (sym == '"')
					getsym();
				else
					error('"');
				break;
			case '(':
				getsym();
				expression(p, q, r, s);
				if (sym == ')')
					getsym();
				else
					error(')');
				break;
			case '[':
				getsym();
				expression(p, q, r, s);
				a = (struct node*)malloc(sizeof(struct node));
				a->type='t';  /* 't' für terminales Symbol */
				(*a).head.tsym=' ';
				(*a).alt=NULL;
				(*a).suc=NULL;
				(*q)->alt=a; (*s)->suc=a; *q=a; *s=a;
				if (sym == ']')
					getsym();
				else
					error(']');
				break;
			case '{':
				getsym();
				expression(p, q, r, s);
				link(*r,*p);
				a = (struct node*)malloc(sizeof(struct node));
				a->type='t';  /* 't' für terminales Sysmbol */
				(*a).head.tsym=' ';
				(*a).alt=NULL;
				(*a).suc=NULL;
				(*q)->alt=a; *q=a; *r=a; *s=a;
				if (sym == '}')
					getsym();
				else
					error('}');
				break;
			default:
				error(' '); }
}

int parse(struct header* goal,int* match)
{
	struct node* s;
	s=goal->entry;
	do {
		if (s->type=='t')
			if ((s->head.tsym)==sym)
			{
				*match=1;
				getsymg();
			}
			else
				*match=((s->head.tsym)==' ');
		else
			parse(s->head.nsym, match);
		if (*match)
			s=s->suc;
		else
			s=s->alt;
	} while (s);
	return (*match);
}








struct header* find(char s)
{
	/*Sucht in einer Liste nach s */
	struct header* h1;
	h1=list;          /* "Laufpointer" auf Listenanfang setzen */
	sentinel->sym=s;  /* Letzes Element enthält im Ernstfall das Gesuchte */
	while (h1->sym != s) h1=h1->suc;  /* Liste durchlaufen bis Element gefunden */
	if (h1==sentinel)                 /* wenn nicht enthalten (sentinel) ... */
		{
		sentinel = (struct header*)ma(sizeof(struct node)); /* ... sentinel neu anlegen */
		h1->suc = sentinel;  /* ehemaliger Sentinel wird Element, Folgeelement zeigt auf sentinel*/
		h1->entry=NULL;      /* Struktur noch nicht aufgebaut */
		}
	return h1;           /* Laufpointer auf Element wird zurückgegeben */
}

void link(struct node* p, struct node* q)
{
	struct node* t;
	while (p)
		{
		t=p;
		p=t->suc;
		t->suc=q;
		}
}

void error(char errorid)
{ /* Gibt bei regelwidrigem Parsen eine entrechende Fehlermeldung aus */
printf("<-- EBNF Parsing Syntax Error:\n\n");
switch (errorid) {
       case '"':
            printf("'\"' Expected\n");
            break;
       case ')':
            printf("')' Expected\n");
            break;
       case ']':
            printf("']' Expected\n");
            break;
       case '}':
            printf("'(' Expected\n");
            break;
       case '.':
            printf("'.' Expected\n");
            break;
       case '=':
            printf("'=' Expected\n");
            break;
       case 'i':
            printf("illegal identifier \n");
            break;
       default:
            printf("Unkwnown Syntax Error\n");
            break;}
printf("(whitespaces between literals !?)");
getchar();
exit(0);
}

