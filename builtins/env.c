/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunjcho <yunjcho@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 20:42:23 by yunjcho           #+#    #+#             */
/*   Updated: 2023/05/02 19:24:02 by yunjcho          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	init_validkeyflag(t_token *token, int idx, char *tmp, int *flag)
{
	if (!ft_strncmp(tmp, "pwd", 4))
		*flag = 1;
	else if (!ft_strncmp(tmp, "log", 4))
		*flag = 3;
	else if (!ft_strncmp(tmp, "logname", 8))
		*flag = 2;
	if (*flag < 3 && !token->command[idx + 1])
		return (*flag);
	else
	{
		if (*flag == 1)
			printf("usage: pwd No need options and args\n");
		else if (*flag == 3)
			printf("usage: logname\n");
		*flag = 5;
	}
	return (*flag);
}

int	exist_validkey(t_token *token)
{
	int		idx;
	int		flag;
	char	*tmp;

	idx = 1;
	flag = 0;
	while (token->command[idx])
	{
		tmp = ft_strlowcase(ft_strjoin("", token->command[idx]));
		if (ft_strcmp(tmp, "env"))
		{
			if (init_validkeyflag(token, idx, tmp, &flag) > 0)
			{
				free(tmp);
				return (flag);
			}
		}
		free(tmp);
		idx++;
	}
	tmp = ft_strlowcase(ft_strjoin("", token->command[idx - 1]));
	if (!ft_strcmp(tmp, "env"))
	{
		print_envlist(token);
		free(tmp);
		return (4);
	}
	free(tmp);
	return (0);
}

void	print_invalidargserror(t_token *token)
{
	int		idx;
	int		print_idx;
	char	*msg_str;

	idx = 1;
	print_idx = 1;
	while (token->command[idx])
	{
		msg_str = ft_strlowcase(ft_strjoin("", token->command[idx]));
		if (ft_strcmp(msg_str, "env"))
		{
			free(msg_str);
			break ;
		}
		free(msg_str);
		idx++;
		print_idx++;
	}
	printf("%s: %s: %s\n", token->command[print_idx - 1], \
		token->command[print_idx], strerror(ENOENT));
	token->status = 127;
	return ;
}

void	print_envlist(t_token *token)
{
	char	*str;
	t_env	*tmp;

	str = NULL;
	tmp = token->envp->head;
	while (tmp)
	{
		if (!ft_strcmp(tmp->val, ""))
		{
			tmp = tmp->next;
			continue ;
		}
		str = ft_strjoin_three(tmp->key, "=", tmp->val);
		printf("%s\n", str);
		free(str);
		tmp = tmp->next;
	}
}

int	exec_env(t_token *token)
{
	int	flag;

	flag = 0;
	if (!exist_args(token))
		print_envlist(token);
	else
	{
		flag = exist_validkey(token);
		if (flag)
		{
			if (flag == 1)
				exec_pwd(token);
			else if (flag == 5)
				return (-1);
			else if (flag == 2)
				printf("%s\n", ft_getenv(token->envp, "LOGNAME"));
			return (1);
		}
		else
			print_invalidargserror(token);
	}
	return (1);
}
