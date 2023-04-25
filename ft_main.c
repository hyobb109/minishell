/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_main.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunjcho <yunjcho@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/15 14:12:04 by hyobicho          #+#    #+#             */
/*   Updated: 2023/04/25 18:46:45 by yunjcho          ###   ########.fr       */
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
	//TODO - history도 free?
	return (0);
}