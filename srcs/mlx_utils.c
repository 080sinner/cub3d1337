/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 20:58:25 by eozben            #+#    #+#             */
/*   Updated: 2022/03/30 22:16:58 by fbindere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	ft_mlx_pixel_put(t_img *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int *)dst = color;
}

int	close_win(t_cub *cub)
{
	mlx_destroy_image(cub->win.mlx, cub->img.img);
	mlx_destroy_window(cub->win.mlx, cub->win.mlx_win);
	//free
	exit(EXIT_SUCCESS);
	return (0);
}

static int	key_hooks(int keycode, t_cub *cub)
{
	if (keycode == ESC_KEY)
		close_win(cub);
	return (0);
}

int	mlx_hooks(t_cub *cub)
{
	mlx_hook(cub->win.mlx_win, 17, 0, close_win, cub);
	mlx_key_hook(cub->win.mlx_win, key_hooks, cub);
	return (0);
}

int	init_win_img(t_win *win, t_img *img)
{
	win->mlx = mlx_init();
	win->mlx_win = mlx_new_window(win->mlx, WIN_WIDTH, WIN_HEIGHT, "cub3d1337");
	img->img = mlx_new_image(win->mlx, WIN_WIDTH, WIN_HEIGHT);
	img->addr = mlx_get_data_addr(img->img, &img->bits_per_pixel,
			&img->line_length, &img->endian);
	return (0);
}
