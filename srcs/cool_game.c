/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cool_game.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/06 16:41:45 by fbindere          #+#    #+#             */
/*   Updated: 2022/04/26 21:23:22 by fbindere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

// void	set_camera_vector(t_cub *cub)
// {
// 	cub->camera.plane.x = cub->player.dir.y;
// 	cub->camera.plane.y = cub->player.dir.x;
// 	cub->camera.plane.y *= -1;
// }

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
	cub->player.dir.x = cub->player.dir.x * cos(ROTSPEED)
		- cub->player.dir.y * sin(ROTSPEED);
	cub->player.dir.y = oldDirX * sin(ROTSPEED) 
		+ cub->player.dir.y *cos(ROTSPEED);
	cub->camera.plane.x = cub->camera.plane.x * cos(ROTSPEED) 
		- cub->camera.plane.y * sin(ROTSPEED);
	cub->camera.plane.y = oldPlaneX * sin(ROTSPEED) 
		+ cub->camera.plane.y * cos(ROTSPEED);
}

void	turn_right(t_cub *cub)
{
	double	oldDirX;
	double	oldPlaneX;

	oldDirX = cub->player.dir.x;
	oldPlaneX = cub->camera.plane.x;
	cub->player.dir.x = cub->player.dir.x * cos(-ROTSPEED)
		- cub->player.dir.y * sin(-ROTSPEED);
	cub->player.dir.y = oldDirX * sin(-ROTSPEED) 
		+ cub->player.dir.y *cos(-ROTSPEED);
	cub->camera.plane.x = cub->camera.plane.x * cos(-ROTSPEED) 
		- cub->camera.plane.y * sin(-ROTSPEED);
	cub->camera.plane.y = oldPlaneX * sin(-ROTSPEED) 
		+ cub->camera.plane.y * cos(-ROTSPEED);
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

void	draw_minimap(t_cub *cub)
{
	int	mm_height;
	int	mm_width;
	int	left_border;
	int	right_border;
	int upper_border;
	int lower_border;
	double	tile_w;
	double	tile_h;

	mm_height = WIN_HEIGHT * 0.25;
	mm_width = WIN_WIDTH * 0.25;
	tile_w = mm_width / 9;
	tile_h = mm_width / 9;

	left_border = cub->player.pos.x - 4;
	if (left_border < 0)
		left_border = 0;

	right_border = cub->player.pos.x + 4;
	if (right_border > cub->map.map_length - 1)
		left_border = cub->map.map_length - 1;

	upper_border = cub->player.pos.y - 4;
	if (upper_border < 0)
		upper_border = 0;

	lower_border = cub->player.pos.y + 4;
	if (lower_border > cub->map.map_height - 1)
		lower_border = cub->map.map_height - 1;
	
	for (int y = 0; y < mm_height; y++)
	{
		for (int x = 0; x < mm_width; x++)
		{
			if (cub->map.map[((int)y / tile_w) - left_border]
		}
	}
	
}


// void	draw_minimap(t_cub *cub)
// {
// 	int	mm_height;
// 	int	mm_width;
// 	int	tile_width;
// 	int	tile_height;

// 	mm_height = WIN_HEIGHT * 0.25;
// 	mm_width = WIN_WIDTH * 0.25;
// 	tile_width = mm_width / cub->map.map_length;
// 	tile_height = mm_width / cub->map.map_height;
// 	for (int y = 0; y < mm_height; y++)
// 	{
// 		for (int x = 0; x < mm_width; x++)
// 		{
// 			if (cub->map.map[y / tile_height][x / tile_width] == '1')
// 				ft_mlx_pixel_put(&cub->img, x, y, 0);
// 			else if (is_player(cub->map.map[y / tile_height][x / tile_width]))
// 				ft_mlx_pixel_put(&cub->img, x, y, 25600);
// 			else	
// 				ft_mlx_pixel_put(&cub->img, x, y, 16777215);

// 		}
// 	}
// }


void	cub3d(t_cub *cub)
{
	t_ray	ray;

	cast_floor_ceiling(cub);
	cast_walls(cub, &ray);
	cast_sprites(cub, &ray);
	draw_minimap(cub);
	mlx_put_image_to_window(cub->win.mlx, cub->win.mlx_win, cub->img.img, 0, 0);
}
