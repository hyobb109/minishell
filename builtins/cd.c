/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunjcho <yunjcho@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 01:25:35 by yunjcho           #+#    #+#             */
/*   Updated: 2023/04/25 01:51:39 by yunjcho          ###   ########seoul.kr  */
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
		else
		{
			dest = ft_strjoin_three(cwd_name, "/", token->command[1]);
			printf("dest : %s\n", dest);
			result = chdir(dest);
		}
	}
	else
		result = chdir_home();
	free (dest);
	if (result == -1)
	{
		printf("cd: %s: %s\n", token->command[1], strerror(errno));
		// perror(token->command[1]);
		exit (EXIT_FAILURE);// ft_error();
	}
	return (1);
}
