/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_check.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyobicho <hyobicho@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 05:41:25 by hyobicho          #+#    #+#             */
/*   Updated: 2023/05/05 16:50:00 by hyobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_envkey(const char *s1, const char *s2, int *key_len)
{
	int		i;
	char	*str;
	char	*key;

	str = (char *)s1;
	key = (char *)s2;
	i = 0;
	while ((str[i] == '_' || ft_isalnum(str[i])) || key[i])
	{
		if (str[i] != key[i])
		{
			while (str[i] == '_' || ft_isalnum(str[i]))
				i++;
			*key_len = i;
			return (0);
		}
		i++;
	}
	*key_len = i;
	return (1);
}

static void	check_blank(char *buf)
{
	int		i;
	char	quote;

	i = 0;
	quote = CLOSED;
	while (buf[i])
	{
		if (quote == CLOSED && (buf[i] == '\'' || buf[i] == '\"'))
		{
			quote = buf[i];
		}
		else if (quote && buf[i] == quote)
			quote = CLOSED;
		else if (quote == CLOSED && is_blank(buf[i]))
			buf[i] = BLANK;
		i++;
	}
}

int	env_trans(char *str, t_edeque *envp, char *buf, int quote)
{
	t_env	*tmp;
	int		key_len;

	key_len = 0;
	tmp = envp->head;
	while (tmp)
	{
		if (is_envkey(str, tmp->key, &key_len))
		{
			ft_memcpy(buf, tmp->val, ft_strlen(tmp->val));
			if (!quote)
				check_blank(buf);
			return (key_len);
		}
		tmp = tmp->next;
	}
	return (key_len);
}

int	search_env(char **str, char *buf, t_edeque *envp, int quote)
{
	int	i;
	int	key_len;

	if (!quote)
		*str += env_trans(*str + 1, envp, &buf[0], quote);
	else
	{
		i = 0;
		while (**str != quote)
		{
			if (**str == '$')
			{
				key_len = env_trans(*str + 1, envp, &buf[i], quote);
				i = ft_strlen(buf);
			}
			else
				buf[i++] = **str;
			*str += 1;
		}
	}
	return (ft_strlen(buf));
}
