CC     = gcc
CFLAGS = -Wall -Wextra -g
SRCS   = main.c utils.c
NAME   = pipex

all: $(NAME)

$(NAME): $(SRCS) pipex.h
	$(CC) $(CFLAGS) $(SRCS) -o $(NAME)

clean:
	rm -f $(NAME) a.out pipex.h.gch

.PHONY: all clean
