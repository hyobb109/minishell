/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quoting.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyobicho <hyobicho@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 19:55:14 by hyobicho          #+#    #+#             */
/*   Updated: 2023/04/21 20:01:30 by hyobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	assign_argument_tmp(char **str, char *av)
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

void	get_size_step1_tmp(char *av, char *quote, int *size)
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

void	get_size_step2_tmp(char **arguments, char *av, int *size, int *index)
{
	arguments[*index] = malloc(sizeof(char) * (*size + 1));
	ft_strlcpy(arguments[*index], av - (*size), (*size + 1));
	++(*index);
	*size = 0;
}

int	count_rows_tmp(char *argument)
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

void	get_size_tmp(char **arguments, char *av)
{
	int		size;
	int		index;
	char	quote;

	size = 0;
	index = 0;
	quote = 0;
	while (*av != '\0')
	{
		get_size_step1_tmp(av, &quote, &size);
		if (size)
			get_size_step2_tmp(arguments, av, &size, &index);
		++av;
	}
}


char	**parse_command_tmp(char *av)
{
	int		row;
	char	**arguments;

	row = count_rows_tmp(av);
	arguments = malloc(sizeof(char *) * row + 1);
	get_size_tmp(arguments, av);
	assign_argument_tmp(arguments, av);
	arguments[row] = 0;
	return (arguments);
}

