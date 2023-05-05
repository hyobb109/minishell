/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_func.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunwoju <hyunwoju@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 18:06:46 by seulee2           #+#    #+#             */
/*   Updated: 2023/05/05 16:17:47 by hyunwoju         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	signal_handler(int sig)
{
	if (sig == SIGINT)
	{
		rl_replace_line("", 0);
		printf("\n");
		rl_on_new_line();
		rl_redisplay();
	}
}

void	signal_handler_child(int sig)
{
	if (sig == SIGINT)
	{
		printf("child SIGINT\n");
		rl_replace_line("", 0);
        write(1, "\b\b", 2);
        rl_done = 1;
        rl_on_new_line();
        rl_redisplay();
		exit(0);
	}
}

void	signal_handler_parent(int sig)
{
	if (sig == SIGINT)
	{
		signal(SIGINT, SIG_IGN);
	}
}

void	ft_signal_parent(void)
{
	signal(SIGINT, signal_handler_parent);	
}
void	ft_signal_set(void)
{
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, SIG_IGN);
}

void	ft_signal_child_set(void)
{
	printf("sig child set\n");
	signal(SIGINT, signal_handler_child);
}


