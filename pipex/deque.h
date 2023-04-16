/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deque.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunjcho <yunjcho@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 14:20:52 by yunjcho           #+#    #+#             */
/*   Updated: 2023/04/16 18:23:36 by yunjcho          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEQUE_H
# define DEQUE_H

# include <unistd.h>
# include <stdlib.h>

typedef struct s_token
{
	char			*command;
	char			*option;
	char			*args;
	struct s_token	*prev;
	struct s_token	*next;
}	t_token;

typedef struct s_deque
{
	t_token	*head;
	t_token	*tail;
	int		cnt;
}	t_deque;

void	init_deque(t_deque *deque);
void	init_element(t_token *element, char **parsed);
// void	append_front(t_deque *deque, char *command);
void	append_back(t_deque *deque, t_token *token);
// t_token	*pop_front(t_deque *deque);
// t_token	*pop_back(t_deque *deque);
// void	free_deque(t_deque *deque);
void	print_deque(t_deque *deque); //delete
int		check_option(char **parsed);
char	*join_all(char **strs, int idx);
#endif