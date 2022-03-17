/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 17:17:46 by fbindere          #+#    #+#             */
/*   Updated: 2022/03/17 16:49:07 by fbindere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"


void	create_map_array()
{
	
}

int	read_map(t_map *map)
{
	char	*line;
	char	*tmp;
	char	*map_line;

	map_line = ft_strdup("");
	if (!map_line)
		map_error(map, NULL, "Allocating map");
	while (1)
	{
		line = get_next_line(map->map_fd);
		if (!line)
			break ;
		if (!ft_is_empty_line(line))
		{
			tmp = map_line;
			map_line = ft_strjoin(map_line, line);
			free(tmp);
			if (!map_line)
				map_error(map, line, "Allocating map");
		}
		free(line);
	}
	map->map = create_map_array(map_line);
	
	// map->map = ft_split(map_line, '\n');
	// if (!map->map)
	// 	map_error(map, map_line, "Allocating map");
	// free(map_line);
	// return (0);
}