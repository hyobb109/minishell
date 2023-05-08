/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_lst.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyobicho <hyobicho@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 15:08:06 by hyobicho          #+#    #+#             */
/*   Updated: 2023/05/08 17:16:52 by hyobicho         ###   ########.fr       */
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

void	get_filename(char **str, t_fdata *new, t_token *token)
{
	int		quote;
	int		len;
	char	*tmp;

	while (is_blank(**str))
		*str += 1;
	len = 0;
	quote = 0;
	while (**str)
	{
		if (!quote && (is_blank(**str) || **str == '<' || **str == '>'))
			break ;
		if (!quote && (**str == '\'' || **str == '\"'))
		{
			quote = **str;
			if (new->type == LIMITER)
				new->type = Q_LIMITER;
		}
		else if (quote && **str == quote)
			quote = CLOSED;
		else if ((new->type != LIMITER && new->type != Q_LIMITER) && \
				((!quote && **str == '$') || (quote == '\"' && **str == '$')))
		{
			if (ft_isalpha(*(*str + 1)) || *(*str + 1) == '_')
				len += search_env(str, &new->filename[len], token->envp, quote);
			else
				*str += 1;
			if (quote)
				quote = CLOSED;
		}
		else
			new->filename[len++] = **str;
		*str += 1;
	}
	new->filename[len] = '\0';
	tmp = ft_strchr(new->filename, BLANK);
	while (tmp)
	{
		*tmp = ' ';
		tmp = ft_strchr(tmp, BLANK);
	}
	append_file(&token->files, new);
}

void	check_redir(char **str, t_token *token)
{
	t_fdata	*newfile;

	newfile = malloc(sizeof(t_fdata));
	if (!newfile)
		ft_error();
	ft_memset(newfile->filename, 0, PATH_MAX);
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
