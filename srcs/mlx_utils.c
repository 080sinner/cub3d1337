/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 20:58:25 by eozben            #+#    #+#             */
/*   Updated: 2022/04/07 22:26:58 by fbindere         ###   ########.fr       */
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
	if (keycode == W_KEY)
		move_forward(cub);
	else if (keycode == S_KEY)
		move_backward(cub);
	else if (keycode == D_KEY)
		turn_right(cub);
	else if (keycode == A_KEY)
		turn_left(cub);
	cub3d(cub);
	return (0);
}

int	mlx_hooks(t_cub *cub)
{
	mlx_hook(cub->win.mlx_win, 17, 0, close_win, cub);
	mlx_key_hook(cub->win.mlx_win, key_hooks, cub);
	return (0);
}

int	init_win_img(t_cub *cub)
{
	cub->win.mlx = mlx_init();
	cub->win.mlx_win = mlx_new_window(cub->win.mlx, WIN_WIDTH, WIN_HEIGHT, "cub3d1337");
	cub->img.img = mlx_new_image(cub->win.mlx, WIN_WIDTH, WIN_HEIGHT);
	cub->img.addr = mlx_get_data_addr(cub->img.img, &cub->img.bits_per_pixel,
			&cub->img.line_length, &cub->img.endian);
	return (0);
}
