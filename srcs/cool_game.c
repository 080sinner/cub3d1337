/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cool_game.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/06 16:41:45 by fbindere          #+#    #+#             */
/*   Updated: 2022/04/08 22:48:36 by fbindere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

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
	if (ray->dir.y)
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

void	perform_DDA(t_ray *ray, t_cub *cub)
{
	int hit;

	hit = 0;
	while (hit == 0)
	{
		if (ray->sideDist.x < ray->sideDist.y)
		{
			ray->sideDist.x += ray->deltaDist.x;
			ray->mapX += ray->stepX;
			ray->hit = XSide;
		}
		else
		{
			ray->sideDist.y += ray->deltaDist.y;
			ray->mapY += ray->stepY;
			ray->hit = ySide;
		}
		if (cub->map.map[ray->mapY][ray->mapX] > '0')
			hit = 1;
	}
	if (ray->hit == XSide)
		ray->perpWallDist =  ray->sideDist.x - ray->deltaDist.x;
	else
		ray->perpWallDist = ray->sideDist.y - ray->deltaDist.y;
}


void	draw_line2(t_ray *ray, t_cub *cub, int x)
{
	int lineheight;
	int drawStart;
	int drawEnd;

	lineheight = (int)(WIN_HEIGHT / ray->perpWallDist);
	drawStart = -lineheight / 2 + WIN_HEIGHT / 2;
	if (drawStart < 0)
		drawStart = 0;
	drawEnd = lineheight / 2 + WIN_HEIGHT / 2;
	if (drawEnd >= WIN_HEIGHT)
		drawEnd = WIN_HEIGHT -1;
	int texNUM;;
	double wallX;

	texNUM = 1;
	if (ray->hit == XSide)
		wallX = cub->player.pos.y + ray->perpWallDist * ray->dir.y;
	else
		wallX = cub->player.pos.x + ray->perpWallDist * ray->dir.x;
	wallX -= floor(wallX);

	int texX;
	texX = (int)(wallX * (double)cub->map.texture[texNUM].width);
	
	if (ray->hit == XSide && ray->dir.x > 0)
		texX = cub->map.texture[texNUM].width - texX - 1; 
	if (ray->hit == ySide && ray->dir.y < 0)
		texX = cub->map.texture[texNUM].width - texX - 1; 
	double step;
	
	step = 1 * cub->map.texture[texNUM].height / lineheight;
	double texpos;
	texpos = (drawStart - WIN_HEIGHT / 2 + lineheight) * step;
	for (int y = drawStart; y < drawEnd; y++)
	{
		int texY;
		texY = (int) texpos & (cub->map.texture[texNUM].height - 1);
		texpos +=  step;
		int color;
		color = (cub->map.texture[texNUM].img[cub->map.texture[texNUM].height * texY + texX]);
		// if (ray->hit == ySide)
		// 	color = (color >> 1) & 8355711;
		ft_mlx_pixel_put(&cub->img, x, y, color);
	}
	// y = 0;
	// while(y < WIN_HEIGHT)
	// {
	// 	if (y >= drawStart && y < drawEnd)
	// 		ft_mlx_pixel_put(&cub->img, x, y, color);
	// 	else
	// 		ft_mlx_pixel_put(&cub->img, x, y, 0);
	// 	y++;
	// }
	//real calc.
	// y = drawStart;
	// while(y < drawEnd)
	// {
	// 	ft_mlx_pixel_put(&cub->img, x, y, color);
	// 	y++;
	// }
}

void 	calculate_frame(t_cub *cub)
{
	t_ray	ray;
	int		x;
	
	x = 0;
	while(x < WIN_WIDTH)
	{
		set_ray_dir_vector(cub, &ray, x);
		ray.mapX = (int)cub->player.pos.x;
		ray.mapY = (int)cub->player.pos.y;
		set_deltaDist(&ray);
		set_sideDist(&ray, &cub->player);
		perform_DDA(&ray, cub);
		x++;
	}
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
	cub->camera.plane.x = cub->camera.plane.x * cos(-ROTSPEED) 
		- cub->camera.plane.y * sin(-ROTSPEED);
	cub->camera.plane.y = oldPlaneX * sin(-ROTSPEED)
		+ cub->camera.plane.y * cos(-ROTSPEED);
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
	cub->camera.plane.x = cub->camera.plane.x * cos(ROTSPEED) 
		- cub->camera.plane.y * sin(ROTSPEED);
	cub->camera.plane.y = oldPlaneX * sin(ROTSPEED)
		+ cub->camera.plane.y * cos(ROTSPEED);
}

void	move_forward(t_cub *cub)
{
	int	newPosX;
	int newPosY;

	newPosX = (int)(cub->player.pos.x + cub->player.dir.x * MOVESPEED);
	newPosY = (int)(cub->player.pos.y + cub->player.dir.y * MOVESPEED);
	if (cub->map.map[(int)cub->player.pos.y][newPosX] == '0')
		cub->player.pos.x += cub->player.dir.x * MOVESPEED;
	if (cub->map.map[newPosY][(int)cub->player.pos.x] == '0')
		cub->player.pos.y += cub->player.dir.y * MOVESPEED;
}

void move_backward(t_cub *cub)
{
	int	newPosX;
	int newPosY;

	newPosX = (int)(cub->player.pos.x - cub->player.dir.x * MOVESPEED);
	newPosY = (int)(cub->player.pos.y - cub->player.dir.y * MOVESPEED);
	if (cub->map.map[(int)cub->player.pos.y][newPosX] == '0')
		cub->player.pos.x -= cub->player.dir.x * MOVESPEED;
	if (cub->map.map[newPosY][(int)cub->player.pos.x] == '0')
		cub->player.pos.y -= cub->player.dir.y * MOVESPEED;
}


void	cub3d(t_cub *cub)
{
	calculate_frame(cub);
	mlx_put_image_to_window(cub->win.mlx, cub->win.mlx_win, cub->img.img, 0, 0);
}