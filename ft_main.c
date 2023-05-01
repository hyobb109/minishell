/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_main.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyobicho <hyobicho@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/15 14:12:04 by hyobicho          #+#    #+#             */
/*   Updated: 2023/05/01 15:37:17 by hyobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	leack_check(void)
{
	system("leaks $PPID");
}

int	main(int ac, char **av, char **env)
{
	char		*str;
	t_deque		cmd_deque;
	t_edeque	envp;
	t_file		file;
	// atexit(leack_check);

	(void) ac;
	(void) av;
	(void) file;
	// env deque create
	make_envlst(&envp, env);
	while (1)
	{
		str = readline("minishell-0.0$ ");
		if (!str)
			exit(EXIT_FAILURE);
		if (syntax_error(str) == FALSE)
		{
			init_deque(&cmd_deque);
			make_cmdlst(str, &cmd_deque, &envp);
			parents_process(&cmd_deque);
			free_deque(&cmd_deque);
		}
		if (*str)
			add_history(str);
		free(str);
	}
	free_edeque(&envp);
	rl_clear_history(); // free readline history list
	//TODO - history도 free?
	return (0);
}
