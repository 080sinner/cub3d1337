/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 20:58:25 by eozben            #+#    #+#             */
/*   Updated: 2022/04/27 00:02:34 by fbindere         ###   ########.fr       */
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

int	key_hooks(t_cub *cub)
{
	if (cub->camera.pressed_keys[0])
		move_forward(cub);
	if (cub->camera.pressed_keys[1])
		move_backward(cub);
	if (cub->camera.pressed_keys[2])
		move_right(cub);
	if (cub->camera.pressed_keys[3])
		move_left(cub);
	if (cub->camera.pressed_keys[4])
		turn_left(cub);
	if (cub->camera.pressed_keys[5])
		turn_right(cub);
	return (0);
}

int	press_keys(int keycode, t_cub *cub)
{
	if (keycode == ESC_KEY)
		close_win(cub);
	if (keycode == W_KEY)
		cub->camera.pressed_keys[0] = 1;
	else if (keycode == S_KEY)
		cub->camera.pressed_keys[1] = 1;
	else if (keycode == D_KEY)
		cub->camera.pressed_keys[2] = 1;
	else if (keycode == A_KEY)
		cub->camera.pressed_keys[3] = 1;
	else if (keycode == LEFT_KEY)
		cub->camera.pressed_keys[4] = 1;
	else if (keycode == RIGHT_KEY)
		cub->camera.pressed_keys[5] = 1;
	return (0);
}

int	mouse_move(int x, int y, t_cub *cub)
{
	static int	old_x;

	if (cub->camera.pressed_mb)
	{
		if (x <= WIN_WIDTH && x >= 0 && y <= WIN_HEIGHT && y >= 0)
		{
			if (x < old_x)
				turn_left(cub);
			else if (x > old_x)
				turn_right(cub);
		}
	}
	old_x = x;
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

int	release_keys(int keycode, t_cub *cub)
{
	if (keycode == W_KEY)
		cub->camera.pressed_keys[0] = 0;
	else if (keycode == S_KEY)
		cub->camera.pressed_keys[1] = 0;
	else if (keycode == D_KEY)
		cub->camera.pressed_keys[2] = 0;
	else if (keycode == A_KEY)
		cub->camera.pressed_keys[3] = 0;
	else if (keycode == LEFT_KEY)
		cub->camera.pressed_keys[4] = 0;
	else if (keycode == RIGHT_KEY)
		cub->camera.pressed_keys[5] = 0;
	return (0);
}

int	mlx_hooks(t_cub *cub)
{
	mlx_hook(cub->win.mlx_win, 17, 0, close_win, cub);
	mlx_hook(cub->win.mlx_win, ON_KEYDOWN, 0, press_keys, cub);
	mlx_hook(cub->win.mlx_win, 3, 0, release_keys, cub);
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
