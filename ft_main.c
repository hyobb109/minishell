/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_main.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seulee2 <seulee2@42seoul.student.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/01 16:55:06 by hyunwoju          #+#    #+#             */
/*   Updated: 2023/05/03 18:19:14 by seulee2          ###   ########.fr       */
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
	// atexit(leack_check);

	(void) ac;
	(void) av;
	// env deque create
	make_envlst(&envp, env);
	//rl_catch_signals = 0;
	while (1)
	{
		//ft_signal_set();
		str = readline("minishell-0.0$ ");
		if (!str)
			exit(EXIT_FAILURE);
		if (syntax_error(str) == FALSE)
		{
			init_deque(&cmd_deque);
			// print_edeque(&envp);
			make_cmdlst(str, &cmd_deque, &envp);
			parents_process(&cmd_deque);
			free_deque(&cmd_deque);
		}
		if (*str)
			add_history(str);
		free(str);
		
	}
	free_edeque(&envp);
	// rl_clear_history(); // free readline history list
	return (0);
}
