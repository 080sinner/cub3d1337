/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eozben <eozben@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 22:47:18 by eozben            #+#    #+#             */
/*   Updated: 2022/03/15 22:48:26 by eozben           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	read_cub_file(t_map *map, char **argv)
{	
	map->map_fd = open(argv[1], O_RDWR);
	if (map->map_fd == ERROR)
	{
		printf("Error \nBad .cub file\n");
		exit(EXIT_FAILURE);
	}
	read_wall_texture(map);
	read_floor_ceiling_colour(map);
}
