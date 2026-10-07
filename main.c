#include "pipex.h"

static void execute_command(char *cmd_str, char **envp) {
    char **cmd = ft_split(cmd_str, ' ');
    if (!cmd || !cmd[0] || cmd[0][0] == '\0') {
        fprintf(stderr, "pipex: command not found: %s\n", cmd_str);
        exit(127);
    }

    char *path = NULL;
    if (cmd[0][0] == '/' || (cmd[0][0] == '.' && cmd[0][1] == '/')) {
        path = cmd[0];
    } else {
        path = get_path(cmd[0], envp);
        if (!path) {
            fprintf(stderr, "pipex: command not found: %s\n", cmd[0]);
            exit(127);
        }
    }

    if (execve(path, cmd, envp) == -1) {
        perror("pipex: execve error");
        exit(127);
    }
}

int main(int ac, char **av, char **envp) {
    if (ac != 5) {
        fprintf(stderr, "Usage: ./pipex <infile> <cmd1> <cmd2> <outfile>\n");
        return 1;
    }

    int p[2];
    if (pipe(p) < 0) {
        perror("pipex: pipe error");
        return 1;
    }

    // Fork Child 1 for cmd1: reads from infile, writes to pipe
    pid_t pid1 = fork();
    if (pid1 < 0) {
        perror("pipex: fork error");
        return 1;
    }

    if (pid1 == 0) {
        // Child 1
        close(p[0]); // Close unused read end

        // Open infile in read-only mode
        int fd_in = open(av[1], O_RDONLY);
        if (fd_in < 0) {
            perror(av[1]);
            close(p[1]);
            exit(1);
        }

        dup2(fd_in, STDIN_FILENO);
        dup2(p[1], STDOUT_FILENO);
        close(fd_in);
        close(p[1]);

        execute_command(av[2], envp);
    }

    // Fork Child 2 for cmd2: reads from pipe, writes to outfile (Concurrent execution)
    pid_t pid2 = fork();
    if (pid2 < 0) {
        perror("pipex: fork error");
        return 1;
    }

    if (pid2 == 0) {
        // Child 2
        close(p[1]); // Close unused write end

        // Open/create outfile with O_TRUNC
        int fd_out = open(av[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd_out < 0) {
            perror(av[4]);
            close(p[0]);
            exit(1);
        }

        dup2(p[0], STDIN_FILENO);
        dup2(fd_out, STDOUT_FILENO);
        close(p[0]);
        close(fd_out);

        execute_command(av[3], envp);
    }

    // Parent: close both pipe ends and wait for both children
    close(p[0]);
    close(p[1]);

    int status = 0;
    waitpid(pid1, NULL, 0);
    waitpid(pid2, &status, 0);

    if (WIFEXITED(status)) {
        return WEXITSTATUS(status);
    }
    return 0;
}
