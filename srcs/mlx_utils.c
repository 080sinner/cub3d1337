/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eozben <eozben@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 20:58:25 by eozben            #+#    #+#             */
/*   Updated: 2022/03/16 21:00:17 by eozben           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	ft_mlx_pixel_put(t_img *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int *)dst = color;
}

int	close_win(t_win *win)
{
	mlx_destroy_image(win->mlx, win->img.img);
	mlx_destroy_window(win->mlx, win->mlx_win);
	if (win->map)
		free_map(win->map);
	exit(EXIT_SUCCESS);
	return (0);
}

static int	key_hooks(int keycode, t_win *win)
{
	if (keycode == ESC_KEY)
		close_win(win);
	return (0);
}

int	mlx_hooks(t_win *win)
{
	mlx_hook(win->mlx_win, 17, 0, close_win, win);
	mlx_key_hook(win->mlx_win, key_hooks, win);
	return (0);
}

int	init_window(t_win *win, t_img *img, t_map *map)
{
	win->map = map;
	win->mlx = mlx_init();
	win->mlx_win = mlx_new_window(win->mlx, WIN_WIDTH, WIN_HEIGHT, "cub3d1337");
	img->img = mlx_new_image(win->mlx, WIN_WIDTH, WIN_HEIGHT);
	img->addr = mlx_get_data_addr(img->img, &img->bits_per_pixel,
			&img->line_length, &img->endian);
	return (0);
}
