#define NUMBER 257
#define VAR 258
#define BLTIN 259
#define INDEF 260
#define UNARYMINUS 261
typedef union {
	double val;
	Symbol *sym;
} YYSTYPE;
extern YYSTYPE yylval;
