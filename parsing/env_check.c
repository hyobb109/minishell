/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_check.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyobicho <hyobicho@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 05:41:25 by hyobicho          #+#    #+#             */
/*   Updated: 2023/05/05 14:45:31 by hyobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// strcmp 변형, 종료조건만 환경변수가 끝나는 조건으로 추가하고 str 인덱스를 증가시킴.
int	is_envkey(const char *s1, const char *s2, int *key_len)
{
	int		i;
	char	*str;
	char	*key;

	str = (char *)s1;
	key = (char *)s2;
	i = 0;
	// printf("str: %s, key: %s\n", s1, s2);
	//환경변수 끝나는 조건 - 숫자나 알파벳이나 '_'가 아닌 것이 나올 때
	while ((str[i] == '_' || ft_isalnum(str[i])) || key[i])
	{
		// 환경변수명과 키가 일치하지 않을 때, 그 잘못된 환경변수명만큼 길이만 늘려줌. ex) $USERa 면 5글자
		if (str[i] != key[i])
		{
			while (str[i] == '_' || ft_isalnum(str[i]))
				i++;
				// printf("str: %s str[%zu]: %c\n", str, i, str[i]);
			*key_len = i;
			return (0);
		}
		i++;
	}
	*key_len = i;
	return (1);
}

static void	check_blank(char *buf)
{
	int		i;
	char	quote;

	i = 0;
	quote = CLOSED;
	while (buf[i])
	{
		if (quote == CLOSED && (buf[i] == '\'' || buf[i] == '\"'))
		{
			quote = buf[i];
		}
		else if (quote && buf[i] == quote)
			quote = CLOSED;
		else if (quote == CLOSED && is_blank(buf[i]))
			buf[i] = BLANK;
		i++;
	}
}
// $USER hyobicho
// 달러 뒷글자부터 보면서 정상적인 환경변수인지 찾고, 찾으면 memcpy로 붙여 넣어주고 str의 길이를 리턴.
int	env_trans(char *str, t_edeque *envp, char *buf)
{
	t_env	*tmp;
	int		key_len;

	key_len = 0;
	// 환경변수 저장된 덱 돌면서 key 찾음.
	tmp = envp->head;
	while (tmp)
	{
		if (is_envkey(str, tmp->key, &key_len))
		{
			ft_memcpy(buf, tmp->val, ft_strlen(tmp->val));
			// value 에서 공백은 BLANK로 바꿔줌
			check_blank(buf);
			return (key_len);
		}
		//못찾으면 다음 환경변수 검사
		tmp = tmp->next;
	}
	// printf("key not found.. key_len: %d\n", key_len);
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
	// printf("====environ===\n");
	// printf("*str: %s\n", *str);
	if (!quote)
	{
		// 따옴표가 없는 경우는 $USER 면 U 의 주소부터 보내서 검사함.
		*str += env_trans(*str + 1, envp, &buf[0]);
		// printf("buf copied: %s\n", buf);
	}
	else
	{
		i = 0;
		// printf("*str: %s, **str: %c\n", *str, **str);
		// "hello $USER bye.." 의 경우 => $만나면 그 다음 주소부터 env_trans 함수로 보내서 치환하기
		while (**str != quote)
		{
			if (**str == '$')
			{
				// printf("*str: %s\n", *str + 1);
				key_len = env_trans(*str + 1, envp, &buf[i]);
				// printf("ket_len: %d\n", key_len);
				i = ft_strlen(buf);
				*str += key_len; // $도 넘겨줘야하니까 key_len + 1 만큼 주소 넘겨줌
				// printf("*******str: %s%%\n", *str);
			}
			else
			{
				buf[i++] = **str;
			}
			*str += 1;
		}
		// printf("buf copied: %s\n", buf);
	}
	// printf("====environ===\n");
	// printf("*str: %s\n", *str);
	return (ft_strlen(buf)); // 치환된 부분 포함하여 버퍼의 길이를 리턴.
}
