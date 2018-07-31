CC		= gcc
CFLAGS	= -g -Wall
EXEC 	= myprog
OBJS 	= generate.o main.o sem.o sharedmem.o
SRCS 	= generate.c main.c sem.c sharedmem.c


.PHONY : all
all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

generate.o: generate.c
	$(CC) $(CFLAGS) -c generate.c

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

sem.o: sem.c
	$(CC) $(CFLAGS) -c sem.c

sharedmem.o: sharedmem.c
	$(CC) $(CFLAGS) -c sharedmem.c



.PHONY: clean
clean:
	rm -f $(OBJS) $(EXEC)