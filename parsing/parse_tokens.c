/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyobicho <hyobicho@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 15:15:26 by yunjcho           #+#    #+#             */
/*   Updated: 2023/04/26 16:39:56 by hyobicho         ###   ########.fr       */
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
//<, <<, > , >> 인지 체크, infile, outfile 업데이트
int	io_here_token(char *str, t_token *token)
{
	int		i;
	t_fdata *newfile;

	i = 0;
	newfile = malloc(sizeof(t_fdata));
	if (!newfile)
		ft_error();
	newfile->next = NULL;
	// redirection flag check
	if (!strncmp(&str[i], "<<", 2))
	{
		newfile->type = DELIMITER;
		i += 2;
	}
	else if (!strncmp(&str[i], ">>", 2))
	{
		newfile->type = APPEND;
		i += 2;
	}
	else if (str[i] == '<')
	{
		newfile->type = INFILE;
		i++;
	}
	else if (str[i] == '>')
	{
		newfile->type = OUTFILE;
		i++;
	}
	return (get_filename(&str[i], newfile, token));
}

// 리다이렉션 처리 필요
void	parse_command(char *str, t_token *token)
{
	int		i;
	int		len;
	int		quote;
	char	res[ARG_MAX];

	// printf("**str: %s\n", str);
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
			//env 쭉 보면서 환경변수 있는지 확인하고 res에 복사
			// printf("***len: %d, ***res: %p\n", len, res);
			len += env_trans(&str[i + 1], &i, &res[len], token->envp);
			// printf("***len: %d, ***res: %s\n", len, res);
		}
		else if (!quote && (str[i] == '<' || str[i] == '>')) // 리다이렉션 있으면 io_here 토큰으로 분리하여 담음
		{
			i += io_here_token(&str[i], token);
			// printf("str[%d]: %c res: %s quote: %d\n", i, str[i], res, quote);
		} 
		else if (!quote && is_blank(str[i]))
		{
			res[len++] = -1; // 따옴표 밖 공백이면 자름
		}
		else
		{
			res[len++] = str[i];
		}
		i++;
	}
	res[len] = '\0';
	// printf("res : %s\n", res);
	token->command = ft_split(res, -1);
	if (is_builtin(token->command[0]))
		token->state = BUILTIN;
}

// cmd 와 arg로 분리
static void	init_token(char *str, t_token *token, t_edeque *envp)
{
	token->envp = envp;
	//TODO - infile/outfile
	token->state = GENERAL;
	token->files = NULL;
	parse_command(str, token);
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
		// printf("str[%d] : %s\n", i, strs[i]);
		token = malloc(sizeof(t_token));
		if(!token)
			ft_error();
		init_token(strs[i], token, envp);
		append_back(cmd_deque, token);
		i++;
	}
	// print_filelst(cmd_deque->head->files);
	// free_strs(parsed);
	// free_strs(strs);
	print_deque(cmd_deque);
}

