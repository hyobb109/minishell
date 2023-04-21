/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyobicho <hyobicho@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 15:15:26 by yunjcho           #+#    #+#             */
/*   Updated: 2023/04/21 19:47:03 by hyobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	parse_token(char *str, t_token *token)
{
	int		i;
	int		len;
	char	parsed[3][ARG_MAX];
	int		flag;

	// 처음 들어오는 공백 넘김
	i = 0;
	while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n')
		i++;
	// command -> 공백 전까지 입력받으니까 따옴표 닫혀 있고 공백이면 break
	flag = 0;
	len = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '\"')
		{
			flag = str[i];
		}
		else if (flag && str[i] == flag)
		{
			flag = 0;
		}
		else
		{
			parsed[0][len++] = str[i];
		}
		i++;
		if (str[i] == ' ' && flag == 0)
			break;
	}
	parsed[0][len] = '\0';
	while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n')
		i++;
	// 1. 따옴표 오고서 -n옵션
	// 2. 따옴표 없이 -n
	// argument, option 같이?
	// option -> echo일 때만 필요! -nnn 인지만 확인하면 됨(일단 arg로 담고 나중에 체크)
	flag = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '\"')
		{
			flag = str[i];
		}
		else if (flag && str[i] == flag)
		{
			flag = 0;
		}
		else if (flag == str[i])
		{
			i++;
		}
		i++;
	}
	token->command = parsed[0];
}

void	make_cmdlst(char *str, t_deque *cmd_deque, char **env)
{
	char		**strs;
	t_token		*token;
	int			i;
	
	(void)env;
	i = 0;
	// quotes 안에 | 따로 처리 필요
	strs = ft_pipe_split(str);
	while (strs[i])
	{
		// printf("str[%d] : %s\n", i, strs[i]);
		token = malloc(sizeof(t_token));
		if(!token)
			ft_error();
		parse_token(strs[i], token);
		append_back(cmd_deque, token);
		i++;
	}
	// free_strs(parsed);
	// free_strs(strs);
	// print_deque(cmd_deque);
}