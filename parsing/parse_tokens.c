/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyobicho <hyobicho@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 15:15:26 by yunjcho           #+#    #+#             */
/*   Updated: 2023/04/30 13:09:38 by hyobicho         ###   ########.fr       */
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
//<, <<, > , >> 인지 체크, infile, outfile 토큰 파일리스트에 업데이트
int	check_redir(char *str, t_token *token)
{
	int		i;
	t_fdata *newfile;

	i = 0;
	newfile = malloc(sizeof(t_fdata));
	if (!newfile)
		ft_error();
	newfile->next = NULL;
	// redirection flag check => << 와 >> 는 인덱스 2개씩 늘려줌
	if (str[i] == '<' && str[i + 1] == '<')
	{
		newfile->type = DELIMITER;
		i++;
	}
	else if (str[i] == '>' && str[i + 1] == '>')
	{
		newfile->type = APPEND;
		i++;
	}
	else if (str[i] == '<')
		newfile->type = INFILE;
	else if (str[i] == '>')
		newfile->type = OUTFILE;
	i++;
	return (get_filename(&str[i], newfile, token));
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
		// 히어독일 때는 환경변수 치환하면 안되므로 공백만나거나 널일 때까지 버퍼에 복사해줌
		else if (quote == CLOSED && *str == '<' && *(str + 1) == '<')
		{
			// 버퍼에 <<만 복사
			ft_memcpy(&buffer[len], str, 2);
			len += 2;
			str += 2;
			while (!is_blank(*str) && *str != '<' && *str != '>' && *str)
			{
				buffer[len++] = *str;
				str++;
			}
		}
		// $esd  뒤가 알파벳이나 '_'일 때만 환경변수로 처리(변수 명 조건)
		else if ((((quote == CLOSED && *str == '$') || (quote == '\"' && *str == '$')) && (ft_isalpha(*(str + 1)) || *(str + 1) == '_')))
		{
			// printf("str: %s, buf[%d]: %s\n", str, len, &buffer[len]);
			// printf("quote: %c\n", quote);
			len += search_env(&str, &buffer[len], token->envp, quote); // $ 위치부터 보내주기
			if (quote)
				quote = CLOSED; // 따옴표 닫아줌 (환경변수 치환되면서 따옴표 제거됨)
		}
		buffer[len++] = *str;
		if (*str == '\0')
			break ;
		str++;
		// printf("str: %s, buf : %s, buf_len: %d\n", str, buffer, len);
	}
	buffer[len] = '\0';
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
	print_filelst(cmd_deque->head->files);
	free_strs(strs);
	print_deque(cmd_deque);
}

