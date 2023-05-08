/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_main.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunjcho <yunjcho@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/01 16:55:06 by hyunwoju          #+#    #+#             */
/*   Updated: 2023/05/08 22:38:33 by yunjcho          ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_exit_status = 0;

int	main(int ac, char **av, char **env)
{
	char		*str;
	char		*tmp;
	t_deque		cmd_deque;
	t_edeque	envp;

	(void) ac;
	(void) av;
	make_envlst(&envp, env);
	// rl_catch_signals = 0;
	ft_signal_set();
	while (1)
	{
		str = readline("minishell-0.0$ ");
		if (!str)
		{
			printf("exit\n");
			exit(g_exit_status);
		}
		tmp = ft_strdup(str);
		if (syntax_error(str) == FALSE)
		{
			init_deque(&cmd_deque);
			// print_edeque(&envp);
			make_cmdlst(tmp, &cmd_deque, &envp);
			parents_process(&cmd_deque);
			unlink_here_doc(&cmd_deque);
			free_deque(&cmd_deque);
		}
		free(tmp);
		if (*str)
			add_history(str);
		free(str);
	}
	free_edeque(&envp);
	// rl_clear_history(); // free readline history list
	return (0);
}
