/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunjcho <yunjcho@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 15:15:26 by yunjcho           #+#    #+#             */
/*   Updated: 2023/04/30 22:01:04 by yunjcho          ###   ########.fr       */
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

char	**parse_command(char *str, t_token *token)
{
	int		i;
	int		len;
	int		quote;
	char	buffer[ARG_MAX];

	// printf("**str: %s\n", str);
	ft_memset(buffer, 0, ARG_MAX); // 버퍼 초기화
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
		// echo hi >> "$USER hi $LANG" <$USER >>append1 >>ap'en'd2 <<"heredoc" <<h_noquote <"infile $USER" <infi 
		// 리다이렉션 있으면 command 에 넣지 않고 분리하여 토큰 파일 리스트에 파일 이름과 형식 추가
		else if (!quote && (str[i] == '<' || str[i] == '>'))
		{
			// printf("*str: %s\n", &str[i]);
			i += check_redir(&str[i], token);
		} 
		else if (!quote && is_blank(str[i]))
		{
			buffer[len++] = BLANK; // 따옴표 밖 공백이면 자름
		}
		else
		{
			buffer[len++] = str[i];
		}
		i++;
		// printf("str[%d]: %c buffer: %s quote: %d\n", i, str[i], buffer, quote);
	}
	buffer[len] = '\0';
	// printf("buffer : %s\n", buffer);
	return (ft_split(buffer, BLANK));
}

// cmd 와 arg로 분리
static void	init_token(char *str, t_token *token, t_edeque *envp)
{
	token->envp = envp;
	//TODO - infile/outfile
	token->state = GENERAL;
	token->files = NULL;
	token->command = parse_command(expand_environ(str, token, CLOSED), token);
	if (is_builtin(token->command[0]))
		token->state = BUILTIN;
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
	// print_filelst(cmd_deque->head->files);
	free_strs(strs);
	// print_deque(cmd_deque);
}
