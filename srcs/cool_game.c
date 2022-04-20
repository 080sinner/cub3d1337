/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cool_game.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/06 16:41:45 by fbindere          #+#    #+#             */
/*   Updated: 2022/04/20 23:16:39 by fbindere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	set_camera_vector(t_cub *cub)
{
	cub->camera.plane.x = cub->player.dir.y;
	cub->camera.plane.y = cub->player.dir.x;
	cub->camera.plane.y *= -1;
}

void 	set_ray_dir_vector(t_cub *cub, t_ray *ray, int x)
{
	double cameraX;

	cameraX = 2 * x / (double)WIN_WIDTH - 1;
	ray->dir.x = cub->player.dir.x + cub->camera.plane.x * cameraX;
	ray->dir.y = cub->player.dir.y + cub->camera.plane.y * cameraX;
}

void	set_deltaDist(t_ray *ray)
{
	if (ray->dir.x == 0)
		ray->deltaDist.x = INFINITY;
	else
		ray->deltaDist.x = fabs(1 / ray->dir.x);
	if (ray->dir.y == 0)
		ray->deltaDist.y = INFINITY;
	else
		ray->deltaDist.y = fabs(1 / ray->dir.y);
}

void	set_sideDist(t_ray *ray, t_player *player)
{
	if (ray->dir.x < 0)
	{
		ray->sideDist.x = (player->pos.x - ray->mapX) * ray->deltaDist.x;
		ray->stepX = -1;
	}
	else
	{
		ray->sideDist.x = (ray->mapX + 1 - player->pos.x) * ray->deltaDist.x;
		ray->stepX = 1;
	}
	if (ray->dir.y < 0)
	{
		ray->sideDist.y = (player->pos.y - ray->mapY) * ray->deltaDist.y;
		ray->stepY = -1;
	}
	else
	{
		ray->sideDist.y = (ray->mapY + 1 - player->pos.y) * ray->deltaDist.y;
		ray->stepY = 1;
	}
}

void	perform_DDA(t_ray *ray, t_cub *cub, int x)
{
	int hit;

	hit = 0;
	while (hit == 0)
	{
		if (ray->sideDist.x < ray->sideDist.y)
		{
			ray->sideDist.x += ray->deltaDist.x;
			ray->mapX += ray->stepX;
			ray->hit = xSide;
		}
		else
		{
			ray->sideDist.y += ray->deltaDist.y;
			ray->mapY += ray->stepY;
			ray->hit = ySide;
		}
		if (cub->map.map[ray->mapY][ray->mapX] > '0' && 
			!is_player(cub->map.map[ray->mapY][ray->mapX]))
			hit = 1;
	}
	if (ray->hit == xSide)
		ray->perpWallDist[x] =  ray->sideDist.x - ray->deltaDist.x;
	else
		ray->perpWallDist[x] = ray->sideDist.y - ray->deltaDist.y;
}

void	get_texture_x(t_cub *cub, t_ray *ray, t_text *text, int x)
{
	double	wallX;

	if (ray->hit == xSide)
		wallX = cub->player.pos.y + ray->perpWallDist[x] * ray->dir.y;
	else
		wallX = cub->player.pos.x + ray->perpWallDist[x] * ray->dir.x;
	wallX -= floor(wallX);
	text->x = (int)(wallX * (double)cub->map.texture[text->dir].width);
	if (ray->hit ==  xSide && ray->dir.x > 0)
		text->x = cub->map.texture[text->dir].width - text->x - 1;
	if (ray->hit ==  ySide && ray->dir.y < 0)
		text->x = cub->map.texture[text->dir].width - text->x - 1;
}

void	get_line_values(t_dline *line, t_ray *ray, int x)
{
	line->height = (int)(WIN_HEIGHT / ray->perpWallDist[x]);
	line->start = -line->height / 2 + WIN_HEIGHT / 2;
	if (line->start < 0)
		line->start = 0;
	line->end = line->height / 2 + WIN_HEIGHT / 2;
	if (line->end >= WIN_HEIGHT)
		line->end = WIN_HEIGHT - 1;
}

void	get_text_values(t_cub *cub, t_dline *line, t_text *text)
{
	text->step = 1.0 * cub->map.texture[text->dir].height / line->height;
	text->pos = (line->start - WIN_HEIGHT / 2 + line->height / 2) * text->step;
}


void	get_text_type(t_ray *ray, t_text *text)
{
	if (ray->hit == ySide)
	{
		if (ray->dir.y >= 0)
			text->dir = NORTH;
		else
			text->dir = SOUTH;
	}
	if (ray->hit == xSide)
	{
		if (ray->dir.x >= 0)
			text->dir = EAST;
		else
			text->dir = WEST;
	}
}

void	draw_line(t_ray *ray, t_cub *cub, int x)
{
	t_dline			line;
	t_text			text;	
	unsigned int	color;

	get_line_values(&line, ray, x);
	get_text_type(ray, &text);
	get_text_values(cub, &line, &text);
	get_texture_x(cub, ray, &text, x);
	for (int y = line.start; y < line.end; y++)
	{
		text.y = (int)text.pos & (cub->map.texture[text.dir].height - 1);
		text.pos += text.step;
		color = mlx_pixel_read(&cub->map.texture[text.dir], text.x, text.y);
		if (ray->hit == ySide)
			color = (color >> 1) & 8355711;
		ft_mlx_pixel_put(&cub->img, x, y, color);
	}
}

void 	cast_walls(t_cub *cub, t_ray *ray)
{
	int		x;

	x = 0;
	while(x < WIN_WIDTH)
	{
		set_ray_dir_vector(cub, ray, x);
		ray->mapX = (int)cub->player.pos.x;
		ray->mapY = (int)cub->player.pos.y;
		set_deltaDist(ray);
		set_sideDist(ray, &cub->player);
		perform_DDA(ray, cub, x);
		draw_line(ray, cub, x);
		x++;
	}
}

int	is_walkable(char tile)
{
	if (tile == '0' || is_player(tile))
		return (true);
	else
		return(false);
}

void	turn_left(t_cub *cub)
{
	double	oldDirX;
	double	oldPlaneX;

	oldDirX = cub->player.dir.x;
	oldPlaneX = cub->camera.plane.x;
	cub->player.dir.x = cub->player.dir.x * cos(-ROTSPEED)
		- cub->player.dir.y * sin(-ROTSPEED);
	cub->player.dir.y = oldDirX * sin(-ROTSPEED) 
		+ cub->player.dir.y *cos(-ROTSPEED);
	set_camera_vector(cub);
}

void	turn_right(t_cub *cub)
{
	double	oldDirX;
	double	oldPlaneX;

	oldDirX = cub->player.dir.x;
	oldPlaneX = cub->camera.plane.x;
	cub->player.dir.x = cub->player.dir.x * cos(ROTSPEED)
		- cub->player.dir.y * sin(ROTSPEED);
	cub->player.dir.y = oldDirX * sin(ROTSPEED) 
		+ cub->player.dir.y *cos(ROTSPEED);
	set_camera_vector(cub);
}

void	move_forward(t_cub *cub)
{
	int	newPosX;
	int newPosY;

	newPosX = (int)(cub->player.pos.x + cub->player.dir.x * MOVESPEED);
	newPosY = (int)(cub->player.pos.y + cub->player.dir.y * MOVESPEED);
	if (is_walkable(cub->map.map[(int)cub->player.pos.y][newPosX]))
		cub->player.pos.x += cub->player.dir.x * MOVESPEED;
	if (is_walkable(cub->map.map[newPosY][(int)cub->player.pos.x]))
		cub->player.pos.y += cub->player.dir.y * MOVESPEED;
}

void	move_right(t_cub *cub)
{
	int	newPosX;
	int newPosY;

	newPosX = (int)(cub->player.pos.x + cub->camera.plane.x * MOVESPEED);
	newPosY = (int)(cub->player.pos.y + cub->camera.plane.y * MOVESPEED);
	if (is_walkable(cub->map.map[(int)cub->player.pos.y][newPosX]))
		cub->player.pos.x += cub->camera.plane.x * MOVESPEED;
	if (is_walkable(cub->map.map[newPosY][(int)cub->player.pos.x]))
		cub->player.pos.y += cub->camera.plane.y * MOVESPEED;
}

void	move_left(t_cub *cub)
{
	int	newPosX;
	int newPosY;

	newPosX = (int)(cub->player.pos.x - cub->camera.plane.x * MOVESPEED);
	newPosY = (int)(cub->player.pos.y - cub->camera.plane.y * MOVESPEED);
	if (is_walkable(cub->map.map[(int)cub->player.pos.y][newPosX]))
		cub->player.pos.x -= cub->camera.plane.x * MOVESPEED;
	if (is_walkable(cub->map.map[newPosY][(int)cub->player.pos.x]))
		cub->player.pos.y -= cub->camera.plane.y * MOVESPEED;
}

void move_backward(t_cub *cub)
{
	int	newPosX;
	int newPosY;

	newPosX = (int)(cub->player.pos.x - cub->player.dir.x * MOVESPEED);
	newPosY = (int)(cub->player.pos.y - cub->player.dir.y * MOVESPEED);
	if (is_walkable(cub->map.map[(int)cub->player.pos.y][newPosX]))
		cub->player.pos.x -= cub->player.dir.x * MOVESPEED;
	if (is_walkable(cub->map.map[newPosY][(int)cub->player.pos.x]))
		cub->player.pos.y -= cub->player.dir.y * MOVESPEED;
}

void	cast_floor_ceiling(t_cub *cub)
{
	unsigned int	color;

	for(int y = 0; y < WIN_HEIGHT; y++)
	{		
		for (int x = 0; x < WIN_WIDTH; x++)
		{
			color = cub->map.f_color;
			color = (color >> 1) & 8355711;
			ft_mlx_pixel_put(&cub->img, x, y, color);	
			color = cub->map.c_color;
			color = (color >> 1) & 8355711;
			ft_mlx_pixel_put(&cub->img, x, WIN_HEIGHT - y - 1, color);	
		}
	}
}

void	cast_sprites(t_cub *cub, t_ray *ray)
{
	t_spr sprite;
	double spriteDist;

	sprite.coord.x = 10.0;
	sprite.coord.y = 2.0;
	sprite.type = 0;
	spriteDist = ((cub->player.pos.x - sprite.coord.x) * (cub->player.pos.x - sprite.coord.x)
	+ (cub->player.pos.y - sprite.coord.y) * (cub->player.pos.y - sprite.coord.y));
	
	double spriteX;
	double spriteY;
	spriteX = sprite.coord.x - cub->player.pos.x;
	spriteY = sprite.coord.y - cub->player.pos.y;
	
	double invDet;
	invDet = 1 / (cub->camera.plane.x * cub->player.dir.y - cub->player.dir.x * cub->camera.plane.y);
	
	double transformX;
	double transformY;
	
	transformX = invDet * (cub->player.dir.y * spriteX - cub->player.dir.x * spriteY);
	transformY = invDet * (-cub->camera.plane.y * spriteX + cub->camera.plane.x * spriteY);
	
	int spriteScreenX;
	spriteScreenX = (int)(WIN_WIDTH / 2) * (1 + transformX / transformY);

	int spriteHeight;
	spriteHeight = abs((int) (WIN_HEIGHT / transformY));
	
	int drawStartY;
	int drawEndY;
	
	drawStartY = -spriteHeight / 2 + WIN_HEIGHT / 2;
	if (drawStartY < 0)
		drawStartY = 0;
	drawEndY = spriteHeight / 2 + WIN_HEIGHT / 2;
	if (drawEndY >= WIN_HEIGHT)
		drawEndY = WIN_HEIGHT - 1;
	int spriteWidth ;
	spriteWidth = abs((int) (WIN_HEIGHT / transformY));
	int drawStartX;
	int drawEndX;
	drawStartX = -spriteWidth / 2 + spriteScreenX;
	if (drawStartX < 0)
		drawStartX = 0;
	drawEndX = spriteWidth / 2 +spriteScreenX;
	if (drawEndX >= WIN_WIDTH)
		drawEndY = WIN_WIDTH - 1;
	for(int stripe = drawStartX; stripe <  drawEndX; stripe++)
	{
		int texX;
		texX = (int)(256 * (stripe - (-spriteWidth / 2 + spriteScreenX)) * cub->map.sprites[0].width / spriteWidth) / 256;
		if (transformY > 0 && stripe > 0 && stripe < WIN_WIDTH && transformY < ray->perpWallDist[stripe])
		{
			for (int y = drawStartY; y < drawEndY; y++)
			{
				int d;
				d = y * 256 - WIN_HEIGHT * 128 + spriteHeight * 128;
				int texY;
				texY = ((d * cub->map.sprites[0].height) / spriteHeight) / 256;
				unsigned int color;
				color = mlx_pixel_read(&cub->map.sprites[0], texX, texY);
				if((color & 0x00FFFFFF) != 0)
					ft_mlx_pixel_put(&cub->img, stripe, y, color);
			}
		}
	}
}

void	cub3d(t_cub *cub)
{
	t_ray	ray;

	cast_floor_ceiling(cub);
	cast_walls(cub, &ray);
	cast_sprites(cub, &ray);
	//printf("posX:%f dirX:%f posY:%f dirY:%f planeX:%f planeY:%f\n", cub->player.pos.x, cub->player.dir.x, cub->player.pos.y, cub->player.dir.y, cub->camera.plane.x, cub->camera.plane.y);
	mlx_put_image_to_window(cub->win.mlx, cub->win.mlx_win, cub->img.img, 0, 0);
}
