/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyobicho <hyobicho@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 15:15:26 by yunjcho           #+#    #+#             */
/*   Updated: 2023/05/09 15:34:32 by hyobicho         ###   ########.fr       */
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

char	*check_cmd_environ(t_vars *v, t_token *token, char *str, char *buffer)
{
	char	*status;
	int		num_len;

	if (ft_isalpha(*(str + 1)) || *(str + 1) == '_')
		v->len += search_env(&str, buffer, token->envp, *v);
	else if (*(str + 1) == '?')
	{
		printf("exit: %d\n", g_exit_status);
		status = ft_itoa(WEXITSTATUS(g_exit_status));
		num_len = ft_strlen(status);
		ft_memcpy(buffer, status, num_len);
		v->len += num_len;
		str++;
		if (v->quote)
			str++;
	}
	else
		str++;
	if (v->quote)
		v->quote = CLOSED;
	return (str);
}

void	is_empty_str(t_vars *v, char *str, char *buffer)
{
	if (*(str - 1) == v->quote)
		buffer[v->len++] = EMPTY;
	v->quote = CLOSED;
}

char	**parse_command(char *str, t_token *token, t_vars *v, char *buffer)
{
	while (*str)
	{
		if (!v->quote && (*str == '\'' || *str == '\"'))
			v->quote = *str;
		else if (v->quote && *str == v->quote)
			is_empty_str(v, str, buffer);
		else if (!v->quote && (*str == '<' || *str == '>'))
			check_redir(&str, token);
		else if (is_environ(v->quote, *str))
			str = check_cmd_environ(v, token, str, &buffer[v->len]);
		else if (!v->quote && is_blank(*str))
			buffer[v->len++] = BLANK;
		else
			buffer[v->len++] = *str;
		if (*str == '\0')
			break ;
		str++;
	}
	buffer[v->len] = '\0';
	if (!buffer[0])
		return (NULL);
	return (ft_split(buffer, BLANK));
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

	// printf("tmp: %s\n", str);
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
