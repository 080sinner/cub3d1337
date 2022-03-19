/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eozben <eozben@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 17:17:46 by fbindere          #+#    #+#             */
/*   Updated: 2022/03/19 18:21:27 by eozben           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	get_map_size(char *map_line, int *longest_line, int *line_count)
{	
	int	i;
	int	current_line;

	current_line = 0;
	*longest_line = 0;
	*line_count = 0;
	i = 0;
	while (map_line[i])
	{
		if (map_line[i] == '\n')
		{
			*line_count += 1;
			if (current_line > *longest_line)
				*longest_line = current_line;
			current_line = 0;
		}
		i++;
		current_line++;
	}
	if (map_line[i - 1] != '\n' && !map_line[i])
		*line_count += 1;
}

int	get_line_length(char *line)
{	
	int	i;

	i = 0;
	while (line[i])
	{
		if (line[i] == '\n')
			return (i);
		i++;
	}
	return (i - 1);
}

void	create_map_array(t_map *map, char *map_line)
{
	int	i;
	int	j;

	get_map_size(map_line, &map->map_length, &map->map_height);
	map->map = ft_calloc(map->map_height + 1, map->map_length);
	if (!map->map)
		map_error(map, map_line, "Allocating map");
	i = 0;
	j = 0;
	while (map_line[i])
	{
		map->map[j] = ft_substr(map_line, i, get_line_length(&map_line[i]));
		i += get_line_length(&map_line[i]) + 1;
		j++;
	}
	printf("map height : %d\n", map->map_height);
	j = 0;
	while (map->map[j])
		printf("map: %s\n", map->map[j++]);
}

char	*get_next_written_line(int fd)
{
	char	*line;

	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			return (NULL);
		if (!ft_is_empty_line(line))
			return (line);
		free(line);
	}	
}

int	is_eof(int fd)
{
	char	*line;

	line = get_next_written_line(fd);
	if (line)
	{
		free(line);
		return (0);
	}
	return (1);
}

char *append_mapline(t_map *map, char *map_line, char *line)
{
	char	*tmp;

	tmp = map_line;
	map_line = ft_strjoin(map_line, line);
	free(tmp);
	if (!map_line)
		map_error(map, line, "Allocating map");
	return (map_line);
}

void read_map(t_map *map)
{
	char	*line;
	char	*map_line;

	map_line = get_next_written_line(map->map_fd);
	if (!map_line)
		map_error(map, NULL, "No map specified");
	while (1)
	{
		line = get_next_line(map->map_fd);
		if (!line)
			break ;
		if (ft_is_empty_line(line))
		{
			free(line);
			if (is_eof(map->map_fd))
				break ;
			map_error(map, map_line, "Empty line in map");
		}
		map_line = append_mapline(map, map_line, line);
		free(line);
	}	
	create_map_array(map, map_line);
}

int	is_empty_tile(char tile)
{
	if (tile == ' ' || tile == '\0')
		return (1);
	return (0);
}

void	check_valid_zero(t_map *map, int x, int y)
{
	int i;
	int j;

	i = -1;
	while (i < 2)	
	{
		j = -1;
		while (j < 2)
		{
			if (is_empty_tile(map->map[y + i][x + j]))
				map_error(map, NULL, "Invalid map configuration");
			j++;
		}
		i++;
	}
}

int	is_player(char tile)
{	
	if (tile == 'N')
		return (1);
	if (tile == 'E')
		return (1);
	if (tile == 'S')
		return (1);
	if (tile == 'W')
		return (1);
	return (0);
}

int	check_border(t_map *map)
{
	int	y;

	if (ft_strchr(map->map[0], '0'))
		return (0);
	if (ft_strchr(map->map[map->map_height - 1], '0'))
		return (0);
	y = 1;
	while (y < map->map_height - 1)
	{
		if (map->map[y][0] == '0')
			return (0);
		y++;
	}
	return (1);
}

void	check_map_validity(t_map *map)
{
	int y;
	int x;
	int playercount;

	check_border(map);
	playercount = 0;
	y = 1;
	while(y < map->map_height - 1)
	{
		x = 1;
		while (map->map[y][x])
		{
			if (map->map[y][x] == '0')
				check_valid_zero(map, x, y);
			else if (is_player(map->map[y][x]))
				playercount++;
			else if (map->map[y][x] != '1' && !ft_is_whitespace(map->map[y][x]))
				map_error(map, NULL, "invalid map character");
			x++;
		}
	y++;
	}
	if (playercount != 1)
		map_error(map, NULL, "Invalid amount of players");
}
