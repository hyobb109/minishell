/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunwoju <hyunwoju@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 16:28:59 by yunjcho           #+#    #+#             */
/*   Updated: 2023/05/09 20:14:18 by hyunwoju         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*init_destpath(t_token *token)
{
	int		idx;
	char	*result;
	char	*result2;
	char	**tmp;

	idx = 0;
	result = ft_strdup("");
	result2 = NULL;
	tmp = ft_split(token->command[1], '/');
	while (tmp[idx])
	{
		if (idx == 0)
		{
			if (!ft_strcmp(tmp[0], "~"))
				result2 = ft_strjoin(result, ft_getenv(token->envp, "HOME"));
			else
				result2 = ft_strjoin_three(result, "/", tmp[idx]);
		}
		else
			result2 = ft_strjoin_three(result, "/", tmp[idx]);
		result = swapfree_strs(result, result2);
		idx++;
	}
	free_strs(tmp);
	return (result);
}

char	*make_destpathstr(t_token *token)
{
	char	*dest;

	dest = NULL;
	if (exist_args(token))
	{
		if (ft_strchr_idx(token->command[1], '~') >= 0)
			dest = init_destpath(token);
		else
		{
			dest = ft_strdup(token->command[1]);
			if (ft_strlen(dest) > 1 && dest[ft_strlen(dest) - 1] == '/')
				dest[ft_strlen(dest) - 1] = '\0';
		}
	}
	else
		dest = ft_strdup(ft_getenv(token->envp, "HOME"));
	return (dest);
}

int	change_env(t_token *token, char *cwd_name)
{
	t_env	*old_pwd;
	t_env	*cur_pwd;

	old_pwd = NULL;
	old_pwd = find_value(token->envp, "OLDPWD");
	if (old_pwd)
	{
		free(old_pwd->val);
		old_pwd->val = NULL;
	}
	else
	{
		old_pwd = init_envelem("OLDPWD", cwd_name);
		if (!old_pwd)
			return (-1);
		append_back_env(token->envp, old_pwd);
	}
	cur_pwd = find_value(token->envp, "PWD");
	if (cur_pwd == NULL)
		return (-1);
	old_pwd->val = ft_strdup(cur_pwd->val);
	if (cur_pwd->val)
		free(cur_pwd->val);
	cur_pwd->val = ft_strdup(cwd_name);
	return (1);
}

int	exec_cd(t_token *token)
{
	int		result;
	char	*dest;
	char	cwd_name[PATH_MAX];

	result = 1;
	dest = make_destpathstr(token);
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
	result = change_env(token, cwd_name);
	free (dest);
	return (result);
}
