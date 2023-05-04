/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunjcho <yunjcho@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 16:28:59 by yunjcho           #+#    #+#             */
/*   Updated: 2023/05/04 18:58:49 by yunjcho          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*init_destpath(t_token *token, char *cwd_name)
{
	size_t	target_idx;
	char	*home_dir;
	char	*result;
	char	*result2;
	char	**tmp;
	int		idx;

	(void) cwd_name;
	target_idx = -1;
	home_dir = ft_getenv(token->envp, "HOME");
	result = ft_strdup("");
	result2 = NULL;
	tmp = NULL;
	if (exist_args(token))
	{
		tmp = ft_split(token->command[1], '/');
		idx = 0;
		while (tmp[idx])
		{ 
			if (idx == 0)
			{
				if (!ft_strcmp(tmp[0], "~"))
					result2 = ft_strjoin(result, home_dir);
				else
					result2 = ft_strjoin_three(result, "/", tmp[idx]);
				free(result);
				result = result2;
				result2 = NULL;
			}
			else
			{
				result2 = ft_strjoin_three(result, "/", tmp[idx]);
				free(result);
				result = result2;
				result2 = NULL;
			}
			idx++;
		}
		free_strs(tmp);
		return (result);
	}
	else
	{
		free_strs(tmp);
		return (ft_strdup(home_dir));
	}
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
	{
		free(oldpwd_env->val);
		oldpwd_env->val = oldpwd;
	}
}

int	exec_cd(t_token *token)
{
	char	*dest;
	char	cwd_name[PATH_MAX];

	getcwd(cwd_name, sizeof(cwd_name));
	dest = init_destpath(token, cwd_name);
	if (chdir(dest) == -1)
	{
		ft_dup2(STDERR_FILENO, STDOUT_FILENO);
		printf("minishell: %s: %s: %s\n", token->command[0], \
			token->command[1], strerror(errno));
		free(dest);
		token->status = 1;
		return (1);
	}
	change_env(token, dest);
	free (dest);
	return (1);
}