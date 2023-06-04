OBJS	= main.o
SOURCE	= main.c
HEADER	= encryption.h
OUT		= diary_project
CC		= gcc
FLAGS	= -g -c -Wall
LFLAGS	= 

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

main.o: main.c
	$(CC) $(FLAGS) main.c -std=c17

clean:
	rm -f $(OBJS) $(OUT)