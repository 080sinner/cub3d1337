/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprites.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/21 18:35:51 by fbindere          #+#    #+#             */
/*   Updated: 2022/04/27 19:17:42 by fbindere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	sort_sprites(t_cub *cub)
{
	int		x;
	int		y;
	t_spr	temp;

	x = 0;
	while (x < cub->map.spr_count - 1)
	{
		y = 0;
		while (y < cub->map.spr_count - x - 1)
		{
			if (cub->map.map_spr[y].distance < cub->map.map_spr[y + 1].distance)
			{
				temp = cub->map.map_spr[y];
				cub->map.map_spr[y] = cub->map.map_spr[y + 1];
				cub->map.map_spr[y + 1] = temp;
			}
			y++;
		}
		x++;
	}
}

void	get_distance_sprites(t_cub *cub)
{
	int	i;

	i = 0;
	while (i < cub->map.spr_count)
	{
		cub->map.map_spr[i].distance
			= (cub->player.pos.x - cub->map.map_spr[i].coord.x)
			* (cub->player.pos.x - cub->map.map_spr[i].coord.x)
			+ (cub->player.pos.y - cub->map.map_spr[i].coord.y)
			* (cub->player.pos.y - cub->map.map_spr[i].coord.y);
		i++;
	}
}

void	get_sprite_pos(t_cub *cub, t_spr *sprite, int i)
{
	t_point	spr_pos;
	double	inv_det;

	spr_pos.x = cub->map.map_spr[i].coord.x - cub->player.pos.x;
	spr_pos.y = cub->map.map_spr[i].coord.y - cub->player.pos.y;
	inv_det = 1.0 / (cub->camera.plane.x * cub->player.dir.y
			- cub->player.dir.x * cub->camera.plane.y);
	sprite->transf.x = inv_det * (cub->player.dir.y * spr_pos.x
			- cub->player.dir.x * spr_pos.y);
	sprite->transf.y = inv_det * (-cub->camera.plane.y * spr_pos.x
			+ cub->camera.plane.x * spr_pos.y);
	sprite->scr_x = (int)((WIN_WIDTH / 2)
			* (1 + sprite->transf.x / sprite->transf.y));
}

void	get_sprite_measures(t_spr *sprite)
{
	sprite->height = abs((int)(WIN_HEIGHT / sprite->transf.y));
	sprite->start_y = -sprite->height / 2 + WIN_HEIGHT / 2;
	if (sprite->start_y < 0)
		sprite->start_y = 0;
	sprite->end_y = sprite->height / 2 + WIN_HEIGHT / 2;
	if (sprite->end_y >= WIN_HEIGHT)
		sprite->end_y = WIN_HEIGHT - 1;
	sprite->width = abs((int)(WIN_HEIGHT / sprite->transf.y));
	sprite->start_x = -sprite->width / 2 + sprite->scr_x;
	if (sprite->start_x < 0)
		sprite->start_x = 0;
	sprite->end_x = sprite->width / 2 + sprite->scr_x;
	if (sprite->end_x >= WIN_WIDTH)
		sprite->end_x = WIN_WIDTH - 1;
}

void	draw_sprite(t_cub *cub, t_spr *sprite, t_ray *ray, int i)
{
	int				stripe;
	int				y;
	int				tex_x;
	int				tex_y;
	int				d;
	unsigned int	color;

	stripe = sprite->start_x;
	while (stripe < sprite->end_x)
	{	
		tex_x = (int)(256 * (stripe - (-sprite->width / 2 + sprite->scr_x))
				* cub->map.sprites[cub->map.map_spr[i].type].width
				/ sprite->width) / 256;
		if (sprite->transf.y > 0 && stripe > 0
			&& stripe < WIN_WIDTH && sprite->transf.y
			< ray->perp_wall_dist[stripe])
		{
			y = sprite->start_y;
			while (y < sprite->end_y)
			{
				d = y * 256 - WIN_HEIGHT * 128 + sprite->height * 128;
				tex_y = ((d * cub->map.sprites[cub->map.map_spr[i].type].height)
						/ sprite->height) / 256;
				color = mlx_pixel_read(&cub->map.sprites
					[cub->map.map_spr[i].type], tex_x, tex_y);
				if ((color & 0x00FFFFFF) != 0)
					ft_mlx_pixel_put(&cub->img, stripe, y, color);
				y++;
			}
		}
		stripe++;
	}
}

void	cast_sprites(t_cub *cub, t_ray *ray)
{
	int		i;
	t_spr	sprite;

	get_distance_sprites(cub);
	sort_sprites(cub);
	i = 0;
	while (i < cub->map.spr_count)
	{
		get_sprite_pos(cub, &sprite, i);
		get_sprite_measures(&sprite);
		draw_sprite(cub, &sprite, ray, i);
		i++;
	}
}
