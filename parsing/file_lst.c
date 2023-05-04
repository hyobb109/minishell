/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_lst.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyobicho <hyobicho@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 15:08:06 by hyobicho          #+#    #+#             */
/*   Updated: 2023/05/04 13:23:42 by hyobicho         ###   ########.fr       */
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

// 토큰 파일리스트에 파일 이름 복사해서 담음
void	get_filename(char **str, t_fdata *new, t_token *token)
{
	int		quote;
	int		i;
	int		len;

	i = 0;
	while (is_blank(**str))
		*str += 1;
	len = 0;
	quote = 0;
	while (**str)
	{
		if (quote == CLOSED && (is_blank(**str) || **str == '<' || **str == '>'))
			break;
		if (quote == CLOSED && (**str == '\'' || **str == '\"'))
		{
			quote = **str;
			if (new->type == LIMITER)
				new->type = Q_LIMITER; // heredoc 에서 리미터에 따옴표 있으면 환경변수 치환 안하므로 따로 분류
		}
		else if (quote && **str == quote)
		{
			quote = CLOSED;
		}
		else if ((new->type != LIMITER && new->type != Q_LIMITER) && ((quote == CLOSED && **str == '$') || (quote == '\"' && **str == '$')))
		{
			if (ft_isalpha(*(*str + 1)) || *(*str + 1) == '_')
			{
				len += search_env(str, &new->filename[len], token->envp, quote); // $ 위치부터 보내주기
			}
			else
			{
				*str += 1;
			}
			// printf("str: %s, buf[%d]: %s\n", str, len, &buffer[len]);
			// printf("quote: %c\n", quote);
			if (quote)
				quote = CLOSED; // 따옴표 닫아줌 (환경변수 치환되면서 따옴표 제거됨)
		}
		else
		{
			new->filename[len++] = **str;
		}
		*str += 1;
		// printf("====file name test====\n");
		// printf("*str: %s, file_buf: %s\n", *str, new->filename);
	}
	new->filename[len] = '\0';

	// printf("file: %s, \n", new->filename);
	// printf("*str: %s\n", *str);
	append_file(&token->files, new);
	// if (new->type == LIMITER || new->type == Q_LIMITER || new->type == APPEND)
	// 	*str += 1; // printf("files addr: %p, next: \n", token->files);
}

// redirection이 있는 토큰은 따로 처리.. 여기서도 따옴표 확인 계속 해야 함
//<, <<, > , >> 인지 체크, infile, outfile 토큰 파일리스트에 업데이트
void	check_redir(char **str, t_token *token)
{
	t_fdata *newfile;

	newfile = malloc(sizeof(t_fdata));
	if (!newfile)
		ft_error();
	ft_memset(newfile->filename, 0, PATH_MAX);
	newfile->next = NULL;
	// redirection flag check => << 와 >> 는 인덱스 2개씩 늘려줌
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
	// printf("===check redirection===\n");
	// printf("*str: %s\n", *str);
	// redirection 기호 다음 위치부터 보냄
	return (get_filename(str, newfile, token));
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
