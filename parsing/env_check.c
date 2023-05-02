/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_check.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunjcho <yunjcho@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 05:41:25 by hyobicho          #+#    #+#             */
/*   Updated: 2023/05/02 23:20:50 by yunjcho          ###   ########.fr       */
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

	//환경변수 끝나는 조건 - 공백, <>, 작따,큰따, 널
	while ((str[i] != '$' && !is_blank(str[i]) && str[i] != '\'' && str[i] != '\"' && str[i] != '<' && str[i] != '>' && str[i]) || key[i])
	{
		// 환경변수명과 키가 일치하지 않을 때, 그 잘못된 환경변수명만큼 길이만 늘려줌. ex) $USERa 면 5글자
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
			// " 가 있으면 버퍼를 ' '로 감싸줌
			if (ft_strchr(tmp->val, '\"'))
			{
				// printf("val : %s, len : %ld\n", tmp->val, ft_strlen(tmp->val));
				buf[0] = '\'';
				ft_memcpy(&buf[1], tmp->val, ft_strlen(tmp->val));
				buf[ft_strlen(tmp->val)] = '\'';
			}
			// ' 가 있으면 버퍼를  " "로 감싸줌
			else if (ft_strchr(tmp->val, '\''))
			{
				buf[0] = '\"';
				ft_memcpy(&buf[1], tmp->val, ft_strlen(tmp->val));
				buf[ft_strlen(tmp->val)] = '\"';
			}
			else
				ft_memcpy(buf,tmp->val,ft_strlen(tmp->val));
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
	if (!quote)
	{
		// 따옴표가 없는 경우는 $USER 면 U 의 주소부터 보내서 검사함.
		*str += env_trans(*str + 1, envp, &buf[0]) + 1;
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
				*str += key_len + 1; // $도 넘겨줘야하니까 key_len + 1 만큼 주소 넘겨줌
				// printf("*******str: %s%%\n", *str);
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

char	*expand_environ(char *str, t_token *token, int quote)
{
	//	어차피 끝까지 볼거임
	int		len;
	char	buffer[ARG_MAX];

	// printf("**str: %s\n", str);
	ft_memset(buffer, 0, ARG_MAX); // 버퍼 초기화
	quote = CLOSED;
	len = 0;
	while (*str)
	{
		if (quote == CLOSED && (*str == '\'' || *str == '\"'))
		{
			quote = *str;
		}
		else if (quote && *str == quote)
		{
			quote = CLOSED;
		}
		// 히어독일 때는 환경변수 치환하면 안되므로 공백만나거나 널일 때까지 버퍼에 복사해줌
		else if (quote == CLOSED && *str == '<' && *(str + 1) == '<')
		{
			// 버퍼에 <<만 복사
			ft_memcpy(&buffer[len], str, 2);
			len += 2;
			str += 2;
			while (!is_blank(*str) && *str != '<' && *str != '>' && *str)
			{
				buffer[len++] = *str;
				str++;
			}
		}
		// $esd  뒤가 알파벳이나 '_'일 때만 환경변수로 처리(변수 명 조건)
		else if ((((quote == CLOSED && *str == '$') || (quote == '\"' && *str == '$')) && (ft_isalpha(*(str + 1)) || *(str + 1) == '_')))
		{
			// printf("str: %s, buf[%d]: %s\n", str, len, &buffer[len]);
			// printf("quote: %c\n", quote);
			len += search_env(&str, &buffer[len], token->envp, quote); // $ 위치부터 보내주기
			if (quote)
				quote = CLOSED; // 따옴표 닫아줌 (환경변수 치환되면서 따옴표 제거됨)
		}
		if (quote == CLOSED && *str == '$')
			continue;
		buffer[len++] = *str;
		if (*str == '\0')
			break ;
		str++;
		printf("str:%s, buf : %s, buf_len: %d\n", str, buffer, len);
	}
	buffer[len] = '\0';
	// printf("============\n");
	printf("environ expansion result : %s\n", buffer);
	// 버퍼에 환경변수 모두 치환된 결과 담김.
	// 메모리 새로 할당하여 리턴.
	return (ft_strdup(buffer));
	// printf("res : %s\n", res);
}
