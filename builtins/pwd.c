/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunjcho <yunjcho@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/16 19:57:20 by hyobicho          #+#    #+#             */
/*   Updated: 2023/05/08 17:40:04 by yunjcho          ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	exec_pwd(void)
{
	char	cwd_name[PATH_MAX];

	getcwd(cwd_name, sizeof(cwd_name));
	printf("%s\n", cwd_name);
	return (1);
}
