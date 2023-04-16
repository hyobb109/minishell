/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_main.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyobicho <hyobicho@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/15 14:12:04 by hyobicho          #+#    #+#             */
/*   Updated: 2023/04/16 22:20:40 by hyobicho         ###   ########.fr       */
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

void	make_cmdlst(char *str, t_deque *cmd_deque)
{
	char		*tmp;
	char		**strs;
	char		**parsed;
	t_token		*token;
	int			i;

	i = 0;
	strs = ft_split(str, '|');
	while (strs[i])
	{
		tmp = ft_strtrim(strs[i], " ");
		// 다시 공백으로 스플릿! [0] : cmd [1]: echo면 옵션 아니면 others,
		parsed = ft_split(tmp, ' ');
		token = malloc(sizeof(t_token));
		if(!token)
			ft_error();
		init_element(token, parsed);
		append_back(cmd_deque, token);
		i++;
	}
	// free_strs(parsed);
	// free_strs(strs);
	print_deque(cmd_deque);
}

// void	exec_cmds(t_deque *cmd_deque)
// {
// 	t_token	*token;

// 	token = cmd_deque->head;
// 	while (token)
// 	{
// 		if (!exec_builtins(token))
// 			exec_pipe();
// 		token = token->next;
// 	}
// }

void	leack_check(void)
{
	system("leaks $PPID");
}

int	main(void)
{
	char	*str;
	t_deque	cmd_deque;
	t_file	file;
	atexit(leack_check);
	while (1)
	{
		init_deque(&cmd_deque);
		str = readline("minishell-0.0$ ");
		if (!str)
			break;
		syntax_check(str);
		// printf("%s\n", str);
		// 받은 문자열 | 로 스플릿 echo -n sdfjlskdjf | dsfcajkkle fdsfklj 
		make_cmdlst(str, &cmd_deque);
		make_pipefork(&cmd_deque, &file);
		// exec_cmds(&cmd_deque);
		add_history(str);
		free(str);
		free_deque(&cmd_deque);
	}
	return (0);
}