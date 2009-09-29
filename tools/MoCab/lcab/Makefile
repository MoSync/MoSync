OBJS = cheader.o cfolder.o cdata.o cfile.o checksum.o cwrite.o main.o

CC = gcc
CFLAGS = -ggdb3 -Wall -pipe
EXE = lcab

all: $(EXE)

$(EXE): $(OBJS)
	$(CC) -o $(EXE) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean: 
	-rm -f $(OBJS) $(EXE)


