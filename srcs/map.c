/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 17:17:46 by fbindere          #+#    #+#             */
/*   Updated: 2022/03/18 22:02:03 by fbindere         ###   ########.fr       */
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
	j = 0;
	while (map->map[j])
		printf("first line %s\n", map->map[j++]);

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

int	check_valid_tile(t_map *map, int x, int y)
{
	if (is_empty_tile(map->map[y][x + 1]))
		return (0);
	if (is_empty_tile(map->map[y][x - 1]))
		return (0);
	if (is_empty_tile(map->map[y + 1][x]))
		return (0);
	if (is_empty_tile(map->map[y - 1][x]))
		return (0);
	if (is_empty_tile(map->map[y + 1][x + 1]))
		return (0);
	if (is_empty_tile(map->map[y + 1][x - 1]))
		return (0);
	if (is_empty_tile(map->map[y - 1][x + 1]))
		return (0);
	if (is_empty_tile(map->map[y - 1][x - 1]))
		return (0);
	return (1);
}

int	is_player(char tile)
{
	static int player_count;
	
	if (tile == 'N')
		return (player_count++);
	if (tile == 'E')
		return (player_count++);
	if (tile == 'S')
		return (player_count++);
	if (tile == 'W')
		return (player_count++);
	return (0);
}

int	check_border(t_map *map)
{
	int	x;
	int	y;
	int	playercount;

	if (ft_strchr(map->map[0], '0'))
		return (0);
	if (ft_strchr(map->map[map->map_height - 1], '0'))
		return (0);
	y = 0;
	while(y < map->map_height)
	{
		x = 0;
		while (map->map[y][x])
		{
			if (map->map[y][x] == '0' && !check_valid_tile(map, x, y))
				map_error(map, NULL, "invalid map configuration");
			else if (is_player(map->map[y][x]) > 1)
				map_error(map, NULL, "invalid player count");
			else if (!ft_is_whitespace(map->map[y][x]) && map->map[y][x] != '1')
				map_error(map, NULL, "invalid map character");
			x++;
			printf("check\n");
		}
	y++;
	}
	if (playercount != 1)
		map_error(map, NULL, "Invalid player count");
}
