/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyobicho <hyobicho@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 14:45:33 by hyobicho          #+#    #+#             */
/*   Updated: 2022/11/16 21:41:06 by hyobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new;
	t_list	*tmp;
	t_list	*head;

	if (lst == NULL || f == NULL)
		return (0);
	new = ft_lstnew(f(lst->content));
	if (new == NULL)
		return (0);
	lst = lst->next;
	head = new;
	tmp = new;
	while (lst)
	{
		new = ft_lstnew(f(lst->content));
		if (new == NULL)
		{
			ft_lstclear(&head, del);
			return (0);
		}
		lst = lst->next;
		tmp->next = new;
		tmp = tmp->next;
	}
	return (head);
}
