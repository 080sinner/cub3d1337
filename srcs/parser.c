/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 22:47:18 by eozben            #+#    #+#             */
/*   Updated: 2022/03/17 21:16:54 by fbindere         ###   ########.fr       */
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
	read_textures(map);
	read_map(map);
	check_map_validity(map);
}
