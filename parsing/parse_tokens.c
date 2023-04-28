/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyobicho <hyobicho@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 15:15:26 by yunjcho           #+#    #+#             */
/*   Updated: 2023/04/28 22:24:24 by hyobicho         ###   ########.fr       */
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
	if (str[i] == '<' && str[i + 1] == '<')
	{
		newfile->type = DELIMITER;
		i += 2;
	}
	else if (str[i] == '>' && str[i + 1] == '>')
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
		// echo hi >> "$USER hi $LANG" <$USER >>append1 >>ap'en'd2 <<"heredoc" <<h_noquote <"infile $USER" <infileeeeeeee
		// <<h_noquote <"infile $USER" <infileeeeeeee 부터 안됨!!!??
		else if (!quote && (str[i] == '<' || str[i] == '>')) // 리다이렉션 있으면 io_here 토큰으로 분리하여 담음
		{
			// printf("*str: %s\n", &str[i]);
			i += io_here_token(&str[i], token);
		} 
		else if (!quote && is_blank(str[i]))
		{
			res[len++] = BLANK; // 따옴표 밖 공백이면 자름
		}
		else
		{
			res[len++] = str[i];
		}
		i++;
		// printf("str[%d]: %c res: %s quote: %d\n", i, str[i], res, quote);
	}
	res[len] = '\0';
	// printf("res : %s\n", res);
	token->command = ft_split(res, BLANK);
	if (is_builtin(token->command[0]))
		token->state = BUILTIN;
}

char	*expand_environ(char *str, t_token *token, int quote)
{
	//	어차피 끝까지 볼거임
	int		len;
	char	buffer[ARG_MAX];

	// printf("**str: %s\n", str);
	ft_memset(buffer, 0, ARG_MAX); // 버퍼 초기화
	quote = CLOSED;
	len = 0;
	while (*str)
	{
		if (quote == CLOSED && (*str == '\'' || *str == '\"'))
		{
			quote = *str;
		}
		else if (quote && *str == quote)
		{
			quote = CLOSED;
		}
		else if (quote == CLOSED && *str == '<' && *(str + 1) == '<')
		{
			buffer[len++] = *str;
			str++;
			buffer[len++] = *str;
			str++;
			while (!is_blank(*str) && *str != '<' && *str != '>' && *str)
			{
				buffer[len++] = *str;
				if (*str == '\0')
					break ;
				str++;
			}
		}
		// $esd  뒤가 알파벳이나 '_'일 때만 환경변수로 처리(변수 명 조건)
		else if ((((quote == CLOSED && *str == '$') || (quote == '\"' && *str == '$')) && (ft_isalpha(*(str + 1)) || *(str + 1) == '_')))
		{
			// len = buff_idx + 1;
			// printf("str: %s, buf[%d]: %s\n", str, len, &buffer[len]);
			len += search_env(&str, &buffer[len], token->envp, quote); // $위치부터 보내주기.
			// str = q_flag;
		}
		buffer[len++] = *str;
		if (*str == '\0')
			break ;
		str++;
		// printf("str: %s, buf : %s, buf_len: %d\n", str, buffer, len);
	}
	// printf("============\n");
	printf("environ expansion result : %s\n", buffer);
	// 버퍼에 환경변수 모두 치환된 결과 담김.
	// 메모리 새로 할당하여 리턴.
	return (ft_strdup(buffer));
	// printf("res : %s\n", res);
}

// cmd 와 arg로 분리
static void	init_token(char *str, t_token *token, t_edeque *envp)
{
	token->envp = envp;
	//TODO - infile/outfile
	token->state = GENERAL;
	token->files = NULL;
	parse_command(expand_environ(str, token, CLOSED), token);
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
	print_filelst(cmd_deque->head->files);
	free_strs(strs);
	print_deque(cmd_deque);
}

