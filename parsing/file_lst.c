/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_lst.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunwoju <hyunwoju@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 15:08:06 by hyobicho          #+#    #+#             */
/*   Updated: 2023/05/09 19:30:27 by hyunwoju         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_heredoc(t_fdata *file)
{
	if (file)
	{
		if (file->type == LIMITER || file->type == Q_LIMITER)
			return (TRUE);
	}
	return (FALSE);
}

static char	*check_file_env(t_vars *v, t_token *token, char **str, char *file)
{
	if (ft_isalpha(*(*str + 1)) || *(*str + 1) == '_')
	{
		v->len += search_env(str, file, token->envp, *v);
	}
	else
	{
		*str += 1;
	}
	printf("*str: %s\n", *str);
	if (v->quote)
		v->quote = CLOSED;
	return (*str);
}

static void	get_filename(char **str, t_fdata *new, t_token *token)
{
	t_vars	v;

	init_vars(&v, new->filename, TRUE);
	while (is_blank(**str))
		*str += 1;
	while (**str)
	{
		if (!v.quote && (is_blank(**str) || **str == '<' || **str == '>'))
			break ;
		if (!v.quote && (**str == '\'' || **str == '\"'))
		{
			v.quote = **str;
			if (new->type == LIMITER)
				new->type = Q_LIMITER;
		}
		else if (v.quote && **str == v.quote)
			v.quote = CLOSED;
		else if (!is_heredoc(new) && is_environ(v.quote, **str))
			*str = check_file_env(&v, token, str, &new->filename[v.len]);
		else
			new->filename[v.len++] = **str;
		*str += 1;
	}
	new->filename[v.len] = '\0';
	append_file(&token->files, new);
}

void	check_redir(char **str, t_token *token)
{
	t_fdata	*newfile;

	newfile = malloc(sizeof(t_fdata));
	if (!newfile)
		ft_error();
	newfile->next = NULL;
	if (**str == '<' && *(*str + 1) == '<')
	{
		newfile->type = LIMITER;
		*str += 1;
	}
	else if (**str == '>' && *(*str + 1) == '>')
	{
		newfile->type = APPEND;
		*str += 1;
	}
	else if (**str == '<')
		newfile->type = INFILE;
	else if (**str == '>')
		newfile->type = OUTFILE;
	*str += 1;
	get_filename(str, newfile, token);
}
