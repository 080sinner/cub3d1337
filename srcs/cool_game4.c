/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cool_game4.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/06 16:41:45 by fbindere          #+#    #+#             */
/*   Updated: 2022/04/07 19:49:16 by fbindere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	set_camera_vector(t_camera *camera)
{
	camera->plane.y = 0; // muss noch angepasst werden
	camera->plane.x = .66; //muss noch angepasst werden
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


void	draw_line(t_ray *ray, t_cub *cub, int x, int color)
{
	int lineheight;
	int drawStart;
	int drawEnd;
	int	y;

	lineheight = (int)(WIN_HEIGHT / ray->perpWallDist);
	drawStart = -lineheight / 2 + WIN_HEIGHT / 2;
	if (drawStart < 0)
		drawStart = 0;
	drawEnd = lineheight / 2 + WIN_HEIGHT / 2;
	if (drawEnd >= WIN_HEIGHT)
		drawEnd = WIN_HEIGHT -1;
	y = drawStart;
	while(y < drawEnd)
	{
		ft_mlx_pixel_put(&cub->img, x, y, color);
		y++;
	}
}

int 	calculate_frame2(t_cub *cub)
{
	t_ray	ray;
	int		x;
	
	x = 0;
	while(x < WIN_WIDTH)
	{
		set_camera_vector(&cub->camera);
		set_ray_dir_vector(cub, &ray, x);
		ray.mapX = (int)cub->player.pos.x;
		ray.mapY = (int)cub->player.pos.y;
		set_deltaDist(&ray);
		set_sideDist(&ray, &cub->player);
		perform_DDA(&ray, cub);
		int color;
		color = create_trgb(255, 0, 0);
		if (ray.hit == ySide)
			color = color / 2;
		draw_line(&ray, cub, x, color);
		x++;
	}
	mlx_put_image_to_window(cub->win.mlx, cub->win.mlx_win, cub->img.img, 0, 0);
	return (INFINITY);
}