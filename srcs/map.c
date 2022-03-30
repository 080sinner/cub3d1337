/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 17:17:46 by fbindere          #+#    #+#             */
/*   Updated: 2022/03/30 21:30:59 by fbindere         ###   ########.fr       */
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

char	*append_mapline(t_map *map, char *map_line, char *line)
{
	char	*tmp;

	tmp = map_line;
	map_line = ft_strjoin(map_line, line);
	free(tmp);
	if (!map_line)
		map_error(map, line, "Allocating map");
	return (map_line);
}

void	read_map(t_map *map)
{
	char	*line;
	char	*map_line;

	map_line = get_next_written_line(map->fd);
	if (!map_line)
		map_error(map, NULL, "No map specified");
	while (1)
	{
		line = get_next_line(map->fd);
		if (!line)
			break ;
		if (ft_is_empty_line(line))
		{
			free(line);
			if (is_eof(map->fd))
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
	int	i;
	int	j;

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

int	check_x_border(char *s, int i)
{
	while (s[i])
	{
		if (ft_is_whitespace(s[i]))
		{
			i++;
			continue ;
		}
		if (s[i] != '1')
			return (0);
		i++;
	}
	return (1);
}

int	check_border(t_map *map)
{
	int	y;

	if (!check_x_border(map->map[0], skip_whitespaces(map->map[0])))
		map_error(map, NULL, "Invalid vertical border");
	if (!check_x_border(map->map[map->map_height - 1],
			skip_whitespaces(map->map[map->map_height - 1])))
		map_error(map, NULL, "Invalid vertical border");
	y = 1;
	while (y < map->map_height - 1)
	{
		if (map->map[y][skip_whitespaces(map->map[y])] != '1')
			map_error(map, NULL, "Invalid border");
		y++;
	}
	return (1);
}

int	player_values(t_map *map, t_player *player, int x, int y)
{
	static int playercount;
	
	if (x == 0 || y == 0)
		return (playercount);
	player->pos_x = (double)x;
	player->pos_y = (double)y;
	if (map->map[y][x] == 'N')
		player->dir_y = 1;
	else if (map->map[y][x] == 'E')
		player->dir_x = 1;
	else if (map->map[y][x] == 'S')
		player->dir_y = -1;
	else if (map->map[y][x] == 'W')
		player->dir_x = -1;
	playercount++;
	return(playercount);
}

void	check_map_validity(t_map *map, t_player *player)
{
	int	y;
	int	x;

	check_border(map);
	y = 1;
	while (y < map->map_height - 1)
	{
		x = 1;
		while (map->map[y][x])
		{
			if (map->map[y][x] == '0')
				check_valid_zero(map, x, y);
			else if (is_player(map->map[y][x]))
				player_values(map, player, x, y);
			else if (map->map[y][x] != '1' && !ft_is_whitespace(map->map[y][x]))
				map_error(map, NULL, "invalid map character");
			x++;
		}
	y++;
	}
	if (player_values(map, player, 0, 0) != 1)
		map_error(map, NULL, "Invalid amount of players");
}
