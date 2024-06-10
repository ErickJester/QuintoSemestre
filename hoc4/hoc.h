typedef struct Symbol { /* entrada de la tabla de s�mbolos */
	char   *name;
	short   type;   /* VAR, BLTIN, UNDEF */
	union {
		double val;	       /* si es VAR */
		double  (*ptr)();      /* s� es BLTIN */
	} u;
	struct Symbol   *next;  /* para ligarse a otro */ 
} Symbol;

Symbol *install(char *s,int t, double d), *lookup(char *s);

typedef union Datum {   /* tipo de la pila del int�rprete */
double  val;
Symbol  *sym; } Datum; 

extern Datum pop();
typedef void (*Inst)();  /* instrucci�n de m�quina */ 

#define STOP    (Inst) 0
extern	Inst prog[];
extern	eval(), add(), sub(), mul(), div(), negate(), power();
extern	assign(), bltin(), varpush(), constpush(), print();
void execerror(char *s, char *t);