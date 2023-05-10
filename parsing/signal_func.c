/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_func.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunwoju <hyunwoju@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 18:06:46 by seulee2           #+#    #+#             */
/*   Updated: 2023/05/10 16:11:12 by hyunwoju         ###   ########.fr       */
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
	if (sig == SIGQUIT)
	{
		printf("Quit : 3\n");
		g_exit_status = 131;
	}
}

void	signal_handler_parent(int sig)
{
	if (sig == SIGINT)
	{
		printf("Quit : 3\n");
		g_exit_status = 131;
	}
}

void	signal_handler_child(int sig)
{
	if (sig == SIGQUIT)
	{
		g_exit_status = 131 * 256;
		exit(g_exit_status);
	}
}

void	signal_handler_heredoc(int sig)
{
	if (sig == SIGINT)
	{
		rl_replace_line("", 0);
		printf("\n");
	}
}

void	ft_signal_set(void)
{
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, SIG_IGN);
}

void	ft_signal_child(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, signal_handler);
}
