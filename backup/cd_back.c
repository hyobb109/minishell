/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunjcho <yunjcho@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 01:25:35 by yunjcho           #+#    #+#             */
/*   Updated: 2023/05/03 16:27:18 by yunjcho          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*make_parentdir(char *cur_dir)
{
	int		idx;
	char	*result;
	char	*result2;
	char	**tmp;

	idx = 0;
	result = ft_strdup("");
	result2 = NULL;
	tmp = ft_split(cur_dir, '/');
	while (tmp[idx])
		idx++;
	if (idx != 1)
	{
		free(tmp[idx - 1]);
		tmp[idx - 1] = 0;
	}
	else
	{
		result2 = tmp[idx - 1];
		free(result2);
		tmp[idx - 1] = ft_strdup("");
	}
	idx = 0;
	while (tmp[idx])
	{
		result2 = ft_strjoin_three(result, "/", tmp[idx]);
		free(result);
		result = result2;
		result2 = NULL;
		idx++;
	}
	free_strs(tmp);
	return (result);
}

char	*make_dirstr(char *str, char *cwd_name, char *home_dir) //절대 경로 & 상대 경로
{
	int		idx;
	char	**tmp;
	char	*result;
	char	*result2;
	char 	*parent_dir;
	int		parent_cnt;

	parent_cnt = 0;
	idx = 0;
	tmp = ft_split(str, '/'); //인자를 쪼갬
	result = ft_strdup("");
	result2 = NULL;
	parent_dir = make_parentdir(cwd_name);
	while (tmp[idx])
	{
		if (!ft_strcmp(tmp[idx], "~"))
		{
			result2 = ft_strjoin(result, home_dir);
			free(result);
			result = result2;
			result2 = NULL;
		}
		else if (!ft_strcmp(tmp[idx], "."))
		{
			if (!ft_strcmp(result, ""))
				result2 = ft_strjoin(result, cwd_name);
			else
				result2 = ft_strjoin_three(result, "/", cwd_name);
			free(result);
			result = result2;
			result2 = NULL;
		}
		else if (!ft_strcmp(tmp[idx], "..")) //TODO - ../../ 처리
		{
			parent_cnt++;
			printf("parent_cnt : %d\n", parent_cnt);
			result2 = ft_strjoin(result, parent_dir);
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
	
	return (result);
}

char	*init_destpath(t_token *token, char *cwd_name)
{
	size_t	target_idx;
	char	*home_dir;

	target_idx = -1;
	home_dir = ft_getenv(token->envp, "HOME");
	printf("home_dir : %s\n", home_dir);
	if (exist_args(token))
	{
		if (!ft_strcmp(token->command[1], "/")) //root
			return (ft_strdup("/"));
		else
		{
			target_idx = ft_strchr_idx(token->command[1], '/');
			if (target_idx < 0)
				return (ft_strjoin_three(cwd_name, "/", token->command[1]));
			else if (target_idx == (ft_strlen(token->command[1]) - 1))
			{
				printf("delete //\n");
				token->command[1][ft_strlen(token->command[1]) - 1] = '\0';
				return (ft_strjoin_three(cwd_name, "/", token->command[1]));
			}
			else
				return (make_dirstr(token->command[1], cwd_name, home_dir));
		}
	}
	else
		return (ft_strdup(home_dir));
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
	if (chdir(token->command[1]) == -1)
	{
		printf("minishell: %s: %s: %s\n", token->command[0], \
			token->command[1], strerror(errno));
		// free(dest);
		token->status = 1;
		return (1);
	}
	// change_env(token, dest);
	// free (dest);
	return (1);
}