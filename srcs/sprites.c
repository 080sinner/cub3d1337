/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprites.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/21 18:35:51 by fbindere          #+#    #+#             */
/*   Updated: 2022/04/25 16:43:05 by fbindere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int numberSprites = 3;

void	example_sprites(t_cub *cub)
{
	cub->map.map_spr[0].coord.x = 20;
	cub->map.map_spr[0].coord.y = 5;
	cub->map.map_spr[0].type = 0;
	cub->map.map_spr[2].coord.x = 22;
	cub->map.map_spr[2].coord.y = 7;
	cub->map.map_spr[2].type = 2;
	cub->map.map_spr[1].coord.x = 24;
	cub->map.map_spr[1].coord.y = 8;
	cub->map.map_spr[1].type = 1;
}

void	sort_sprites(t_cub *cub)
{
	int x;
	int y;
	t_spr temp;

	x = 0;
    while(x < numberSprites - 1)
	{
        y = 0;        
        while(y < numberSprites - x - 1){

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
	int i;

	i = 0;
	while(i < numberSprites)
	{
		cub->map.map_spr[i].distance = 
			(cub->player.pos.x - cub->map.map_spr[i].coord.x) * 
			(cub->player.pos.x - cub->map.map_spr[i].coord.x) +
			(cub->player.pos.y - cub->map.map_spr[i].coord.y) *
			(cub->player.pos.y - cub->map.map_spr[i].coord.y);
		i++;
	}
}

void	get_sprite_pos(t_cub *cub, t_spr *sprite, int i)
{
	t_point	spr_pos;
	double	invDet;

	spr_pos.x = cub->map.map_spr[i].coord.x - cub->player.pos.x;
	spr_pos.y = cub->map.map_spr[i].coord.y - cub->player.pos.y;
	invDet = 1.0 / (cub->camera.plane.x * cub->player.dir.y -
		cub->player.dir.x * cub->camera.plane.y);
	sprite->transf.x = invDet * (cub->player.dir.y * spr_pos.x - 
		cub->player.dir.x * spr_pos.y);
	sprite->transf.y = invDet * (-cub->camera.plane.y * spr_pos.x + 
		cub->camera.plane.x * spr_pos.y);
	sprite->scrX = (int)((WIN_WIDTH / 2) * 
		(1 + sprite->transf.x / sprite->transf.y));
}

void	get_sprite_measures(t_spr *sprite)
{
	sprite->height = abs((int)(WIN_HEIGHT / sprite->transf.y));
	sprite->startY = - sprite->height / 2 + WIN_HEIGHT / 2;
	if (sprite->startY < 0)
		sprite->startY = 0;
	sprite->endY = sprite->height / 2 + WIN_HEIGHT / 2;
	if (sprite->endY >= WIN_HEIGHT)
		sprite->endY = WIN_HEIGHT - 1;
	sprite->width = abs((int) (WIN_HEIGHT / sprite->transf.y));
	sprite->startX = -sprite->width / 2 + sprite->scrX;
	if (sprite->startX < 0)
		sprite->startX = 0;
	sprite->endX = sprite->width / 2 + sprite->scrX;
	if (sprite->endX >= WIN_WIDTH)
		sprite->endX = WIN_WIDTH - 1;
}

void	draw_sprite(t_cub *cub, t_spr *sprite, t_ray *ray, int i)
{
	int	stripe;
	int y;
	int	texX;
	int texY;
	int	d;
	unsigned int color;

	stripe = sprite->startX;
	while (stripe < sprite->endX)
	{	
		texX = (int)(256 * (stripe - (-sprite->width / 2 + sprite->scrX)) * 
		cub->map.sprites[cub->map.map_spr[i].type].width / sprite->width) / 256;
		if (sprite->transf.y > 0 && stripe > 0 
		&& stripe < WIN_WIDTH && sprite->transf.y < ray->perpWallDist[stripe])
		{
			y = sprite->startY;
			while (y < sprite->endY)
			{
				d = y * 256 - WIN_HEIGHT * 128 + sprite->height * 128;
				texY = ((d * cub->map.sprites[cub->map.map_spr[i].type].height)
				/ sprite->height) / 256;
				color = mlx_pixel_read(&cub->map.sprites[cub->map.map_spr[i].type], texX, texY);
				if((color & 0x00FFFFFF) != 0)
					ft_mlx_pixel_put(&cub->img, stripe, y, color);
				y++;
			}
		} 
		stripe++;
	}
}

void	cast_sprites(t_cub *cub, t_ray *ray)
{
	int 	i;
	t_spr	sprite;

	example_sprites(cub);
	get_distance_sprites(cub);
	sort_sprites(cub);
	i = 0;
	while (i < numberSprites)
	{
		get_sprite_pos(cub, &sprite, i);
		get_sprite_measures(&sprite);
		draw_sprite(cub, &sprite, ray, i);
		i++;
	}
}