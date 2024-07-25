# Makefile.

CFLAGS = -Wall -g -O2# -Wshadow
OBJS = theboys.o libset.o liblef.o liblista.o# Todos os objetos
NAME = theboys
CC = gcc# Compilador padr∆o
RM = rm -f# Deletar todos os objetos

.PHONY: all clean

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) -o $@ $(OBJS) -lm

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	$(RM) $(OBJS) *~ $(NAME)

vrun: $(NAME)
	valgrind --leak-check=full --track-origins=yes ./$(NAME)

