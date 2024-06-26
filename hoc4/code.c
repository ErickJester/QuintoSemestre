#include "hoc.h" 
#include "y.tab.h"
#define NSTACK  256
static  Datum  stack[NSTACK];  /* la pila */
static  Datum   *stackp;       /* siguiente lugar libre en la pila */
#define NPROG   2000
Inst    prog[NPROG];    /* RAM la m�quina */
Inst    *progp;         /* siguiente lugar libre para la generaci�n de c�digo */
Inst    *pc;	/* contador de programa durante la ejecuci�n */


void initcode(){      /* inicializaci�n para la generaci�n de c�digo */ 
   stackp = stack;
   progp = prog;
}

void push(Datum d){	/*  meter d en la pila  */
   if (stackp >= &stack[NSTACK]){
      execerror("stack overflow", (char *) 0);
      *stackp++ = d;
   }
   
}


Datum pop( ){     /* sacar y retornar de la pila el elemento del tope */
if (stackp <= stack)
execerror("stack underflow", (char *) 0);
return  *--stackp;
}
void constpush( ){	/* meter una constante a la pila  */
Datum d;
d.val  =  ((Symbol  *)*pc++)->u.val;
push(d);
}

void varpush( )	{/* meter una variable a la pila   */
Datum d;
d.sym  =  (Symbol   *)(*pc++);
push(d);
}

void eval( )	/*  evaluar una variable en la pila   */
{
Datum  d;
d   =  pop();
if   (d.sym->type   ==   INDEF)
execerror("undefined variable",   
d.sym->name); 
d.val   =  d.sym->u.val; push(d);
}

void add( ){	/*   sumar los dos elementos superiores de la pila   */
Datum d1,   d2; 
d2  =  pop(); 
d1  =  pop(); 
d1.val  +=  d2.val; 
push(d1); 
}

void sub( ){
Datum d1,  d2; 
d2  = pop(); 
d1  = pop(); 
d1.val  -= d2.val; 
push(d1);
}

void mul( ){
Datum d1, d2;
d2 = pop(); 
d1 = pop(); 
d1.val *= d2.val; 
push(d1);
}
void div( ){
Datum d1, d2;
d2 = pop();
if (d2.val == 0.0)
execerror("division by zero", (char *)0);
d1 = pop(); 
d1.val /= d2.val; 
push(d1);
}

negate()
{
Datum d; 
d = pop(); 
d.val  =  -d.val; 
push(d);
}

power(){
Datum d1, d2;
extern double Pow();
d2 = pop();
d1 = pop();
d1.val = Pow(d1.val, d2.val);
push(d1);
}


void assign( )        /* asignar el valor superior al siguientevalor */ 
{
Datum d1, d2;
d1 = pop();
d2 = pop();
if (d1.sym->type != VAR && d1.sym->type != INDEF) 
execerror("assignment to non-variable", d1.sym->name);
d1.sym->u.val = d2.val;
d1.sym->type = VAR;
push(d2); 
} 

print( )  /* sacar el valor superior de la pila e imprimirlo */ 
{
Datum d;
d = pop();
printf("\t%.8g\n",   d.val);
}

void bltin( )/*  evaluar un predefinido en el tope de la pila  */
{
Datum d;
d  =  pop();
d.val  =   (*(double   (*)(double))(*pc++))(d.val);
push(d);
}
Inst   *code(Inst f){ /*   instalar una instrucci�n u operando   */
   Inst *oprogp = progp;
	if (progp >= &prog [ NPROG ])
		execerror("program too big", (char *) 0);
	*progp++ = f;
        // *progp = f;
        // progp = progp+1;
	return oprogp;
}
void execute(Inst *p){	/*   ejecuci�n con la m�quina   */
for  (pc  =  p;   *pc != STOP; ) 
	(*pc++)();
}














