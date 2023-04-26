/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_lst.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyobicho <hyobicho@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 15:08:06 by hyobicho          #+#    #+#             */
/*   Updated: 2023/04/26 16:47:18 by hyobicho         ###   ########.fr       */
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

int	get_filename(char *str, t_fdata *new, t_token *token)
{
	int		quote;
	int		i;
	int		len;

	i = 0;
	while (is_blank(str[i]))
		i++;
	len = 0;
	quote = 0;
	while (str[i])
	{
		if (!quote && (is_blank(str[i]) || str[i] == '<' || str[i] == '>'))
			break;
		if (!quote && (str[i] == '\'' || str[i] == '\"'))
		{
			quote = str[i];
		}
		else if (quote && str[i] == quote)
		{
			quote = 0;
		}
		else if ((!quote && str[i] == '$') || (quote == '\"' && str[i] == '$'))
		{
			len += env_trans(&str[i + 1], &i, &new->filename[len], token->envp);
		}
		else
		{
			new->filename[len++] = str[i];
		}
		i++;
	}
	new->filename[len] = '\0';
	// printf("string idx: %d filename: %s\n", i, new->filename);
	append_file(&token->files, new);
	if (new->type == DELIMITER || new->type == APPEND)
		i++;	// printf("files addr: %p, next: \n", token->files);
	return (i);
}

// delete!
void	print_filelst(t_fdata *head)
{
	t_fdata *tmp = head;
	while (tmp)
	{
		printf("filename: %s, type: %d\n", tmp->filename, tmp->type);
		tmp = tmp->next;
	}
}