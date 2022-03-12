/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eozben <eozben@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/12 19:00:58 by eozben            #+#    #+#             */
/*   Updated: 2022/03/12 22:20:22 by eozben           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	ft_iswhitespace(char c)
{
	if (c == ' ' || c == '\t' || c == '\r')
		return (1);
	else
		return (0);
}

int	ft_is_empty_line(char *s)
{
	int	i;

	i = 0;
	while (ft_iswhitespace(s[i]))
		i++;
	if (s[i] == '\n')
		return (1);
	return (0);
}

int	ft_check_pattern(char *str)
{
	int	i;

	i = 0;
	while (ft_iswhitespace(str[i]))
		i++;
	if (str[i] != '1')
		return (0);
	if (str[ft_strlen(str) - 2] != '1') 
		return (0);
	return (1);
}


/*keine ahnung ob das parsing so stimmt, wahrscheinlich noch nicht
 aber kein bock mehr jetzt */

int	check_valid_map(int fd, char *str)
{	
	(void)fd;
	if (!ft_check_pattern(str))
		printf("   wrong pattern\n");
	else
		printf("   good\n");
	return (1);
}

int	read_map(char **map_adr)
{
	(void)map_adr;
	char	*str;
	int		line_count;
	int fd = open("./map.cub", O_RDWR);

	line_count = 0;
	while (1)
	{
		str = get_next_line(fd);
		if (!str)
			break ;
		if (!ft_is_empty_line(str))
			line_count++;
		if (line_count > 6)
		{
			char *tmp = ft_strtrim(str, "\n");
			printf("read line : %s", tmp);
			free(tmp);
		}
		else
			printf("read line : %s", str);
		if (line_count > 6)
			check_valid_map(fd, str);
		free(str);
	}
	return (0);
}


int main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;
	read_map(NULL);
}