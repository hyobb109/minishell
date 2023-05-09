/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyobicho <hyobicho@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 15:15:26 by yunjcho           #+#    #+#             */
/*   Updated: 2023/05/09 20:31:51 by hyobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	check_empty_str(char **cmds)
{
	char	*tmp;
	int		i;

	i = 0;
	if (!cmds)
		return ;
	while (cmds[i])
	{
		if (ft_strchr(cmds[i], EMPTY))
		{
			tmp = ft_strcpy(tmp, cmds[i]);
			free(cmds[i]);
			cmds[i] = tmp;
		}
		i++;
	}
}

static int	is_builtin(char *cmd)
{
	char	*tmp;
	int		res;

	res = 0;
	tmp = ft_strdup(cmd);
	if (!ft_strcmp(ft_strlowcase(tmp), "echo"))
		res = 1;
	else if (!ft_strcmp(ft_strlowcase(tmp), "pwd"))
		res = 1;
	else if (!ft_strcmp(ft_strlowcase(tmp), "env"))
		res = 1;
	else if (!ft_strcmp(cmd, "cd"))
		res = 1;
	else if (!ft_strcmp(cmd, "export"))
		res = 1;
	else if (!ft_strcmp(cmd, "unset"))
		res = 1;
	else if (!ft_strcmp(cmd, "exit"))
		res = 1;
	free(tmp);
	return (res);
}

static void	init_token(char *str, t_token *token, t_edeque *envp)
{
	t_vars	v;
	char	buffer[ARG_MAX];

	init_vars(&v, buffer, FALSE);
	while (is_blank(*str))
		str++;
	token->envp = envp;
	token->func = GENERAL;
	token->files = NULL;
	token->command = parse_command(str, token, &v, buffer);
	check_empty_str(token->command);
	if (token->command && is_builtin(token->command[0]))
		token->func = BUILTIN;
	token->status = 0;
	token->prev = NULL;
	token->next = NULL;
}

void	make_cmdlst(char *str, t_deque *cmd_deque, t_edeque *envp)
{
	t_token		*token;
	char		**strs;
	int			i;

	i = 0;
	strs = ft_pipe_split(str);
	while (strs[i])
	{
		token = malloc(sizeof(t_token));
		if (!token)
			ft_error();
		init_token(strs[i], token, envp);
		append_back(cmd_deque, token);
		i++;
	}
	free_strs(strs);
}
