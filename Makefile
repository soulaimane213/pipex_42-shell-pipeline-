
srcs = main.c pipex.h utils.c
output = pipex


all:
	gcc -g $(srcs) -o $(output)

clean:
	rm -f $(output)

