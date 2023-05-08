/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunjcho <yunjcho@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 15:15:26 by yunjcho           #+#    #+#             */
/*   Updated: 2023/05/08 22:42:29 by yunjcho          ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_blank(char c)
{
	if (c == ' ' || c == '\t' || c == '\n')
		return (1);
	return (0);
}

int	is_builtin(char *cmd)
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

void	check_empty_str(char **cmds)
{
	char	*tmp;
	int		i;

	i = 0;
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

char	**parse_command(char *str, t_token *token)
{
	t_vars	v;
	char	buffer[ARG_MAX];

	v.quote = CLOSED;
	v.len = 0;
	v.flag = 0;
	ft_memset(buffer, 0, ARG_MAX);
	while (is_blank(*str))
		str++;
	while (*str)
	{
		if (!v.quote && (*str == '\'' || *str == '\"'))
			v.quote = *str;
		else if (v.quote && *str == v.quote)
		{
			if (*(str - 1) == v.quote)
				buffer[v.len++] = EMPTY;
			v.quote = CLOSED;
		}
		else if (!v.quote && (*str == '<' || *str == '>'))
			check_redir(&str, token);
		else if ((!v.quote && *str == '$') || (v.quote == '\"' && *str == '$'))
		{
			if (ft_isalpha(*(str + 1)) || *(str + 1) == '_')
				v.len += search_env(&str, &buffer[v.len], token->envp, v);
			else if (*(str + 1) == '?')
			{
				printf("exit: %d\n", g_exit_status);
				char *status = ft_itoa(WEXITSTATUS(g_exit_status));
				int num_len = ft_strlen(status);
				ft_memcpy(&buffer[v.len], status, num_len);
				v.len += num_len;
				str++;
				if (v.quote)
					str++;
			}
			else
				str++;
			if (v.quote)
				v.quote = CLOSED;
		}
		else if (!v.quote && is_blank(*str))
			buffer[v.len++] = BLANK;
		else
			buffer[v.len++] = *str;
		if (*str == '\0')
			break ;
		str++;
	}
	buffer[v.len] = '\0';
	if (!buffer[0])
		return (NULL);
	return (ft_split(buffer, BLANK));
}

static void	init_token(char *str, t_token *token, t_edeque *envp)
{
	token->envp = envp;
	token->func = GENERAL;
	token->files = NULL;
	token->command = parse_command(str, token);
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
		if(!token)
			ft_error();
		// printf("str[%d] : %s\n", i, strs[i]);
		init_token(strs[i], token, envp);
		append_back(cmd_deque, token);
		i++;
	}
	// print_filelst(cmd_deque);
	free_strs(strs);
	// print_deque(cmd_deque);
}
