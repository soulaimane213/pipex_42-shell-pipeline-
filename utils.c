#include "pipex.h"


char	*ft_substr(char *s, int start, int len)
{
	char	*buf;
	int		i;

	buf = malloc(len + 1);
	if (!buf)
		return (NULL);

	i = 0;
	while (i < len)
	{
		buf[i] = s[start + i];
		i++;
	}

	buf[i] = '\0';
	return (buf);
}



char	**ft_split(char *str, char c)
{
	int		flag;
	int		i;
	int		j;
	int		len;
	int		start;
	int		count;
	char	**result;

	flag = 0;
	i = 0;
	len = 0;
	count = 0;

	while (str[i] != 0)
	{
		if (str[i] != c)
		{
			i++;
			len++;
			flag = 1;
		}
		else if (str[i] == c && flag == 1)
		{
			count++;
			len = 0;
			flag = 0;
			i++;
		}
		else
			i++;
	}
	if (flag == 1)
		count++;

	result = malloc(sizeof(char *) * (count + 1));
	if (!result)
		return (NULL);

	flag = 0;
	i = 0;
	j = 0;
	len = 0;
	start = 0;

	while (str[i] != 0)
	{
		if (str[i] != c)
		{
			i++;
			len++;
			flag = 1;
		}
		else if (str[i] == c && flag == 1)
		{
			start = i - len;
			result[j] = ft_substr(str, start, len);
			j++;
			len = 0;
			flag = 0;
			i++;
		}
		else
			i++;
	}
	if (flag == 1)
	{
		start = i - len;
		result[j] = ft_substr(str, start, len);
		j++;
	}

	result[j] = NULL;
	return (result);
}





// the join fucntion
char *ft_join(char *envp_path ,char *cmd ){
	
	int cmd_len = strlen(cmd);

	char *path = malloc( strlen(envp_path) + cmd_len + 2);
	int i =0;
	int j =0;

	while(envp_path[i]){
		
		path[j] = envp_path[i];
		j++;
		i++;
	}

	path[j++] = '/';
	
	i =0;

	while(cmd[i]){
		path[j] = cmd[i];
		i++;
		j++;
	}
	
	path[j] = 0;
	return path;
}



// the get path fcuntion (the fucntion that will check if this command 
// is in the bin folder or not!!!!);

char *get_path(char *cmd  , char **envp){

	int i =0;
	int j =0;
	int len =0;
	char *start =NULL;


	if(cmd == NULL || envp == NULL){
		return NULL;
	}


	while(envp[i] != NULL){
		
		if(strncmp(envp[i] , "PATH=" , 5) == 0){
			while(envp[i][j]){
				if(envp[i][j] == '='){
					start = &envp[i][j + 1];
					break;
				}
				j++;
			}	
			break;
		}	
				
		i++;
	}

	
	char **paths = ft_split(start , ':');
	i =0;

	while(paths[i] != NULL){

		char *path = ft_join(paths[i] , cmd);

		int acces_status = access(path ,  F_OK);
		if(acces_status == 0){
			return path;
		}
		i++;
	}	
	
	return NULL;

}









