/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gnl.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunwoju <hyunwoju@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 15:10:05 by hyunwoju          #+#    #+#             */
/*   Updated: 2023/05/08 18:34:49 by hyunwoju         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	ft_linelen(char *s)
{
	size_t	i;

	i = 0;
	while (*(s + i) != '\n' && *(s + i) != '\0')
		i++;
	if (*(s + i) == '\0')
		return (i);
	return (i + 1);
}

static char	*ft_extract_line(char *buffer)
{
	char	*line;
	int		size;

	if (!buffer || buffer[0] == '\0')
		return (0);
	size = ft_linelen(buffer) + 1;
	line = ft_calloc(size, sizeof (char));
	if (line == 0)
		return (0);
	ft_strlcpy(line, buffer, size);
	return (line);
}

static char	*ft_read(int fd, char *buffer)
{
	char	*new_buffer;
	char	*reading;
	int		byte_read;

	if (ft_strchr(buffer, '\n') != 0)
		return (buffer);
	reading = ft_calloc((BUFFER_SIZE + 1), sizeof(char));
	if (reading == 0)
		return (buffer);
	while (ft_strchr(reading, '\n') == 0)
	{
		byte_read = read(fd, reading, BUFFER_SIZE);
		if (byte_read < 1)
			break ;
		reading[byte_read] = '\0';
		new_buffer = ft_strjoin(buffer, reading);
		free (buffer);
		buffer = new_buffer;
	}
	free (reading);
	return (buffer);
}

static char	*ft_remove_line(char *buffer)
{
	int		start;
	char	*newbuffer;
	int		i;

	if (!buffer)
		return (0);
	if (buffer[0] == '\0')
	{
		free (buffer);
		return (0);
	}
	start = ft_linelen(buffer);
	newbuffer = ft_calloc (ft_strlen(buffer) - start + 1, sizeof (char));
	if (newbuffer == 0)
		return (0);
	i = 0;
	while (buffer[start + i] != '\0')
	{
		newbuffer[i] = buffer[start + i];
		i++;
	}
	newbuffer[i] = buffer[start + i];
	free (buffer);
	return (newbuffer);
}

char	*get_next_line(int fd)
{
	static char	*buffer[1024];
	char		*line;

	if (fd < 0 || fd > 1023 || BUFFER_SIZE < 1)
		return (0);
	if (read(fd, 0, 0) < 0 && !buffer[fd])
		return (0);
	if (!buffer[fd])
		buffer[fd] = ft_calloc(1, 1);
	if (buffer[fd] == 0)
		return (0);
	buffer[fd] = ft_read(fd, buffer[fd]);
	line = ft_extract_line(buffer[fd]);
	buffer[fd] = ft_remove_line(buffer[fd]);
	return (line);
}
