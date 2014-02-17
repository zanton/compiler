CC := gcc
CFLAGS := -Wall -g
SYNTREE_OBJS := util.o list.o syntree.o
MY_OBJS := parser.o tokenizer.o cogen_main.o char_buf.o cogen.o environment.o util2.o

parser: $(SYNTREE_OBJS) $(MY_OBJS)
	$(CC) -o cogen $(SYNTREE_OBJS) $(MY_OBJS)

clean:
	rm -f $(SYNTREE_OBJS) $(MY_OBJS)

