/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_lst.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyobicho <hyobicho@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 15:08:06 by hyobicho          #+#    #+#             */
/*   Updated: 2023/04/30 19:04:00 by hyobicho         ###   ########.fr       */
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

// 토큰 파일리스트에 파일 이름 복사해서 담고 증가시킨 str 인덱스 리턴
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
			if (new->type == DELIMITER)
				new->type = Q_DELIMITER; // heredoc 에서 리미터에 따옴표 있으면 환경변수 치환 안하므로 따로 분류
		}
		else if (quote && str[i] == quote)
		{
			quote = 0;
		}
		else
		{
			new->filename[len++] = str[i];
		}
		i++;
	}
	new->filename[len] = '\0';
	// printf("=========\n");
	// printf("file: %s\n", new->filename);
	append_file(&token->files, new);
	if (new->type == DELIMITER || new->type == Q_DELIMITER || new->type == APPEND)
		i++; // printf("files addr: %p, next: \n", token->files);
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
