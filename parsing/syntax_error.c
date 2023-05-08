/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunjcho <yunjcho@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 15:16:54 by yunjcho           #+#    #+#             */
/*   Updated: 2023/05/05 22:26:46 by yunjcho          ###   ########.fr       */
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

int	is_redir_pipe(char c)
{
	if (c == '<' || c == '>' || c == '|')
		return (1);
	return (0);
}

int	print_syntax_error(int ver, t_syntax s)
{
	g_exit_status = 258 * 256;
	printf("minishell: syntax error\n");
	if (ver == 1)
	{
		printf("here, g_:%d \n", g_exit_status);
		return (TRUE);
	}
	if (ver == 2)
	{
		printf("*tmp: %s\n", s.tmp);
		if (s.heredoc == TRUE && !(*(s.tmp) == '<' && *(s.tmp + 1) == '<'))
		{
			ft_memblank(s.tmp);
			return (FALSE);
		}
	}
	if (ver == 3)
	{
		if (s.heredoc == TRUE)
		{
			ft_memblank(s.tmp);
			return (FALSE);
		}
	}
	return (TRUE);
}

int	check_flag(t_syntax *s, char *str)
{
	s->flag = *str;
	s->tmp = str;
	if (s->flag != '|' && *(str + 1) == s->flag)
	{
		if (s->flag == '<')
			s->heredoc = TRUE;
		return (1);
	}
	return (0);
}

int	empty_error(char *str)
{
	t_syntax	s;

	s.i = 0;
	s.flag = 0;
	s.heredoc = FALSE;
	while (is_blank(str[s.i]))
		s.i++;
	if (str[s.i] == '|')
		return (print_syntax_error(1, s));
	while (str[s.i])
	{
		if (s.flag == '|' && (str[s.i] == '<' || str[s.i] == '>'))
			s.i += check_flag(&s, str);
		else if (s.flag && is_redir_pipe(str[s.i]))
			return (print_syntax_error(2, s));
		if (s.flag && !is_redir_pipe(str[s.i]) && !is_blank(str[s.i]))
			s.flag = 0;
		else if (!s.flag && is_redir_pipe(str[s.i]))
			s.i += check_flag(&s, str);
		s.i++;
	}
	if (s.flag)
		return (print_syntax_error(3, s));
	return (FALSE);
}

int	syntax_error(char *str)
{
	if (quote_error(str))
	{
		g_exit_status = 258 * 256;
		return (TRUE);
	}
	if (empty_error(str))
	{
		g_exit_status = 258 * 256;
		return (TRUE);
	}
	return (FALSE);
}
