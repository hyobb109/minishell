// 토큰 파일리스트에 파일 이름 복사해서 담고 증가시킨 str 인덱스 리턴
int	get_filename(char *str, t_fdata *new, t_token *token)
{
	int		quote;
	int		i;
	int		len;
	char	*tmp;
	int		flag;
	char	buffer[PATH_MAX];

	tmp = str;
	len = 0;
	i = 0;
	while (is_blank(str[i]))
		i++;
	quote = CLOSED;
	flag = 0;
	while (str[i])
	{
		if (quote == CLOSED && (is_blank(str[i]) || str[i] == '<' || str[i] == '>'))
			break;
		if (quote == CLOSED && (str[i] == '\'' || str[i] == '\"'))
		{
			quote = str[i];
			if (quote == '\"')
				flag = quote;
			if (new->type == LIMITER)
				new->type = Q_LIMITER; // heredoc 에서 리미터에 따옴표 있으면 입력된 환경변수 치환 안하므로 따로 분류
		}
		else if (quote && str[i] == quote)
		{
			quote = CLOSED;
		}
		else
		{
			new->filename[len++] = str[i];
		}
		i++;
	}
	new->filename[len] = '\0';
	// while (is_blank(*str))
	// 	str++;
	// len = 0;
	// quote = CLOSED;
	// while (*str)
	// {
	// 	if (quote == CLOSED && (*str == '\'' || *str == '\"'))
	// 	{
	// 		quote = *str;
	// 		if (new->type == LIMITER)
	// 			new->type = Q_LIMITER; // heredoc 에서 리미터에 따옴표 있으면 입력된 환경변수 치환 안하므로 따로 분류
	// 	}
	// 	else if (quote && *str == quote)
	// 	{
	// 		quote = CLOSED;
	// 	}
	// 	// $ 뒤가 알파벳이나 '_'일 때만 환경변수로 처리(변수 명 조건)
	// 	else if ((new->type != LIMITER && new->type != Q_LIMITER) && ((quote == CLOSED && *str == '$') || (quote == '\"' && *str == '$')) && (ft_isalpha(*(str + 1)) || *(str + 1) == '_'))
	// 	{
	// 		len += search_env(&str, &new->filename[len], token->envp, quote);
	// 		if (quote)
	// 			quote = CLOSED;
	// 	}
	// 	else
	// 	{
	// 		new->filename[len++] = *str;
	// 	}
	// 	printf("**str: %s, buf : %s, buf_len: %d\n", str, new->filename, len);
	// 	if (quote == CLOSED && (is_blank(*str) || *str == '<' || *str == '>' || *str == '\0'))
	// 		break;
	// 	str++;
	// }
	// new->filename[len] = '\0';
	
	// 히어독 제외하고 파일 이름이 환경변수라면 치환해줌
	if (new->filename[0] == '$' && new->type != LIMITER && new->type != Q_LIMITER)
	{
		ft_memset(buffer, 0, PATH_MAX);
		env_trans(&new->filename[1], token->envp, buffer);
		len =ft_strlen(buffer);
		// 환경변수가 없으면 에러 메시지 출력, 파일이름은 빈문자열이 됨
		if (!len)
		{
			printf("minishell: %s: ambiguous redirect\n", new->filename);
		}
		// 기존 파일 이름 버퍼로 교체, 널까지 복사해야하므로 len + 1만큼 카피
		ft_memcpy(new->filename, buffer, len + 1);
		printf("file_buffer: %s\n", buffer);
	}
	printf("file: %s\n", new->filename);
	append_file(&token->files, new);
	if (new->type == LIMITER || new->type == Q_LIMITER || new->type == APPEND)
		i++;
	return (i);
}

