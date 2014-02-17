CC=gcc
CFLAGS=-Wall -g
OBJS=tokenizer.o tokenizer_main.o
tokenizer_main : $(OBJS)
	gcc -o $@ $(OBJS)
clean :
	rm -f $(OBJS)