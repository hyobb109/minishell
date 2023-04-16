/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_main.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunjcho <yunjcho@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/15 14:12:04 by hyobicho          #+#    #+#             */
/*   Updated: 2023/04/16 18:22:50 by yunjcho          ###   ########.fr       */
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



void	make_cmdlst(char *str, t_deque *cmd_deque)
{
	char		**strs;
	char		**parsed;
	t_token		*token;
	int			i;

	strs = ft_split(str, '|');
	i = 0;
	while (strs[i])
	{
		// printf("%s\n", strs[i]);
		// 다시 공백으로 스플릿! [0] : cmd [1]: echo면 옵션 아니면 others,
		parsed = ft_split(strs[i], ' ');
		token = malloc(sizeof(t_token));
		if(!token)
			ft_error();
		init_element(token, parsed);
		append_back(cmd_deque, token);
		exec_builtin(token); // 테스트용 위치
		i++;
	}
	free_strs(parsed);
	free_strs(strs);
	print_deque(cmd_deque);
}

int	main(void)
{
	char	*str;
	t_deque	cmd_deque;

	init_deque(&cmd_deque);
	while (1)
	{
		str = readline("minishell-0.0$ ");
		if (!str)
			break;
		syntax_check(str);
		// printf("%s\n", str);
		// 받은 문자열 | 로 스플릿 echo -n sdfjlskdjf | dsfcajkkle fdsfklj 
		make_cmdlst(str, &cmd_deque);
		add_history(str);
		free(str);
	}
	return (0);
}