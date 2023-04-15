/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_main.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyobicho <hyobicho@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/15 14:12:04 by hyobicho          #+#    #+#             */
/*   Updated: 2023/04/15 21:31:52 by hyobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	quote_check(char *str)
{
	char	quote;
	int		i;

	i = 0;
	quote = CLOSED;
	while (str[i])
	{
		if (quote && quote == str[i])
			quote = CLOSED;
		else if (quote == CLOSED && (str[i] == '\'' || str[i] == '\"'))
			quote = str[i];
		i++;
	}
	if (quote)
		ft_error();
}

void	empty_check(char *str)
{
	char	flag;
	int		i;

	i = 0;
	flag = CLOSED;
	while (str[i])
	{
		// 공백 문자 무시
		if (str[i] == ' ')
			i++;
		if (flag && (str[i] == '<' ||  str[i] == '>' || str[i] == '|' || str[i] == '#' || str[i] == '&'))
			ft_error();
		if (flag && str[i] != '<' &&  str[i] != '>' && str[i] != '|')
		{
			flag = CLOSED;
			// escape 문자 무시
			if (str[i] == '\\')
				i++;
		}
		else if (!flag && (str[i] == '<' || str[i] == '>' || str[i] == '|'))
		{
			flag = str[i];
			if (flag != '|' && str[i + 1] == flag)
				i++;
		}
		// printf("str[%d]: %c, flag: %c\n", i, str[i], flag);
		i++;
	}
	if (flag)
		ft_error();
}


void	syntax_check(char *str)
{
	quote_check(str);
	empty_check(str);
}



void	exec_builtin(t_token *token)
{
	// 디렉토리 최대 길이(나중에 제대로 확인)
	char	cwd_name[256];
	// printf("token->command: %s\n", token->command);
	if (ft_strcmp(token->command, "pwd") == 0)
		getcwd(cwd_name, sizeof(cwd_name));
	printf("%s\n", cwd_name);
}

void	make_cmdlst(char *str)
{
	char		**strs;
	char		**token;
	t_token		*new;
	int			i;

	strs = ft_split(str, '|');
	i = 0;
	while (strs[i])
	{
		// printf("%s\n", strs[i]);
		// 다시 공백으로 스플릿! [0] : cmd [1]: echo면 옵션 아니면 others,
		token = ft_split(strs[i], ' ');
		new = malloc(sizeof(t_token));
		if(!new)
			ft_error();
		init_element(new, token);
		exec_builtin(new);
		// make_comdeque(token);
		i++;
	}
}

int	main(void)
{
	char	*str;
	// t_deque	cmd_deque;

	// init_deque(&cmd_deque);
	while (1)
	{
		str = readline("minishell-0.0$ ");
		if (!str)
			break;
		syntax_check(str);
		// printf("%s\n", str);
		// 받은 문자열 | 로 스플릿 echo -n sdfjlskdjf | dsfcajkkle fdsfklj 
		make_cmdlst(str);
		add_history(str);
		free(str);
	}
	return (0);
}