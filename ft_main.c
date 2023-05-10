/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_main.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunwoju <hyunwoju@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/01 16:55:06 by hyunwoju          #+#    #+#             */
/*   Updated: 2023/05/09 21:28:27 by hyunwoju         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_exit_status = 0;

void	minishell_action(t_deque *cmd_deque, t_edeque *envp)
{
	char	*str;
	char	*tmp;

	str = readline("minishell-0.0$ ");
	if (!str)
	{
		printf("exit\n");
		exit(g_exit_status);
	}
	tmp = ft_strdup(str);
	if (syntax_error(tmp) == FALSE)
	{
		init_deque(cmd_deque);
		make_cmdlst(tmp, cmd_deque, envp);
		parents_process(cmd_deque);
		rl_catch_signals = 0;
		unlink_here_doc(cmd_deque);
		free_deque(cmd_deque);
	}
	free(tmp);
	if (*str)
		add_history(str);
	free(str);
}

int	main(int ac, char **av, char **env)
{
	t_deque		cmd_deque;
	t_edeque	envp;

	(void) ac;
	(void) av;
	make_envlst(&envp, env);
	ft_signal_set();
	while (1)
	{
		minishell_action(&cmd_deque, &envp);
	}
	free_edeque(&envp);
	return (0);
}
