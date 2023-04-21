/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunjcho <yunjcho@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/16 19:57:20 by hyobicho          #+#    #+#             */
/*   Updated: 2023/04/21 16:51:39 by yunjcho          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	exec_pwd(void)
{
	// 디렉토리 최대 길이(나중에 제대로 확인)
	char	cwd_name[PATH_MAX];
	// printf("token->command: %s\n", token->command);
	getcwd(cwd_name, sizeof(cwd_name));
	ft_putendl_fd(cwd_name, 1);
	return (1);
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

int	count_rows(char *argument)
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
		else if (!in_quote && c == ' ')
		{
			++rows;
		}
		++index;
	}
	if (!in_quote)
		++rows;
	return (rows);
}

// echo 명령어에서 join 시작할 인덱스를 리턴
int	check_option(char **arguments)
{
	int	i;
	int	j;

	i = 0;
	if (!arguments[0])
		return (0);
	while (arguments[i])
	{
		if (!strncmp(arguments[i], "-n", 2))
		{
			j = 2;
			while (arguments[i][j])
			{
				if (arguments[i][j] != 'n')
					return (0);
				j++;
			}
		}
		else	
			return (0);
		i++;
	}
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

// void	parse_args(char *str)
// {
// 	char	quote;
// 	int		i;

// 	i = 0;
// 	quote = 0;
// 	while (str[i])
// 	{
// 		if (quote && str[i] == quote)
// 		{
// 			quote = 0;
// 			str[i] = -1;
// 		}
// 		else if (quote == 0 && (str[i] == '\'' || str[i] == '\"'))
// 		{
// 			quote = str[i];
// 			str[i] = -1;
// 		}
// 		i++;
// 	}
// }

int	exec_echo(t_token *echo)
{
	char **arguments;

	arguments = NULL;
	echo->args = "-n -n-n-n-n \"Hello \'42\'\"             ";
	if (echo->args)
		arguments = parse_args(echo->args);
	if (check_option(arguments))
	{
		printf("yes\n");
		ft_putstr_fd(echo->args, 1); //-n 옵션이 유효한 경우 (-n은 출력 안함)
	}
	else
	{
		printf("no\n");
	
		ft_putendl_fd(echo->args, 1); //-n 옵션이 무효한 경우 (-n 출력)
	}
	// int idx = 0;
	// while (arguments[idx])
	// {
	// 	printf("args : %s\n", arguments[idx]);
	// 	idx++;
	// }
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
		return (1);
	return (0);
}
