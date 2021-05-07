/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   get_next_line.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: mramadan <mramadan@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/11/19 15:47:48 by mramadan      #+#    #+#                 */
/*   Updated: 2021/05/07 23:23:24 by Marty         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <unistd.h>

#include <fcntl.h>
#include <stdio.h>

int		write_return(char *str, char **line, int ret)
{
	int		i;

	i = 0;
	(*line) = malloc(sizeof(char) * (ft_strlen(str) + 1));
	if (!(*line))
	{
		free(str);
		return (-1);
	}
	while (str[i] != '\0' && str[i] != '\n')
	{
		(*line)[i] = str[i];
		i++;
	}
	(*line)[i] = '\0';
	if (ret == 0)
		free(str);
	return (0);
}

char	*get_that_line(char *str, char **line)
{
	char			*tmp;
	int				check;

	check = write_return(str, line, 1);
	if (check == -1)
		return (0);
	tmp = str;
	str = ft_strdup(ft_strchr(str, '\n'));
	free(tmp);
	if (!str)
		return (0);
	return (str);
}

char	*read_file(int fd, char *str, int *ret)
{
	char			*buf;
	char			*tmp;

	buf = malloc(sizeof(*buf) * (BUFFER_SIZE) + 1);
	if (!buf)
	{
		free(str);
		return (0);
	}
	*ret = read(fd, buf, BUFFER_SIZE);
	buf[*ret] = '\0';
	tmp = str;
	str = ft_strjoin(str, buf);
	free(tmp);
	free(buf);
	if (!str)
		return (0);
	return (str);
}

int		get_next_line(int fd, char **line)
{
	static char		*str[FD_SIZE];
	int				ret;

	if (read(fd, 0, 0) == -1 || !line || fd < 0 || BUFFER_SIZE < 1)
		return (-1);
	if (!str[fd])
		str[fd] = ft_strdup("");
	if (!str[fd])
		return (-1);
	if (ft_strchr(str[fd], '\n'))
	{
		str[fd] = get_that_line(str[fd], line);
		if (!str[fd])
			return (-1);
		return (1);
	}
	else
		str[fd] = read_file(fd, str[fd], &ret);
	if (!str[fd])
		return (-1);
	if (ret == 0)
		return (write_return(str[fd], line, 0));
	return (get_next_line(fd, line));
}
