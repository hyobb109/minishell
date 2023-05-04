/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyobicho <hyobicho@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 15:15:26 by yunjcho           #+#    #+#             */
/*   Updated: 2023/05/04 16:04:12 by hyobicho         ###   ########.fr       */
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

char	**parse_command(char *str, t_token *token, int quote)
{
	//	어차피 끝까지 볼거임
	int		len;
	int		q_flag;
	char	**cmds;
	char	buffer[ARG_MAX];

	// printf("**str: %s\n", str);
	ft_memset(buffer, 0, ARG_MAX); // 버퍼 초기화
	while (is_blank(*str))
		str++;
	quote = CLOSED;
	q_flag = FALSE;
	len = 0;
	while (*str)
	{
		if (quote == CLOSED && (*str == '\'' || *str == '\"'))
		{
			quote = *str;
			q_flag = TRUE;
		}
		else if (quote && *str == quote)
		{
			quote = CLOSED;
		}
		else if (quote == CLOSED && (*str == '<' || *str == '>'))
		{
			// 리다이렉션 있으면 파일리스트로 분리, str 주소 넘겨줌
			check_redir(&str, token);
			// printf("str:%s, buf : %s, buf_len: %d\n", str, buffer, len);
		}
		else if ((quote == CLOSED && *str == '$') || (quote == '\"' && *str == '$'))
		{
			// $ 뒤가 알파벳이나 '_'일 때만 환경변수로 처리(변수 명 조건)
			if (ft_isalpha(*(str + 1)) || *(str + 1) == '_')
			{
				len += search_env(&str, &buffer[len], token->envp, quote); // $ 위치부터 보내주기
			}
			// TODO  $? => exit status 로 치환
			// else if (*(str + 1) == '?')
			// {
			// }
			else
			{
				// 변수명 조건 벗어나면 $ 뒤 한글자 무시
				str++;
			}
			if (quote)
				quote = CLOSED; // 따옴표 닫아줌 (환경변수 치환되면서 따옴표 제거됨)
		}
		else if (!quote && is_blank(*str))
		{
			buffer[len++] = BLANK; // 따옴표 밖 공백이면 자름
		}
		else
		{
			buffer[len++] = *str;
		}
		if (*str == '\0')
			break ;
		str++;
	}
	buffer[len] = '\0';
	// printf("============\n");
	// printf("environ expansion result : %s\n", buffer);
	// 버퍼에 환경변수 모두 치환된 결과 담김, 공백으로 스플릿해서 리턴.
	cmds = ft_split(buffer, BLANK);
	if (q_flag == FALSE && cmds[0][0] == '\0')
	{
		free_strs(cmds);
		cmds = NULL;
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
	//print_filelst(cmd_deque);
	free_strs(strs);
	// print_deque(cmd_deque);
}
