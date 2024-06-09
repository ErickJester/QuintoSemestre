Gram=y.tab.c y.tab.h

all: $(Gram) 
	@gcc -o calc y.tab.c symbol.c init.c math.c -lm
	@echo Compiled

$(Gram): hoc3.y
	@yacc -d hoc3.y
clean:
	@rm -f  *.tab.* calc 
	@echo Clean
