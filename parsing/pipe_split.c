/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_split.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seulee2 <seulee2@42seoul.student.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/20 15:55:17 by hyobicho          #+#    #+#             */
/*   Updated: 2023/04/24 19:58:22 by seulee2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	token_len(char *str)
{
	int		i;
	int		flag_quote;
	char	quote;

	flag_quote = 0;
	quote = 0;
	i = 0;
	while (str[i])
	{
		if (flag_quote == 0 && (str[i] == '\'' || str[i] == '\"'))
		{
			quote = str[i];
			flag_quote = 1;
		}
		else if (flag_quote == 1 && str[i] == quote)
			flag_quote = 0;
		if (flag_quote == 0 && str[i] == '|' && i > 0 && str[i - 1] != '\\' \
			&&str[i - 1] != '|')
			return (i);
		i++;
	}
	return (i);
}

static char	**result(char **res, char *str, int cnt)
{
	int	i;
	int	j;
	int	len;

	i = 0;
	while (i < cnt)
	{
		len = token_len(str);
		res[i] = (char *)malloc(sizeof(char) * (len + 1));
		if (res[i] == NULL)
			ft_error();
		j = 0;
		while (j < len)
		{
			res[i][j] = str[j];
			j++;
		}
		res[i][j] = '\0';
		if (str[j] == '|')
			j++;
		str += j;
		i++;
	}
	res[i] = 0;
	return (res);
}

static int	cnt_pipe(char *str)
{
	int		i;
	int		flag_quote;
	int		pipe;
	char	quote;

	pipe = 0;
	quote = 0;
	flag_quote = 0;
	i = 0;
	while (str[i])
	{
		if (flag_quote == 0 && (str[i] == '\'' || str[i] == '\"'))
		{
			quote = str[i];
			flag_quote = 1;
		}
		else if (flag_quote == 1 && str[i] == quote)
			flag_quote = 0;
		if (flag_quote == 0 && str[i] == '|' && i > 0 && str[i - 1] != '\\' \
			&&str[i - 1] != '|')
			pipe++;
		i++;
	}
	return (pipe);
}

char	**ft_pipe_split(char *str)
{
	char	**res;
	int		pipe_cnt;

	pipe_cnt = cnt_pipe(str);
	res = (char **)malloc(sizeof(char *) * (pipe_cnt + 2));
	if (res == NULL)
		ft_error();
	return (result(res, str, pipe_cnt + 1));
}
