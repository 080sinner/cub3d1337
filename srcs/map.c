/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   map.c											  :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: fbindere <fbindere@student.42.fr>		  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2022/03/16 17:17:46 by fbindere		  #+#	#+#			 */
/*   Updated: 2022/04/07 20:52:50 by fbindere		 ###   ########.fr	   */
/*																			*/
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
	int i;

	i = 0;
	while (line[i])
	{
		if (line[i] == '\n')
			return (i);
		i++;
	}
	return (i - 1);
}

void	create_map_array(t_cub *cub, char *map_line)
{
	int	i;
	int	j;

	get_map_size(map_line, &cub->map.map_length, &cub->map.map_height);
	cub->map.map = ft_calloc(cub->map.map_height + 1, cub->map.map_length);
	if (!cub->map.map)
		map_error(cub, map_line, "Allocating map");
	i = 0;
	j = 0;
	while (map_line[i])
	{
		cub->map.map[j] = ft_substr(map_line, i,
				get_line_length(&map_line[i]) + 1);
		if (cub->map.map[j][ft_strlen(cub->map.map[j]) - 1] == '\n')
			cub->map.map[j][ft_strlen(cub->map.map[j]) - 1] = '\0';
		i += get_line_length(&map_line[i]) + 1;
		j++;
	}
	printf("map height : %d\n", cub->map.map_height);
	j = 0;
	while (cub->map.map[j])
		printf("map: %s\n", cub->map.map[j++]);
	free(map_line);
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

char	*append_mapline(t_cub *cub, char *map_line, char *line)
{
	char	*tmp;

	tmp = map_line;
	map_line = ft_strjoin(map_line, line);
	free(tmp);
	if (!map_line)
		map_error(cub, line, "Allocating map");
	return (map_line);
}

void	read_map(t_cub *cub)
{
	char	*line;
	char	*map_line;

	map_line = get_next_written_line(cub->map.fd);
	if (!map_line)
		map_error(cub, NULL, "No map specified");
	while (1)
	{
		line = get_next_line(cub->map.fd);
		if (!line)
			break ;
		if (ft_is_empty_line(line))
		{
			free(line);
			if (is_eof(cub->map.fd))
				break ;
			map_error(cub, map_line, "Empty line in map");
		}
		map_line = append_mapline(cub, map_line, line);
		free(line);
	}
	create_map_array(cub, map_line);
}

int is_empty_tile(char tile)
{
	if (tile == ' ' || tile == '\0')
		return (1);
	return (0);
}

void	check_valid_zero(t_cub *cub, int x, int y)
{
	int	i;
	int	j;

	i = -1;
	while (i < 2)
	{
		j = -1;
		while (j < 2)
		{
			if (is_empty_tile(cub->map.map[y + i][x + j]))
				map_error(cub, NULL, "Invalid map configuration");
			j++;
		}
		i++;
	}
}

int is_player(char tile)
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

int check_x_border(char *s, int i)
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

int check_border(t_cub *cub)
{
	int	y;

	if (!check_x_border(cub->map.map[0], skip_whitespaces(cub->map.map[0])))
		map_error(cub, NULL, "Invalid vertical border");
	if (!check_x_border(cub->map.map[cub->map.map_height - 1],
			skip_whitespaces(cub->map.map[cub->map.map_height - 1])))
		map_error(cub, NULL, "Invalid vertical border");
	y = 1;
	while (y < cub->map.map_height - 1)
	{
		if (cub->map.map[y][skip_whitespaces(cub->map.map[y])] != '1')
			map_error(cub, NULL, "Invalid border");
		y++;
	}
	return (1);
}

int	player_values(t_map *map, t_player *player, int x, int y)
{
	static int	playercount;

	if (x == 0 || y == 0)
		return (playercount);
	player->pos.x = x + 0.5;
	player->pos.y = y + 0.5;
	if (map->map[y][x] == 'N')
		map->p_dir = NORTH;
	else if (map->map[y][x] == 'E')
		map->p_dir = EAST;
	else if (map->map[y][x] == 'S')
		map->p_dir = SOUTH;
	else if (map->map[y][x] == 'W')
		map->p_dir = WEST;
	playercount++;
	return (playercount);
}

int	is_sprite(char c)
{
	if (c == 'D' || c == 'L' || c == 'B' || c == 'P')
		return (1);
	return (0);
}

void	parse_sprite(char c, t_cub *cub, int x, int y)
{
	if (c == 'D')
	{
		if (cub->map.door_count > 3)
			map_error(cub, NULL, "More than 3 doors");
		cub->map.map_doors[cub->map.door_count].x = x;
		cub->map.map_doors[cub->map.door_count].y = y;
		cub->map.door_count += 1;
	}
	else if (c == 'L' || c == 'B' || c == 'P')
	{
		if (cub->map.spr_count > 10)
			map_error(cub, NULL, "More than 10 doors");
		if (c == 'L')
			cub->map.map_spr[cub->map.spr_count].type = LAMP;
		else if (c == 'B')
			cub->map.map_spr[cub->map.spr_count].type = BARREL;
		else if (c == 'P')
			cub->map.map_spr[cub->map.spr_count].type = PILLAR;
		cub->map.map_spr[cub->map.spr_count].coord.x = x;
		cub->map.map_spr[cub->map.spr_count].coord.y = y;
		cub->map.spr_count += 1;
	}
}

void	check_map_validity(t_cub *cub, t_player *player)
{
	int	y;
	int	x;

	check_border(cub);
	y = 1;
	while (y < cub->map.map_height - 1)
	{
		x = 1;
		while (cub->map.map[y][x])
		{
			if (cub->map.map[y][x] == '0')
				check_valid_zero(cub, x, y);
			else if (is_player(cub->map.map[y][x]))
				player_values(&cub->map, player, x, y);
			else if (is_sprite(cub->map.map[y][x]))
				parse_sprite(cub->map.map[y][x], cub, x, y);
			else if (cub->map.map[y][x] != '1'
				&& !ft_is_whitespace(cub->map.map[y][x]))
				map_error(cub, NULL, "invalid map character");
			x++;
		}
		y++;
	}
	if (player_values(&cub->map, player, 0, 0) != 1)
		map_error(cub, NULL, "Invalid amount of players");
}
