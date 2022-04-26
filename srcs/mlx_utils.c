/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eozben <eozben@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 20:58:25 by eozben            #+#    #+#             */
/*   Updated: 2022/04/26 23:12:29 by eozben           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	ft_mlx_pixel_put(t_img *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int *)dst = color;
}

unsigned int	mlx_pixel_read(t_img *data, int x, int y)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	return (*(unsigned int *)dst);
}

int	close_win(t_cub *cub)
{
	mlx_destroy_image(cub->win.mlx, cub->img.img);
	mlx_destroy_window(cub->win.mlx, cub->win.mlx_win);
	free_map(cub);
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
		move_right(cub);
	else if (keycode == A_KEY)
		move_left(cub);
	else if (keycode == LEFT_KEY)
		turn_left(cub);
	else if (keycode == RIGHT_KEY)
		turn_right(cub);
	cub3d(cub);
	return (0);
}

int	mouse_move(int x, int y, t_cub *cub)
{
	static int	old_x;

	if (old_x == 0)
		old_x = WIN_WIDTH / 2;
	if (cub->camera.pressed_mb)
	{
		if (x <= WIN_WIDTH && x >= 0 && y <= WIN_HEIGHT && y >= 0)
		{
			if (x < old_x)
				turn_left(cub);
			else
				turn_right(cub);
		}
		old_x = x;
		cub3d(cub); // kann man das nicht anders aufrufen?
	}
	return (0);
}

int	register_mouseclick(int button, int x, int y, t_cub *cub)
{
	(void)x;
	(void)y;
	if (button == 1)
		cub->camera.pressed_mb = 1;
	return (0);
}

int unregister_mouseclick(int button, int x, int y, t_cub *cub)
{
	(void)x;
	(void)y;
	(void)cub;
	if (button == 1)
		cub->camera.pressed_mb = 0;
	return (0);
}

int	mlx_hooks(t_cub *cub)
{
	mlx_hook(cub->win.mlx_win, 17, 0, close_win, cub);
	mlx_hook(cub->win.mlx_win, ON_KEYDOWN, 0, key_hooks, cub);
	mlx_hook(cub->win.mlx_win, 4, 0, register_mouseclick, cub);
	mlx_hook(cub->win.mlx_win, 5, 0, unregister_mouseclick, cub);
	mlx_hook(cub->win.mlx_win, 6, 0, mouse_move, cub);
	return (0);
}

int	init_win_img(t_cub *cub)
{
	cub->win.mlx = mlx_init();
	cub->win.mlx_win = mlx_new_window(cub->win.mlx, WIN_WIDTH, WIN_HEIGHT,
			"cub3d1337");
	cub->img.img = mlx_new_image(cub->win.mlx, WIN_WIDTH, WIN_HEIGHT);
	cub->img.addr = mlx_get_data_addr(cub->img.img, &cub->img.bits_per_pixel,
			&cub->img.line_length, &cub->img.endian);
	return (0);
}
