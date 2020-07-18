# -*- Makefile -*-
#target: dependencies
#	action

my_tar: my_tar.o getOpt.o fManips.o
	gcc my_tar.o getOpt.o fManips.o -o my_tar  -Wall -Wextra -Werror
my_ls.o: my_tar.c
	gcc -c my_tar.c  
getOpt.o: getOpt.c
	gcc -c getOpt.c  -Wall -Wextra -Werror 
fManips.o: fManips.c internalFuncts.c
	gcc -c fManips.c internalFuncts.c -Wall -Wextra -Werror  
clean:
	rm -f *.o *.gch
