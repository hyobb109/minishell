/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_check.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyobicho <hyobicho@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 05:41:25 by hyobicho          #+#    #+#             */
/*   Updated: 2023/04/28 21:58:45 by hyobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_envkey(const char *s1, const char *s2, size_t *key_len)
{
	size_t	i;
	char	*str;
	char	*key;

	str = (char *)s1;
	key = (char *)s2;
	i = 0;

	//환경변수 끝나는 조건 - 공백, <>, 작따,큰따
	while ((str[i] != '$' && !is_blank(str[i]) && str[i] != '\'' && str[i] != '\"' && str[i] != '<' && str[i] != '>' && str[i]) || key[i])
	{
		if (str[i] != key[i])
		{
			while (str[i] != '$' && !is_blank(str[i]) && str[i] != '\'' && str[i] != '\"' && str[i] != '<' && str[i] != '>' && str[i])
			{
				i++;
				// printf("str: %s str[%zu]: %c\n", str, i, str[i]);
				*key_len = i;
			}
			return (0);
		}
		i++;
	}
	*key_len = i;
	return (1);
}
// $USER hyobicho
// 달러 뒤에 부터 보면서 정상적인 환경변수인지 찾고, 찾으면 memcpy로 붙여 넣어주고 str의 길이를 리턴.
int	env_trans(char *str, int i, t_edeque *envp, char *buf)
{
	t_env	*tmp;
	size_t	key_len;

	key_len = 0;
	tmp = envp->head;
	while (tmp)
	{
		if (is_envkey(str + i, tmp->key, &key_len))
		{
			ft_memcpy(buf,tmp->val,ft_strlen(tmp->val));
			return (key_len);
		}
		//못찾으면 다음 환경변수 검사
		tmp = tmp->next;
	}
	// printf("key not found.. key_len: %zu\n", key_len);
	return (key_len);
}
// 넘어올 때 큰따옴표를 만나거나, 따옴표 없는 환경변수 만나면 그 주소 넘어 옴.
// 1. quote == 0 -> 따옴표 없이 환경변수 만나서 들어온 경우, str은 '$'의 주소.
// 2. quote == \" -> "$USER" , str은 '\"'의 주소. ex) "hello $USER bye.."
int	search_env(char **str, char *buf, t_edeque *envp, int quote)
{
	int	i;
	int	key_len;

	// 환경변수 끝나는 조건 - 공백, <>, 작따,큰따
	// 따옴표 없을 때	
		// str == $USER 
		// buf == seulee2
		//버퍼에 복사되고, str은 환경변수 끝에 포인터 위치.
	// }
	if (!quote)
	{
		// 따옴표가 없는 경우는 $USER 면 U 의 주소부터 보내서 검사함.
		*str += env_trans(*str, 1, envp, &buf[0]) + 1;
	}
	else
	{
		i = 0;
		printf("*str: %s, **str: %c\n", *str, **str);
		// "hello $USER bye.." 의 경우 => $만나면 그 다음 주소부터 env_trans 함수로 보내서 치환하기
		while (**str != quote)
		{
			if (**str == '$')
			{
				// printf("*str: %s\n", *str);
				key_len = env_trans(*str, 1, envp, &buf[i]);
				// printf("ket_len: %d\n", key_len);
				i = ft_strlen(buf);
				*str += key_len + 1;
				// printf("*******str: %s\n", *str);
			}
			else
			{
				buf[i++] = **str;
				*str += 1;
			}
		}
		// printf("buf copied: %s\n", buf);
	}
	return (ft_strlen(buf)); // 치환된 부분 포함하여 버퍼의 길이를 리턴.
}
