/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunjcho <yunjcho@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/16 19:57:20 by hyobicho          #+#    #+#             */
/*   Updated: 2023/04/21 21:58:15 by yunjcho          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	exec_pwd(void)
{
	char	cwd_name[PATH_MAX];

	getcwd(cwd_name, sizeof(cwd_name));
	ft_putendl_fd(cwd_name, 1);
	return (1);
}

int	exec_exit(t_token *token)
{
	//TODO - 프로그램 종료시 반환값 전달 가능한지 확인 후 리턴값 없애기
	//TODO - 프로그램 종료 실패 시 예외처리 추가?
	ft_putstr_fd("exit\n", 1);
	return (!kill(token->pid, SIGKILL));
}

void	assign_argument(char **str, char *av)
{
	t_matrix	matrix;
	int			in_quotes;
	int			in_single_quotes;

	matrix.row = 0;
	matrix.column = 0;
	in_quotes = 0;
	in_single_quotes = 0;
	while (*av)
	{
		if (*av == '\"' && !in_single_quotes)
			in_quotes = !in_quotes;
		else if (*av == '\'' && !in_quotes)
			in_single_quotes = !in_single_quotes;
		else if (*av == ' ' && !in_quotes && !in_single_quotes)
		{
			str[(matrix.row)++][matrix.column] = '\0';
			matrix.column = 0;
		}
		else
			str[matrix.row][(matrix.column)++] = *av;
		av++;
	}
	str[matrix.row][matrix.column] = '\0';
}

void	get_size(char **arguments, char *av)
{
	int		size;
	int		index;
	char	quote;

	size = 0;
	index = 0;
	quote = 0;
	while (*av != '\0')
	{
		get_size_step1(av, &quote, &size);
		if (size)
			get_size_step2(arguments, av, &size, &index);
		++av;
	}
}

void	get_size_step1(char *av, char *quote, int *size)
{
	while (*av == ' ')
		++av;
	if (*av == '\'' || *av == '\"')
	{
		*quote = *av;
		++av;
		while (*av != *quote && *av != '\0')
		{
			++(*size);
			++av;
		}
		if (*av == *quote)
			++av;
	}
	else
	{
		while (*av != ' ' && *av != '\'' && *av != '\"' && *av != '\0')
		{
			++(*size);
			++av;
		}
	}
}

void	get_size_step2(char **arguments, char *av, int *size, int *index)
{
	arguments[*index] = malloc(sizeof(char) * (*size + 1));
	ft_strlcpy(arguments[*index], av - (*size), (*size + 1));
	++(*index);
	*size = 0;
}

int	count_rows(char *argument) //token->args가 인자로 들어옴
{
	int		rows;
	int		index;
	int		in_quote;
	char	c;

	rows = 0;
	index = 0;
	in_quote = 0;
	while (argument[index] != '\0')
	{
		c = argument[index];
		if (c == '\'' || c == '\"')
		{
			in_quote = !in_quote;
		}
		else if (!in_quote && (c == ' ' || c == '\t' || c == '\n')) // 공백문자 추가(23.04.21)
		{
			++rows;
		}
		++index;
	}
	if (!in_quote)
		++rows;
	return (rows);
}

// echo 명령어에서 출력을 시작할 행의 인덱스를 반환
int	check_option(char **arguments)
{
	int	idx;

	idx = 0;
	if (!arguments[0])
		return (0);
	if (!strncmp(arguments[0], "-n", 2))
	{
		idx = 2;
		while (arguments[0][idx])
		{
			if (arguments[0][idx] != 'n')
				return (0);
			idx++;
		}
	}
	else
		return (0);
	return (1);
}

char	**parse_args(char *av)
{
	int		row;
	char	**arguments;

	row = count_rows(av);
	arguments = malloc(sizeof(char *) * row + 1);
	get_size(arguments, av);
	assign_argument(arguments, av);
	arguments[row] = 0;
	return (arguments);
}

void	print_args(char **arguments, int target_idx)
{
	while(arguments[target_idx])
	{
		ft_putstr_fd(arguments[target_idx], 1);
		target_idx++;
		if (arguments[target_idx] != 0)
			ft_putstr_fd(" ", 1);
	}
}

int	exec_echo(t_token *echo)
{
	int		target_idx;
	char	**arguments;

	arguments = NULL;
	echo->args = "abc def";
	if (echo->args)
		arguments = parse_args(echo->args);

	//TODO - Debuggin 추후 삭제
	// int i = 0;
	// while (arguments[i])
	// {
	// 	printf("arguments[%d] %s\n", i, arguments[i]);
	// 	i++;
	// }

	target_idx = check_option(arguments);
	if (target_idx)
	{
		// printf("yes\n");
		print_args(arguments, target_idx); //-n 옵션이 유효한 경우 (-n은 출력 안함)
	}
	else
	{
		// printf("no\n");
		ft_putendl_fd(echo->args, 1); //-n 옵션이 무효한 경우 (-n 출력)
	}
	return (1);
}

int	exec_builtins(t_token *token)
{
	if (!strcmp(ft_strlowcase(token->command), "echo"))
		return(exec_echo(token));
	else if (!strcmp(ft_strlowcase(token->command), "pwd"))
		return (exec_pwd());
	else if (!strcmp(ft_strlowcase(token->command), "env"))
		return (1);
	else if (!strcmp(token->command, "cd"))
		return (1);
	else if (!strcmp(token->command, "export"))
		return (1);
	else if (!strcmp(token->command, "unset"))
		return (1);
	else if (!strcmp(token->command, "exit"))
		return (exec_exit(token));
	return (0);
}
