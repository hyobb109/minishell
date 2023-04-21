/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyobicho <hyobicho@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 15:15:26 by yunjcho           #+#    #+#             */
/*   Updated: 2023/04/21 23:04:11 by hyobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// cmd 와 arg로 분리
static void	init_token(char *str, t_token *token, char **env)
{
	token->command = parse_command(str, env); // 우주님 파싱 가져옴
	//TODO - infile/outfile
	token->state = 0;
	token->infile = NULL;
	token->outfile = NULL;
	token->prev = NULL;
	token->next = NULL;
}

void	make_cmdlst(char *str, t_deque *cmd_deque, char **env)
{
	char		**strs;
	t_token		*token;
	int			i;
	
	i = 0;
	// quotes 안에 | 따로 처리 필요
	strs = ft_pipe_split(str);
	while (strs[i])
	{
		// printf("str[%d] : %s\n", i, strs[i]);
		token = malloc(sizeof(t_token));
		if(!token)
			ft_error();
		init_token(strs[i], token, env);
		append_back(cmd_deque, token);
		i++;
	}
	// free_strs(parsed);
	// free_strs(strs);
	print_deque(cmd_deque);
}
