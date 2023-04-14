/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunjcho <yunjcho@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/16 19:01:48 by yunjcho           #+#    #+#             */
/*   Updated: 2022/10/25 21:15:54 by yunjcho          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_BONUS_H
# define GET_NEXT_LINE_BONUS_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

# include <unistd.h>
# include <stdlib.h>

typedef struct s_info
{
	int				fd;
	char			buffer[BUFFER_SIZE];
	int				index;
	int				rbyte;
	int				len;
	int				total;
	struct s_info	*next;
}	t_info;

char	*get_next_line(int fd);
void	ft_memcpy(char *dst, char *src, size_t len);
char	*join(char *old, t_info *node);
void	ft_init(t_info *node, char **result);
char	*ft_exception(t_info *node, char *result, t_info **head);
t_info	*ft_get_node(t_info **head, int fd);
t_info	*ft_lstadd_back(t_info **head, int fd);
void	ft_remove_node(t_info **head, int fd);

#endif
