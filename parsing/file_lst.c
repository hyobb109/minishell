/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_lst.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyobicho <hyobicho@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 15:08:06 by hyobicho          #+#    #+#             */
/*   Updated: 2023/05/09 15:33:43 by hyobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_files(t_fdata **lst)
{
	t_fdata	*tmp;

	while (*lst)
	{
		tmp = (*lst)->next;
		free(*lst);
		*lst = tmp;
	}
}

void	append_file(t_fdata **head, t_fdata *new)
{
	t_fdata	*last;

	if (!(*head))
	{
		*head = new;
		return ;
	}
	last = *head;
	while (last->next)
		last = last->next;
	last->next = new;
}

void	init_vars(t_vars *v, char *buf, int file)
{
	if (file)
		ft_memset(buf, 0, PATH_MAX);
	else
		ft_memset(buf, 0, ARG_MAX);
	v->flag = file;
	v->i = 0;
	v->len = 0;
	v->quote = CLOSED;
}

int	is_heredoc(int type)
{
	if (type == LIMITER || type == Q_LIMITER)
		return (TRUE);
	return (FALSE);
}

int	is_environ(char quote, char c)
{
	if (c == '$' && quote == CLOSED)
		return (TRUE);
	if (c == '$' && quote == '\"')
		return (TRUE);
	return (FALSE);
}

char	*check_file_environ(t_vars *v, t_token *token, char **str, char *filename)
{
	if (ft_isalpha(*(*str + 1)) || *(*str + 1) == '_')
	{
		v->len += search_env(str, filename, token->envp, *v);
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

void	get_filename(char **str, t_fdata *new, t_token *token)
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
		else if (!is_heredoc(new->type) && is_environ(v.quote, **str))
			*str = check_file_environ(&v, token, str, &new->filename[v.len]);
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

// delete!
void	print_filelst(t_deque *cmd_lst)
{
	t_token *tmp = cmd_lst->head;

	for (int i = 0; i < cmd_lst->cnt; i++)
	{
		t_fdata *files = tmp->files;
		while (files)
		{
			printf("filename: %s, type: %d\n", files->filename, files->type);
			files = files->next;
		}
		tmp = tmp->next;
	}
}
