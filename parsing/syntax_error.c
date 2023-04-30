/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunjcho <yunjcho@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 15:16:54 by yunjcho           #+#    #+#             */
/*   Updated: 2023/04/30 20:49:29 by yunjcho          ###   ########.fr       */
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
		return (0);
	printf("minishell: syntax error near unexpected token\n");
	return (1);
}

int	empty_error(char *str)
{
	char	flag;
	int		i;

	i = 0;
	flag = 0;
	while (is_blank(str[i]))
		i++;
	// 파이프로 시작하는 건 heredoc 있어도 무조건 신택스에러
	if (str[i] == '|')
	{
		printf("minishell: syntax error near unexpected token\n");
		return (1);
	}
	// 히어독이 신택스에러보다 앞에 나오면 히어독 실행되어야 함
	while (str[i])
	{
		if (flag == '|' && (str[i] == '<' || str[i] == '>'))
			i++;
		else if (flag && (str[i] == '<' ||  str[i] == '>' || str[i] == '|'))
		{
			printf("minishell: syntax error near unexpected token\n");	
			return (1);
		}
		if (flag && str[i] != '<' &&  str[i] != '>' && str[i] != '|' && !is_blank(str[i]))
		{
			flag = 0;
			// escape 문자 무시 -> 안해도 됨
			// if (str[i] == '\\')
			// 	i++;
		}
		else if (!flag && (str[i] == '<' || str[i] == '>' || str[i] == '|'))
		{
			flag = str[i];
			if (flag != '|' && str[i + 1] == flag)
				i++;
		}
		// printf("str[%d]: %c, flag: %c\n", i, str[i], flag);
		i++;
	}
	if (flag)
	{
		printf("minishell: syntax error near unexpected token\n");	
		return (1);
	}
	return (0);
}

// 에러코드 258
int	syntax_error(char *str)
{
	// 닫히지 않는 따옴표
	if (quote_error(str))
		return (1);
	// TODO
	// heredoc 있는 것 처리 다시 -> exit하지 않고 에러메시지만 띄움
	// 비어있는 리다이렉션, 파이프  => 다시 해야함!!
	if (empty_error(str))
		return (1);
	return (0);
}
