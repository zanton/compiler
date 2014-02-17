#
# makefile for testing cogen
#
# 'make' will do the following for each of fXX.c files in this directory.
# 
# ./cogen fXX.c > fXX.s
# gcc -o fXX.exe fXX.s main.c
# gcc -o fXX.gexe fXX.c main.c
# compare outputs by fXX.exe and fXX.gexe
#


CC=gcc
CFLAGS=-g

C_FILES:=$(wildcard f*.c)
EXE_FILES:=$(subst .c,.exe,$(C_FILES))
GEXE_FILES:=$(subst .c,.gexe,$(C_FILES))
CMP_FILES:=$(subst .c,.cmp,$(C_FILES))

all : $(CMP_FILES)

%.exe : %.c main.c
	./cogen $(COGEN_FLAGS) $*.c > $*.s 
	gcc $(CFLAGS) -c $*.s
	gcc $(CFLAGS) -o $@ $*.o $(BUILTIN) main.c

%.gexe : %.c main.c
	gcc $(CFLAGS) -o $@ $^

%.cmp : %.gexe %.exe
	./$*.gexe > $*.gexe.out
	./$*.exe > $*.exe.out
	diff $*.gexe.out $*.exe.out

clean :
	rm -f *.s *.o *.exe *.gexe *.exe.out *.gexe.out
