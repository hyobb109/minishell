/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunjcho <yunjcho@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 16:28:59 by yunjcho           #+#    #+#             */
/*   Updated: 2023/05/05 21:56:37 by yunjcho          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*init_destpath(t_token *token, char *cwd_name)
{
	size_t	target_idx;
	char	*result;
	char	*result2;
	char	**tmp;
	int		idx;

	(void) cwd_name;
	target_idx = -1;
	result = ft_strdup("");
	result2 = NULL;
	tmp = NULL;
	tmp = ft_split(token->command[1], '/');
	idx = 0;
	while (tmp[idx])
	{ 
		if (idx == 0)
		{
			if (!ft_strcmp(tmp[0], "~"))
				result2 = ft_strjoin(result, ft_getenv(token->envp, "HOME"));
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

void	change_env(t_token *token, char *cwd_name)
{
	char	*free_path;
	t_env	*old_pwd;
	t_env	*cur_pwd;
	// char	cwd_name[PATH_MAX];

	old_pwd = NULL;
	free_path = NULL;
	old_pwd = find_value(token->envp, "OLDPWD");
	printf("old_pwd_val: %p\n", old_pwd);
	if (old_pwd)
	{
		free_path = old_pwd->val;
		free(free_path);
		free_path = NULL;
		old_pwd->val = NULL;
	}
	else
	{
		old_pwd = malloc(sizeof(t_env));
		old_pwd->val = ft_strdup(cwd_name);
		old_pwd->key = ft_strdup("OLDPWD");
		old_pwd->prev = NULL;
		old_pwd->next = NULL;
		append_back_env(token->envp, old_pwd);
	}
	cur_pwd = find_value(token->envp, "PWD");
	printf("curr_pwd_val: %s\n", cur_pwd->val);
	old_pwd->val = ft_strdup(cur_pwd->val);
	printf("**old_pwd_val: %s\n", old_pwd->val);
	if (cur_pwd->val)
		free(cur_pwd->val);
	cur_pwd->val = ft_strdup(cwd_name);
	printf("**curr_pwd_val: %s\n", cur_pwd->val);
}

int	exec_cd(t_token *token)
{
	char	*dest;
	char	cwd_name[PATH_MAX];

	if (exist_args(token))
	{
		if (ft_strchr_idx(token->command[1], '~') >= 0)
			dest = init_destpath(token, cwd_name);
		else
		{
			dest = ft_strdup(token->command[1]);
			if (ft_strlen(dest) > 1 && dest[ft_strlen(dest) - 1] == '/')
				dest[ft_strlen(dest) - 1] = '\0';
		}
	}
	else
		dest = ft_strdup(ft_getenv(token->envp, "HOME"));
	printf("dest : %s\n", dest);
	if (chdir(dest) == -1)
	{
		ft_dup2(STDERR_FILENO, STDOUT_FILENO, token->func);
		printf("minishell: %s: %s: %s\n", token->command[0], \
			dest, strerror(errno));
		free(dest);
		g_exit_status = 256;
		return (-1);
	}
	getcwd(cwd_name, sizeof(cwd_name));
	printf("cwd: %s\n", cwd_name);
	change_env(token, cwd_name);
	free (dest);
	return (1);
}
