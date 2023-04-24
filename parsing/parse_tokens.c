/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seulee2 <seulee2@42seoul.student.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 15:15:26 by yunjcho           #+#    #+#             */
/*   Updated: 2023/04/24 21:00:49 by seulee2          ###   ########.fr       */
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
	if (!ft_strcmp(ft_strlowcase(cmd), "echo"))
		return(1);
	if (!ft_strcmp(ft_strlowcase(cmd), "pwd"))
		return (1);
	if (!ft_strcmp(ft_strlowcase(cmd), "env"))
		return (1);
	if (!ft_strcmp(cmd, "cd"))
		return (1);
	if (!ft_strcmp(cmd, "export"))
		return (1);
	if (!ft_strcmp(cmd, "unset"))
		return (1);
	if (!ft_strcmp(cmd, "exit"))
		return (1);
	return (0);
}

// redirection이 있는 토큰은 따로 처리.. 여기서도 따옴표 확인 계속 해야 함
// <, <<, > , >> 인지 체크, infile, outfile 업데이트
void	io_here_token(char *str, int idx, t_token *token)
{
	(void) str;
	(void) idx;
	(void) token;
	return;
}

// 리다이렉션 처리 필요
void	parse_command(char *str, t_token *token)
{
	int		i;
	int		len;
	int		quote;
	char	res[ARG_MAX];
	char	charset[1];

	charset[0] = -1;
	printf("**str: %s\n", str);
	ft_memset(res, 0, ARG_MAX); // 버퍼 초기화
	i = 0;
	// 처음 들어오는 공백 넘김
	while (is_blank(str[i]))
		i++;
	quote = 0;
	len = 0;
	while (str[i])
	{
		if (!quote && (str[i] == '\'' || str[i] == '\"'))
		{
			quote = str[i];
		}
		else if (quote && str[i] == quote)
		{
			quote = 0;
		}
		else if ((!quote && str[i] == '$') || (quote == '\"' && str[i] == '$')) // $ 나오면 환경변수 아닌 것 까지 보고 자름
		{
			// env 쭉 보면서 환경변수 있는지 확인하고 res에 복사
			// printf("***len: %d, ***res: %p\n", len, res);
			len += env_trans(&str[i + 1], &i, &res[len], token->env);
			// printf("***len: %d, ***res: %s\n", len, res);
		}
		// else if (!quote && (str[i] == '<' || str[i] == '>')) // 리다이렉션 있으면 io_here 토큰으로 분리하여 담음
		// {
		// 	io_here_token(str, i, token);
		// 	return ;
		// } 
		else if (!quote && is_blank(str[i]))
		{
			res[len++] = -1; // 따옴표 밖 공백이면 자름
		}
		else
		{
			res[len++] = str[i];
		}
		// printf("str[%d]: %c res: %s quote: %d\n", i, str[i], res, quote);
		i++;
	}
	res[len] = '\0';
	// printf("res : %s\n", res);
	token->command = ft_split(res, charset);
	if (is_builtin(token->command[0]))
		token->state = BUILTIN;
}

// cmd 와 arg로 분리
static void	init_token(char *str, t_token *token, char **env)
{
	token->env = env;
	//TODO - infile/outfile
	token->state = GENERAL;
	token->redir = NONE;
	token->infile = NULL;
	token->outfile = NULL;
	parse_command(str, token);
	token->prev = NULL;
	token->next = NULL;
}

void	make_cmdlst(char *str, t_deque *cmd_deque, char **env)
{
	char		**strs;
	t_token		*token;
	int			i;
	
	i = 0;
	// quotes 안에 | 따로 처리 필요
	strs = ft_pipe_split(str);
	while (strs[i])
	{
		// printf("str[%d] : %s\n", i, strs[i]);
		token = malloc(sizeof(t_token));
		if(!token)
			ft_error();
		init_token(strs[i], token, env);
		append_back(cmd_deque, token);
		i++;
	}
	// free_strs(parsed);
	// free_strs(strs);
	print_deque(cmd_deque);
}
