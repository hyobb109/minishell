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
// ex) $hello 일때 h부터 he, hel, hell, hello 이런 식으로..
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

// 따옴표는 대충 했는데 테스트 안해봄, 환경변수 처리 해야함!
// "" '' 는 빈문자열 입력된거로 처리 해야하는데 아직 안 함
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
		else if (!quote && is_blank(str[i]))
		{
			str[i] = '\0'; // 따옴표 밖 공백이면 자름
			//break;
			}
			else
		{
			res[len++] = str[i];
		}
		i++;
	}
	res[len] = '\0';
	return (ft_split_c(res, "\0"));
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