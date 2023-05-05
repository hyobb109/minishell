/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyobicho <hyobicho@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 15:16:54 by yunjcho           #+#    #+#             */
/*   Updated: 2023/05/05 20:09:07 by hyobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	quote_error(char *str)
{
	char	quote;
	int		i;

	i = 0;
	quote = CLOSED;
	while (str[i])
	{
		if (quote && quote == str[i])
			quote = CLOSED;
		else if (quote == CLOSED && (str[i] == '\'' || str[i] == '\"'))
			quote = str[i];
		i++;
	}
	if (!quote)
		return (FALSE);
	printf("minishell: syntax error\n");
	return (TRUE);
}

void	ft_memblank(char *str)
{
	while (*str)
	{
		*str = BLANK;
		str++;
	}
}

int	empty_error(char *str)
{
	char	flag;
	int		heredoc;
	int		i;
	char	*tmp;

	i = 0;
	flag = 0;
	heredoc = FALSE;
	while (is_blank(str[i]))
		i++;
	// 파이프로 시작하는 건 heredoc 있어도 무조건 신택스에러
	if (str[i] == '|')
	{
		g_exit_status = 258;
		printf("minishell: syntax error\n");
		return (TRUE);
	}
	// 히어독이 신택스에러보다 앞에 나오면 히어독 실행되어야 함 
	// => 이 경우엔 FALSE리턴하고, 신택스 에러 부분 널로 치환
	while (str[i])
	{
		// |<infile, |>outfile 등은 정상적인 입력이므로 인덱스 넘겨주고 플래그 갱신
		if (flag == '|' && (str[i] == '<' || str[i] == '>'))
		{
			flag = str[i];
			tmp = &str[i];
			if (flag != '|' && str[i + 1] == flag)
			{
				if (flag == '<')
					heredoc = TRUE;
				i++;
			}
		}
		else if (flag && (str[i] == '<' ||  str[i] == '>' || str[i] == '|'))
		{
			g_exit_status = 258;
			printf("minishell: syntax error\n");
			// 앞에 히어독이 있었으면 뒤에 끊어줌
			if (heredoc == TRUE && !(*tmp == '<' && *(tmp + 1) == '<'))
			{
				ft_memblank(tmp);
				// *tmp = '\0';
				// printf("str: %s, flag: %c\n", str, flag);
				return (FALSE);
			}
			return (TRUE);
		}
		// 이전에 |<>를 만났었고, 지금 글자가 |<>가 아니고 공백도 아니면 순수한 문자여서 플래그 내림
		if (flag && str[i] != '<' &&  str[i] != '>' && str[i] != '|' && !is_blank(str[i]))
		{
			flag = 0;
		}
		//처음 만난 |<> ->플래그에 저장, 특히 히어독이면 히어독플래그 표시
		else if (!flag && (str[i] == '<' || str[i] == '>' || str[i] == '|'))
		{
			flag = str[i];
			tmp = &str[i];
			if (flag != '|' && str[i + 1] == flag)
			{
				if (flag == '<')
					heredoc = TRUE;
				i++;
			}
		}
		i++;
		// printf("str[%d]: %c, flag: %c, heredoc: %d\n", i, str[i], flag, heredoc);
	}
	// 문자열 끝났는데 플래그가 세워져있으면 에러
	if (flag)
	{
		g_exit_status = 258;
		printf("minishell: syntax error\n");
		if (heredoc == TRUE)
		{
			ft_memblank(tmp);
			// *tmp = '\0';
			return (FALSE);
		}
		return (TRUE);
	}
	return (FALSE);
}

// 에러코드 258
int	syntax_error(char *str)
{
	// 닫히지 않는 따옴표
	if (quote_error(str))
	{
		g_exit_status = 258;
		return (TRUE);
	}
	// TODO
	// heredoc 있는 것 처리 다시 -> exit하지 않고 에러메시지만 띄움
	// 비어있는 리다이렉션, 파이프  => 다시 해야함!!
	if (empty_error(str))
	{
		g_exit_status = 258;
		return (TRUE);
	}
	return (FALSE);
}


/*
empty error 인 경우 
=>  앞에서부터 보다가 신택스 에러가 먼저 나오면 continue, 
	히어독이 신택스 에러보다 먼저 나오면 히어독만 실행 : 히어독 토큰만 만들어서 실행부에 보냄
1. 빈 파이프 => 파이프 뒤에 글자 없이 공백만 있을 때
	dfkelfjw |     |  dsgad
2. 리다이렉션 기호 뒤에 글자 없이 공백만 올 때

*/