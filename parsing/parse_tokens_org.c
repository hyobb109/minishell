/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunjcho <yunjcho@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 15:15:26 by yunjcho           #+#    #+#             */
/*   Updated: 2023/05/06 14:02:10 by yunjcho          ###   ########.fr       */
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

char	**parse_command(char *str, t_token *token, int quote)
{
	int		len;
	int		q_flag;
	char	**cmds;
	char	buffer[ARG_MAX];

	cmds = NULL;
	ft_memset(buffer, 0, ARG_MAX);
	while (is_blank(*str))
		str++;
	quote = CLOSED;
	q_flag = FALSE;
	len = 0;
	while (*str)
	{
		if (!quote && (*str == '\'' || *str == '\"'))
		{
			quote = *str;
			q_flag = TRUE;
		}
		else if (quote && *str == quote)
		{
			if (*(str - 1) == quote)
				buffer[len++] = EMPTY;
			quote = CLOSED;
		}
		else if (!quote && (*str == '<' || *str == '>'))
			check_redir(&str, token);
		else if ((!quote && *str == '$') || (quote == '\"' && *str == '$'))
		{
			if (ft_isalpha(*(str + 1)) || *(str + 1) == '_')
				len += search_env(&str, &buffer[len], token->envp, quote);
			else if (*(str + 1) == '?')
			{
				printf("exit: %d\n", g_exit_status);
				char *status = ft_itoa(WEXITSTATUS(g_exit_status));
				int num_len = ft_strlen(status);
				ft_memcpy(&buffer[len], status, num_len);
				len += num_len;
				str++;
				if (quote)
					str++;
			}
			else
				str++;
			if (quote)
				quote = CLOSED;
		}
		else if (!quote && is_blank(*str))
			buffer[len++] = BLANK;
		else
			buffer[len++] = *str;
		if (*str == '\0')
			break ;
		str++;
	}
	buffer[len] = '\0';
	if (buffer[0])
	{
		cmds = ft_split(buffer, BLANK);
		check_empty_str(cmds);
	}
	return (cmds);
}

// cmd 와 arg로 분리
static void	init_token(char *str, t_token *token, t_edeque *envp)
{
	token->envp = envp;
	//TODO - infile/outfile
	token->func = GENERAL;
	token->files = NULL;
	token->command = parse_command(str, token, CLOSED);
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
	// quotes 안에 | 따로 처리 필요
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
