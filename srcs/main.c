/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/12 19:00:58 by eozben            #+#    #+#             */
/*   Updated: 2022/03/17 22:13:51 by fbindere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	init_map(t_map *map)
{
	map->map_fd = 0;
	map->f_color = 0;
	map->map_height = 0;
	map->map_length = 0;
	map->c_color = 0;
	map->ea_path = NULL;
	map->no_path = NULL;
	map->so_path = NULL;
	map->we_path = NULL;
	map->map = NULL;
}

int	main(int argc, char *argv[])
{
	t_map	map;
	t_win	win;

	if (argc != 2)
		return (printf("Error \nInvalid amount of arguments\n"));
	init_map(&map);
	read_cub_file(&map, argv);
	init_window(&win, &win.img, &map);
	printf("no texture: %s\n", map.no_path);
	printf("so texture: %s\n", map.so_path);
	printf("we texture: %s\n", map.we_path);
	printf("ea texture: %s\n", map.ea_path);
	printf("f_colour: %d\n", map.f_color);
	printf("c_colour: %d\n", map.c_color);
	mlx_hooks(&win);
	mlx_loop(win.mlx);
}
