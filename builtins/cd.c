/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunjcho <yunjcho@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 01:25:35 by yunjcho           #+#    #+#             */
/*   Updated: 2023/04/25 16:24:04 by yunjcho          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	chdir_home(void)
{
	char	*dest;

	dest = getenv("HOME");
	printf("dest : %s\n", dest);
	// if (access(dest, F_OK) == -1)
	// 	return (0);
	if (chdir(dest) == -1)
	{
		perror(dest);
		return (-1);
	}
	return (1);
}

// int	find_cutidx(char *str)
// {
// 	int	len;

// 	if (!str)
// 		return (-1); //ft_error();
// 	len = ft_strlen(str);
// 	while (len >= 0)
// 	{
// 		if (str[len] == '/')
// 			break ;
// 		len--;
// 	}
// 	return (len);
// }

char	*make_dirstr(char *str)
{
	int		idx;
	char	*result;
	char	**tmp;

	idx = 0;
	result = malloc(1);
	*result = 0;
	tmp = ft_split(str, "/");
	while (tmp[idx])
		idx++;
	free(tmp[idx - 1]);
	tmp[idx - 1] = 0;

	idx = 0;
	while (tmp[idx])
	{
		result = ft_strjoin_three(result, "/", tmp[idx]);
		printf("tmp[%d] %s\n", idx, tmp[idx]);
		free(result);
		idx++;
	}
	printf("result : %s\n", result);
	return (result);
}

int	exec_cd(t_token *token)
{
	int		result;
	char	*dest;

	result = 0;
	dest = NULL;

	char	cwd_name[PATH_MAX];
	getcwd(cwd_name, sizeof(cwd_name));

	if (exist_args(token))
	{
		if (!ft_strcmp(token->command[1], "~"))
			result = chdir_home();
		else if (!ft_strcmp(token->command[1], ".."))
		{
			dest = getenv("PWD");
			make_dirstr(dest);
		}
		else
		{
			dest = ft_strjoin_three(cwd_name, "/", token->command[1]);
			printf("dest : %s\n", dest);
			result = chdir(dest);
		}
	}
	else
		result = chdir_home();
	// free (dest); //에러 메시지
	if (result == -1)
	{
		printf("cd: %s: %s\n", token->command[1], strerror(errno)); // perror(token->command[1]);
		exit (EXIT_FAILURE);// ft_error();
	}
	return (1);
}
