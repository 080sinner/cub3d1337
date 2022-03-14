/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/12 19:00:58 by eozben            #+#    #+#             */
/*   Updated: 2022/03/14 21:46:07 by fbindere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"


void	map_error(t_map *map, char *str, char *error_msg);

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

// int	check_valid_map(int fd, char *str)
// {	
// 	(void)fd;
// 	if (!ft_check_pattern_middle(str))
// 		printf("   wrong pattern\n");
// 	else
// 		printf("   good\n");
// 	return (1);
// }

int	skip_whitespaces(char *str)
{
	int i;

	i = 0;
	while (ft_iswhitespace(str[i]))
		i++;
	return (i);
}

void	read_path(t_map *map, char indentifier, char *str, int i)
{
	i += (2 + skip_whitespaces(&str[i]));
	if (indentifier == 'N')
	{
		map->no_path = ft_strdup(&str[i]);
		if (!map->no_path)
			map_error(map, str, "allocating texture path");
	}
	else if (indentifier == 'E')
	{
		map->ea_path = ft_strdup(&str[i]);
		if (!map->ea_path)
			map_error(map, str, "allocating texture path");
	}
	else if (indentifier == 'S')
	{
		map->so_path = ft_strdup(&str[i]);
		if (!map->so_path)
			map_error(map, str, "allocating texture path");
	}
	else if (indentifier == 'W')
	{
		map->we_path = ft_strdup(&str[i]);
		if (!map->we_path)
			map_error(map, str, "allocating texture path");
	}
}

void	check_texture_path(t_map *map, char *str)
{
	int	i;

	i = 0;
	i = skip_whitespaces(&str[i]);
	if (!ft_strncmp("NO", &str[i], 2))
		return (read_path(map, 'N', str, i));
	else if (!ft_strncmp("SO", &str[i], 2))
		return (read_path(map, 'S', str, i));
	else if (!ft_strncmp("WE", &str[i], 2))
		return (read_path(map, 'W', str, i));
	else if (!ft_strncmp("EA", &str[i], 2))
		return (read_path(map, 'E', str, i));
	map_error(map, str, "no valid type identifier");
}

int	handle_textures(t_map *map, int fd)
{
	char	*str;

	while (1)
	{
		str = get_next_line(fd);
		if (!str)
			break ;
		if (!ft_is_empty_line(str))
			check_texture_path(map, str);
		free(str);
	}
	return (0);
}

void	free_map(t_map *map)
{
	free(map->ea_path);
	free(map->no_path);
	free(map->so_path);
	free(map->we_path);
	free(map->map);
}

void	map_error(t_map *map, char *str, char *error_msg)
{
	free_map(map);
	free(str);
	printf("Error\n%s\n", error_msg);
	exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
	t_map	map;
	(void)argc;
	(void)argv;
	handle_textures(&map, open("./map.cub", O_RDWR));
}
