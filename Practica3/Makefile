Gram=y.tab.c y.tab.h

all: $(Gram)
	@gcc -o vect y.tab.c symbol.c init.c math.c vector_calc.c -lm
	@echo Compiled

$(Gram): vector.y
	@yacc -d vector.y

clean:
	@rm -f *.tab.* vect
	@echo Clean