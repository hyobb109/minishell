/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunjcho <yunjcho@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 01:25:35 by yunjcho           #+#    #+#             */
/*   Updated: 2023/04/30 19:16:00 by yunjcho          ###   ########.fr       */
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
	free(tmp[idx - 1]);
	tmp[idx - 1] = 0;
	idx = 0;
	while (tmp[idx])
	{
		result = ft_strjoin_three(result, "/", tmp[idx]); //TODO - free(old_result);
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
			return (ft_strdup(ft_getenv(token->envp, "HOME")));
		else if (!ft_strcmp(token->command[1], ".."))
			return (make_dirstr(ft_getenv(token->envp, "PWD"))); //TODO - free 에러 메시지, free(dest) 
		else
		{
			if (token->command[1][ft_strlen(token->command[1]) - 1] == '/')
				token->command[1][ft_strlen(token->command[1]) - 1] = '\0';
			return (ft_strjoin_three(cwd_name, "/", \
				token->command[1])); //free(dest)
		}
	}
	else
		return (ft_strdup(ft_getenv(token->envp, "HOME")));
}

void	change_env(t_token *token, char *dest)
{
	char	*oldpwd;
	t_env	*pwd_env;
	t_env	*oldpwd_env;

	pwd_env = find_value(token->envp, "PWD");
	oldpwd = ft_strdup(pwd_env->val);
	free(pwd_env->val);
	pwd_env->val = ft_strdup(dest);
	oldpwd_env = find_value(token->envp, "OLDPWD");
	if (!oldpwd_env)
	{
		oldpwd_env = malloc(sizeof(t_env));
		if (!oldpwd_env)
			ft_error();
		oldpwd_env->key = ft_strdup("OLDPWD");
		oldpwd_env->val = oldpwd;
		oldpwd_env->prev = NULL;
		oldpwd_env->next = NULL;
		append_back_env(token->envp, oldpwd_env);
	}
	else
		oldpwd_env->val = oldpwd;
}

int	exec_cd(t_token *token)
{
	int		result;
	char	*dest;
	char	cwd_name[PATH_MAX];

	result = 0;
	getcwd(cwd_name, sizeof(cwd_name));
	dest = init_destpath(token, cwd_name);
	// printf("----dest : %s-----\n", dest);
	if (access(dest, F_OK) == -1)
	{
		printf("%s: %s: %s\n", token->command[0], \
			token->command[1], strerror(errno));
		exit (EXIT_FAILURE);
	}
	if (chdir(dest) == -1)
	{
		printf("%s: %s: %s\n", token->command[0], \
			token->command[1], strerror(errno));
		exit (EXIT_FAILURE);
	}
	change_env(token, dest);
	getcwd(cwd_name, sizeof(cwd_name));
	// printf("dest : %s, cwd_name : %s, ft_getenv() : %s, result : %d\n", dest, cwd_name, ft_getenv(token->envp, "PWD"), result);
	free (dest);
	return (1);
}