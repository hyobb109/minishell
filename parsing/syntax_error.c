/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyobicho <hyobicho@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 15:16:54 by yunjcho           #+#    #+#             */
/*   Updated: 2023/04/27 20:43:35 by hyobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	quote_check(char *str)
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
	if (quote)
		ft_error();
}

void	empty_check(char *str)
{
	char	flag;
	int		i;

	i = 0;
	flag = 0;
	while (is_blank(str[i]))
		i++;
	if (str[i] == '|') // 파이프로 시작하는 건 heredoc 있어도 무조건 신택스에러
		ft_error();
	while (str[i])
	{
		if (flag == '|' && (str[i] == '<' || str[i] == '>'))
			i++;
		else if (flag && (str[i] == '<' ||  str[i] == '>' || str[i] == '|'))
			ft_error();
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
		ft_error();
}

// 에러코드 258
void	syntax_check(char *str)
{
	// 닫히지 않는 따옴표
	quote_check(str);
	// TODO
	// heredoc 있는 것 처리 다시 -> exit하지 않고 에러메시지만 띄움
	// 비어있는 리다이렉션, 파이프  => 다시 해야함!!
	empty_check(str);
}
