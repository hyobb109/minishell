/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyobicho <hyobicho@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 15:15:26 by yunjcho           #+#    #+#             */
/*   Updated: 2023/04/23 21:16:40 by hyobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_blank(char c)
{
	if (c == ' ' || c == '\t' || c == '\n')
		return (1);
	return (0);
}

static char	**parse_command(char *str, char **env)
{
	int		i;
	int		len;
	int		quote;
	char	res[ARG_MAX];
	char	charset[1];

	(void)env;
	charset[0] = -1;
	// printf("str: %s\n", str);
	ft_memset(res, 0, ARG_MAX); // 버퍼 초기화
	i = 0;
	// 처음 들어오는 공백 넘김
	while (is_blank(str[i]))
		i++;
	quote = 0;
	len = 0;
	while (str[i])
	{
		if (!quote && (str[i] == '\'' || str[i] == '\"'))
		{
			quote = str[i];
		}
		else if (quote && str[i] == quote)
		{
			quote = 0;
		}
		else if ((!quote && str[i] == '$') || (quote == '\"' && str[i] == '$')) // $ 나오면 환경변수 아닌 것 까지 보고 자름
		{
			i++;
			// env 쭉 보면서 환경변수 있는지 확인
			check_env(&str[i], env);
		}
		else if (!quote && is_blank(str[i]))
		{
			res[len++] = -1; // 따옴표 밖 공백이면 자름
			//break;
		}
		else
		{
			res[len++] = str[i];
		}
		// printf("str[%d]: %c res: %s quote: %d\n", i, str[i], res, quote);
		i++;
	}
	res[len] = '\0';
	// printf("res : %s\n", res);

	return (ft_split(res, charset));
}

// cmd 와 arg로 분리
static void	init_token(char *str, t_token *token, char **env)
{
	token->command = parse_command(str, env);
	//TODO - infile/outfile
	token->state = 0;
	token->infile = NULL;
	token->outfile = NULL;
	token->prev = NULL;
	token->next = NULL;
	token->env = env;
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
