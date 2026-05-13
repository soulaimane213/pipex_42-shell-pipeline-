#include "pipex.h"


int main(int ac , char **av , char **envp){

	int p[2];
	int fd;
	size_t bytes;
	char buffer[BUFFER_SIZE];
	pid_t pid; 
	char **cmd = NULL;
	char *path = NULL;
	int saved_stdout;


	// first we will check if the arg == 5 or not 
	
	if(ac == 5){
	
		// first we will open the file and check that there
		// is no probleme
		fd = open(av[1] , O_RDWR , 0);
		if(fd < 0){
			perror(av[1]);
			return 1;
		}
		
		// now we need to create the pipe;
		if(pipe(p) < 0){
			perror("pipe");
			return 1;
		}
		
		pid_t pid = fork();
		
		if(pid < 0){
			perror("fork");
			return 1;
		}


		if(pid == 0){
			// close the read pipe becuase we dont need it here bruuh;
			close(p[0]);
			
			saved_stdout = dup(1);
			
			// make the stdin point to the opne fd file descriptor;	
			dup2(fd ,0);
			
			// and make the the stdout point to the fd[1] pipe
			dup2(p[1] , 1);


           		//dup2(saved_stdout , 1);


			// then here we need to split the command into array of pointers;
			cmd = ft_split(av[2] , ' ');
			if(cmd == NULL || !cmd[0] || cmd[0][0] == 0){
				dup2(saved_stdout , 1);
				printf("command not found: :( \n");
				return -1;
			}	
	


			// then here we need to check the first arg is 
			// it available in the path or no
			
			if(cmd[0][0] == '/' || (cmd[0][0] == '.' && cmd[0][1] == '/')){
				path = cmd[0];
			}else {
				
				path = get_path(cmd[0] , envp);	
				if(path == NULL){
					dup2(saved_stdout , 1);
					printf("command not found: %s :( \n" , cmd[0]);
					return -1;
				}
			}

			// then here executed with execve();
			execve(path , cmd , envp);
			perror("execve:");

			// then close the fd[1];
			close(saved_stdout);


		}else {
			close(p[1]);	
			wait(NULL);

			// first we ned to maeke the stdin points to the fd of fd[1]
			dup2(p[0] , 0);

			// and now we need to open the file / create it if its not exist
			fd = open(av[4] ,O_WRONLY | O_CREAT , 0644);
				if(fd < 0){
				perror("open");
				return 1;
			}
		

			// then here we need to make the the stdout point also to the 
		
			// save the stdout
			saved_stdout = dup(1);

			// open file descriptor
			dup2(fd , 1);



			// now we will execute the second command  normally;

			// then here we need to split the command into array of pointers;
			cmd = ft_split(av[3] , ' ');
		
			if(cmd == NULL || !cmd[0] || cmd[0][0] == 0){
				dup2(saved_stdout , 1);
				printf("command not found: :( \n");
				return -1;
			}	
	

			// then here we need to check the first arg is 
			// it available in the path or no
			
			if(cmd[0][0] == '/' || (cmd[0][0] == '.' && cmd[0][1] == '/')){
			
				path = cmd[0];
			
			}else {
				
				path = get_path(cmd[0] , envp);	
				if(path == NULL){
					dup2(saved_stdout , 1);
					printf("command not found: %s :( \n" , cmd[0]);
					return -1;
				}
			}


			




			// then here executed with execve();
			execve(path , cmd , envp);
			perror("execve:");


			close(p[0]);
	

		}

	}else {
		printf("Usage: ./pipex infile 'cmd_1' 'cmd_2' outfile\n");
		return 1;
	}




	close(p[1]);
	close(p[0]);
	close(fd);

}


