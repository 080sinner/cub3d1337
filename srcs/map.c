/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 17:17:46 by fbindere          #+#    #+#             */
/*   Updated: 2022/03/17 20:49:31 by fbindere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"


void	get_map_size(char *map_line, int *longest_line, int *line_count)
{	
	int i;
	int current_line;
	
	current_line = 0;
	*longest_line = 0;
	*line_count = 0;
	i = 0;
	while(map_line[i])
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
	int i;

	i = 0;
	while(line[i])
	{
		if (line[i] == '\n')
			return (i);
		i++;
	}
	return (i - 1);	
}

void	create_map_array(t_map *map, char *map_line)
{
	int longest_line;
	int	line_count;
	int i;
	int j;
	
	get_map_size(map_line, &longest_line, &line_count);
	map->map = ft_calloc(line_count + 1, longest_line);
	if (!map->map)
		map_error(map, map_line, "Allocating map");
	i = 0;
	j = 0;
	while(map_line[i])
	{
		map->map[j] = ft_substr(map_line, i, get_line_length(&map_line[i]));
		i += get_line_length(&map_line[i]) + 1;
		j++;
	}
	j = 0;
	while(map->map[j])
		printf("first line %s\n", map->map[j++]);

}

char *get_next_written_line(int fd)
{
	char *line;

	while(1)
	{
		line = get_next_line(fd);
		if (!line)
			return (NULL);
		if (!ft_is_empty_line(line))
			return (line);
	}	
}

char *append_mapline(t_map *map, char *map_line, char *line)
{
	char *tmp;
	char *ret_str;

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

	map_line = get_next_written_line(map->map_fd);
	if (!map_line)
		map_error(map, NULL, "no map specified");

	while (1)
	{
		line = get_next_line(map->map_fd);
		if (!line)
			break;
		if (ft_is_empty_line(line))
		{
			free(line);
			line = get_next_written_line(map->map_fd);
			if (line)
			{
				free(line);
				map_error(map, map_line, "empty line in map");
			}
			break;
		}
		append_mapline(map, map_line, line);
		free(line);
	}
	create_map_array(map, map_line);
}