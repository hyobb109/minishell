/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunwoju <hyunwoju@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 15:37:28 by hyobicho          #+#    #+#             */
/*   Updated: 2023/05/09 20:28:07 by hyunwoju         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*check_cmd_env(t_vars *v, t_token *token, char *str, char *buf)
{
	char	*status;
	int		num_len;

	if (ft_isalpha(*(str + 1)) || *(str + 1) == '_')
		v->len += search_env(&str, buf, token->envp, *v);
	else if (*(str + 1) == '?')
	{
		status = ft_itoa(WEXITSTATUS(g_exit_status));
		num_len = ft_strlen(status);
		ft_memcpy(buf, status, num_len);
		free(status);
		v->len += num_len;
		str++;
		if (v->quote)
			str++;
	}
	else
		str++;
	if (v->quote)
		v->quote = CLOSED;
	return (str);
}

static void	is_empty_str(t_vars *v, char *str, char *buffer)
{
	if (*(str - 1) == v->quote)
		buffer[v->len++] = EMPTY;
	v->quote = CLOSED;
}

char	**parse_command(char *str, t_token *token, t_vars *v, char *buffer)
{
	while (*str)
	{
		if (!v->quote && (*str == '\'' || *str == '\"'))
			v->quote = *str;
		else if (v->quote && *str == v->quote)
			is_empty_str(v, str, buffer);
		else if (!v->quote && (*str == '<' || *str == '>'))
			check_redir(&str, token);
		else if (is_environ(v->quote, *str))
			str = check_cmd_env(v, token, str, &buffer[v->len]);
		else if (!v->quote && is_blank(*str))
			buffer[v->len++] = BLANK;
		else
			buffer[v->len++] = *str;
		if (*str == '\0')
			break ;
		str++;
	}
	buffer[v->len] = '\0';
	if (!buffer[0])
		return (NULL);
	return (ft_split(buffer, BLANK));
}
