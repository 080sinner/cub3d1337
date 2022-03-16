/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eozben <eozben@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/12 19:00:58 by eozben            #+#    #+#             */
/*   Updated: 2022/03/16 17:19:40 by eozben           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	init_map(t_map *map)
{
	map->map_fd = 0;
	map->f_color = 0;
	map->c_color = 0;
	map->ea_path = NULL;
	map->no_path = NULL;
	map->so_path = NULL;
	map->we_path = NULL;
	map->map = NULL;
}

void	ft_mlx_pixel_put(t_img *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int *)dst = color;
}

int	init_window(t_win *win, t_img *img)
{
	win->mlx = mlx_init();
	win->mlx_win = mlx_new_window(win->mlx, WIN_WIDTH, WIN_HEIGHT, "cub3d1337");
	img->img = mlx_new_image(win->mlx, WIN_WIDTH, WIN_HEIGHT);
	img->addr = mlx_get_data_addr(img->img, &img->bits_per_pixel,
			&img->line_length, &img->endian);
	return (0);
}

int	main(int argc, char *argv[])
{
	t_map	map;
	t_win	win;
	t_img	img;

	if (argc != 2)
		return (printf("Error \nInvalid amount of arguments\n"));
	init_map(&map);
	read_cub_file(&map, argv);
	init_window(&win, &img);
	printf("no texture: %s\n", map.no_path);
	printf("so texture: %s\n", map.so_path);
	printf("we texture: %s\n", map.we_path);
	printf("ea texture: %s\n", map.ea_path);
	printf("f_colour: %d\n", map.f_color);
	printf("c_colour: %d\n", map.c_color);
	mlx_loop(win.mlx);
}
