/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quoting.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyobicho <hyobicho@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 19:55:14 by hyobicho          #+#    #+#             */
/*   Updated: 2023/04/21 22:57:45 by hyobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_blank(char c)
{
	if (c == ' ' || c == '\t' || c == '\n')
		return (1);
	return (0);
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

void	get_size_step1(char *av, char *quote, int *size)
{
	while (is_blank(*av))
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
		while (!is_blank(*av) && *av != '\'' && *av != '\"' && *av != '\0')
		{
			++(*size);
			++av;
		}
	}
}

void	get_size_step2(char **arguments, char *av, int *size, int *index)
{
	arguments[*index] = malloc(sizeof(char) * (*size + 1));
	if (!arguments[*index])
		ft_error();
	ft_strlcpy(arguments[*index], av - (*size), (*size + 1));
	++(*index);
	*size = 0;
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
		else if (!in_quote && is_blank(c))
		{
			++rows;
		}
		++index;
	}
	if (!in_quote)
		++rows;
	return (rows);
}

char	**parse_command(char *av, char **env)
{
	int		row;
	char	**arguments;

	(void)env;
	row = count_rows(av);
	arguments = malloc(sizeof(char *) * (row + 1));
	if (!arguments)
		ft_error();
	get_size(arguments, av);
	assign_argument(arguments, av);
	arguments[row] = 0;
	return (arguments);
}
