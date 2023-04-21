/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_tmp.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyobicho <hyobicho@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 15:15:26 by yunjcho           #+#    #+#             */
/*   Updated: 2023/04/21 23:06:05 by hyobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


int	is_blank(char c)
{
	if (c == ' ' || c == '\t' || c == '\n')
		return (1);
	return (0);
}

// $ 나오면 뒤에 한글자씩 붙여가면서 env에서 계속 확인..!
void	check_env(char *str, char **env)
{
	int	i;
	int	j;
	i = 0;
	while (env[i])
	{
		/* code */
	}
	
}

// 지금은 일차원 배열인데.. 이차원배열로 만들어야해요 고쳐주세요..(시도)
char	**parse_command11(char *str, char **env)
{
	int		i;
	int		len;
	int		quote;
	char	res[ARG_MAX];

	i = 0;
	// 처음 들어오는 공백 넘김
	while (is_blank(str[i]))
		i++;
	// command -> 공백 전까지 입력받으니까 따옴표 닫혀 있고 공백이면 break
	quote = 0;
	len = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '\"')
		{
			quote = str[i];
		}
		else if (quote && str[i] == quote)
		{
			quote = 0;
		}
		else if ((!quote && str[i] == '$') || (quote == '\"' && str[i] == '$')) // $ 나오면 환경변수 아닌 것 까지 보고 자름
		{

			i++;
			// env 쭉 보면서 환경변수 있는지 확인
			check_env(&str[i], env);
		}
		else
		{
			res[len++] = str[i];
		}
		i++;
		if (!quote && is_blank(str[i]))
			break;
	}
	res[len] = '\0';
	return (res);
}

// 파싱 테스트용 메인
int	main(int ac, char **av, char **env)
{
	(void) ac;
	int i = 0;
	char **cmd = parse_command11(av[1], env);
	for (int n = 0; cmd[n]; n++)
	{
		printf("command[%d] : %s\n", n, cmd[n]);
	}
	return (0);
}