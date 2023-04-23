/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_main.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyobicho <hyobicho@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/15 14:12:04 by hyobicho          #+#    #+#             */
/*   Updated: 2023/04/24 05:53:49 by hyobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	leack_check(void)
{
	system("leaks $PPID");
}

int	main(int ac, char **av, char **env)
{
	char	*str;
	t_deque	cmd_deque;
	t_file	file;
	// atexit(leack_check);

	(void) ac;
	(void) av;
	while (1)
	{
		init_deque(&cmd_deque);
		str = readline("minishell-0.0$ ");
		if (!str)
			break;
		syntax_check(str);
		// printf("%s\n", str);
		make_cmdlst(str, &cmd_deque, env);
		make_pipefork(&cmd_deque, &file, env);
		add_history(str);
		free(str);
		free_deque(&cmd_deque);
	}
	return (0);
}