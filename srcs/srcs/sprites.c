/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprites.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/21 18:35:51 by fbindere          #+#    #+#             */
/*   Updated: 2022/04/27 23:53:21 by fbindere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

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

t_img *select_sprite_text(t_cub *cub, int i)
{
	static unsigned int	j;
	t_img	*spr_img;

	if (cub->map.map_spr[i].type == BARREL)
	{
		spr_img = &(cub->map.enemy[j / 10]);
		j++;
		if (j == 50)
			j = 0;
	}
	else
		spr_img = &(cub->map.sprites[cub->map.map_spr[i].type]);
	return (spr_img);
}


void	draw_sprite(t_cub *cub, t_spr *sprite, t_ray *ray, int i)
{
	int				d;
	unsigned int	color;
	t_img			*spr_img;
	t_coord			tex;
	t_coord			pixel;

	pixel.x = sprite->start_x;
	spr_img = select_sprite_text(cub, i);
	while (pixel.x < sprite->end_x)
	{	
		tex.x = (int)(256 * (pixel.x - (-sprite->width / 2 + sprite->scr_x))
			* (*spr_img).width	/ sprite->width) / 256;
		if (sprite->transf.y > 0 && pixel.x > 0 && pixel.x < WIN_WIDTH 
			&& sprite->transf.y	< ray->perp_wall_dist[pixel.x])
		{
			pixel.y = sprite->start_y;
			while (pixel.y < sprite->end_y)
			{
				d = pixel.y * 256 - WIN_HEIGHT * 128 + sprite->height * 128;
				tex.y = ((d * (*spr_img).height) / sprite->height) / 256;
				color = mlx_pixel_read(&(*spr_img), tex.x, tex.y);
				// printf("color = %u\n", mlx_pixel_read(&(*spr_img), 0, 0));
				// exit(EXIT_FAILURE);
				if (color != 9961608)
					ft_mlx_pixel_put(&cub->img, pixel.x, pixel.y, color);
				pixel.y++;
			}
		}
		pixel.x++;
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