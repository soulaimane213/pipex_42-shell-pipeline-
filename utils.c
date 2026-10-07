#include "pipex.h"

char *ft_substr(char *s, int start, int len) {
    char *buf = malloc(len + 1);
    if (!buf)
        return NULL;

    int i = 0;
    while (i < len) {
        buf[i] = s[start + i];
        i++;
    }
    buf[i] = '\0';
    return buf;
}

char **ft_split(char *str, char c) {
    if (!str) return NULL;

    int flag = 0;
    int i = 0;
    int count = 0;

    while (str[i] != '\0') {
        if (str[i] != c) {
            flag = 1;
        } else if (str[i] == c && flag == 1) {
            count++;
            flag = 0;
        }
        i++;
    }
    if (flag == 1)
        count++;

    char **result = malloc(sizeof(char *) * (count + 1));
    if (!result)
        return NULL;

    flag = 0;
    i = 0;
    int j = 0;
    int len = 0;

    while (str[i] != '\0') {
        if (str[i] != c) {
            len++;
            flag = 1;
        } else if (str[i] == c && flag == 1) {
            int start = i - len;
            result[j] = ft_substr(str, start, len);
            j++;
            len = 0;
            flag = 0;
        }
        i++;
    }
    if (flag == 1) {
        int start = i - len;
        result[j] = ft_substr(str, start, len);
        j++;
    }
    result[j] = NULL;
    return result;
}

char *ft_join(char *envp_path, char *cmd) {
    int cmd_len = strlen(cmd);
    int env_len = strlen(envp_path);

    char *path = malloc(env_len + cmd_len + 2);
    if (!path) return NULL;

    int i = 0, j = 0;
    while (envp_path[i]) {
        path[j++] = envp_path[i++];
    }
    path[j++] = '/';

    i = 0;
    while (cmd[i]) {
        path[j++] = cmd[i++];
    }
    path[j] = '\0';
    return path;
}

char *get_path(char *cmd, char **envp) {
    if (!cmd || !envp) return NULL;

    int i = 0;
    char *start = NULL;

    // Search for PATH= in environment variables
    while (envp[i] != NULL) {
        if (strncmp(envp[i], "PATH=", 5) == 0) {
            start = envp[i] + 5;
            break;
        }
        i++;
    }

    // Safety: if environment has no PATH, return NULL without segfaulting
    if (!start) return NULL;

    char **paths = ft_split(start, ':');
    if (!paths) return NULL;

    i = 0;
    while (paths[i] != NULL) {
        char *path = ft_join(paths[i], cmd);
        if (path && access(path, X_OK) == 0) {
            // Free remaining paths and return matched path
            return path;
        }
        free(path);
        i++;
    }

    return NULL;
}
