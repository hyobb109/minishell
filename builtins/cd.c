/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyobicho <hyobicho@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 01:25:35 by yunjcho           #+#    #+#             */
/*   Updated: 2023/04/26 16:53:07 by hyobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// make_dirstr() ft_split() 사용하지 않는 경우
// int	find_cutidx(char *str)
// {
// 	int	len;

// 	if (!str)
// 		return (-1); //ft_error();
// 	len = ft_strlen(str);
// 	while (len >= 0)
// 	{
// 		if (str[len] == '/')
// 			break ;
// 		len--;
// 	}
// 	return (len);
// }

char	*make_dirstr(char *str)
{
	int		idx;
	char	*result;
	char	**tmp;

	idx = 0;
	result = ft_strdup("");
	tmp = ft_split(str, '/');
	while (tmp[idx])
		idx++;
	free(tmp[idx]);
	tmp[idx] = 0;
	idx = 0;
	while (tmp[idx])
	{
		result = ft_strjoin_three(result, "/", tmp[idx]);
		// free(result); //TODO - leaks체크
		idx++;
	}
	free_strs(tmp);
	return (result);
}

char	*init_destpath(t_token *token, char *cwd_name)
{
	if (exist_args(token))
	{
		if (!ft_strcmp(token->command[1], "~"))
			return (ft_strdup(getenv("HOME")));
		else if (!ft_strcmp(token->command[1], ".."))
			return (make_dirstr(getenv("PWD"))); //TODO - free 에러 메시지, free(dest) 
		else
			return (ft_strjoin_three(cwd_name, "/", \
				token->command[1])); //free(dest)
	}
	else
		return (ft_strdup(getenv("HOME")));
}

int	exec_cd(t_token *token)
{
	int		result;
	char	*dest;
	char	cwd_name[PATH_MAX];

	result = 0;
	getcwd(cwd_name, sizeof(cwd_name));
	dest = init_destpath(token, cwd_name);
	if (access(dest, F_OK) == -1)
	{
		printf("%s: %s: %s\n", token->command[0], \
			token->command[1], strerror(errno));
		exit (EXIT_FAILURE);
	}
	getcwd(cwd_name, sizeof(cwd_name));
	printf("dest : %s, cur_dir : %s, env pwd : %s, result : %d\n", dest, cwd_name, getenv("PWD"), result);
	//TODO - env PWD 현재 위치로 바꿔야 함
	if (chdir(dest) == -1)
	{
		printf("%s: %s: %s\n", token->command[0], \
			token->command[1], strerror(errno));
		exit (EXIT_FAILURE);
	}
	free (dest);
	return (1);
}
