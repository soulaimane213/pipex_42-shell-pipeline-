#ifndef PIPEX_H
#define PIPEX_H
 
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFFER_SIZE 100





// ft_split fcuntion declaration
char	**ft_split(char *str, char c);

// the get path fcuntion
char *get_path(char *cmd  , char **envp);


#endif
