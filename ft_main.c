/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_main.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunwoju <hyunwoju@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/01 16:55:06 by hyunwoju          #+#    #+#             */
/*   Updated: 2023/05/05 19:48:37 by hyunwoju         ###   ########.fr       */
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
	make_envlst(&envp, env);
	rl_catch_signals = 0;
	ft_signal_set();
	while (1)
	{
		str = readline("minishell-0.0$ ");
		if (!str)
		{
			printf("exit\n");
			exit(g_exit_status);
		}
		if (!ft_strcmp(str, "$?"))
			printf("%d\n", WEXITSTATUS(g_exit_status));
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
