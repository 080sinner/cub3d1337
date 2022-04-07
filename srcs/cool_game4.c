/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cool_game4.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/06 16:41:45 by fbindere          #+#    #+#             */
/*   Updated: 2022/04/07 16:12:13 by fbindere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int 	calculate_frame2(t_cub *cub)
{
	double cameraX;
	double rayDirX;
	double rayDirY;
	double planeX;
	double planeY;

	planeX = .66;
	planeY = 0;

	for (int x = 0; x < WIN_WIDTH; x++)
	{
		cameraX = 2 * x / (double)WIN_WIDTH - 1;
		rayDirX = cub->player.dir_x + planeX * cameraX;
		rayDirY = cub->player.dir_y + planeY * cameraX;
		
		int mapX;
		int mapY;

		mapX = (int)cub->player.pos_x;
		mapY = (int)cub->player.pos_y;

		double sideDistX;
		double sideDistY;
		double deltaDistX;
		double deltaDistY;

		if (rayDirX == 0)
			deltaDistX = INFINITY;
		else
			deltaDistX = fabs(1 / rayDirX);
		if (rayDirY == 0)
			deltaDistY = INFINITY;
		else
			deltaDistY = fabs(1 / rayDirY);

		double perpWallDist;
		
		int stepX;
		int stepY;

		int hit;
		hit = 0;
		int side;

		if (rayDirX < 0)
		{
			stepX = -1;
			sideDistX = (cub->player.pos_x - mapX) * deltaDistX;
		}
		else
		{
			stepX = 1;
			sideDistX = (mapX + 1 - cub->player.pos_x) * deltaDistX;
		}
		if (rayDirY < 0)
		{
			stepY = -1;
			sideDistY = (cub->player.pos_y - mapY) * deltaDistY;
		}
		else
		{
			stepY = 1;
			sideDistY = (mapY + 1 - cub->player.pos_y) * deltaDistY;
		}
		//printf("posX %f, posY %f, mapX %d, mapY %d\n", posX, posY, mapX, mapY);
		//printf("sideDistX %f, sideDisty %f\n", sideDistX, sideDistX);
		//printf("sideDistX %f, sideDisty %f\n", sideDistX, sideDistX);
		while (hit == 0)
		{
			if (sideDistX < sideDistY)
			{
				sideDistX += deltaDistX;
				mapX += stepX;
				side = 0;
			}
			else
			{
				sideDistY += deltaDistY;
				mapY += stepY;
				side = 1;
			}
			if (cub->map.map[mapY][mapX] > '0')
				hit = 1;
		}
		//printf("sideDistX %f deltaDistX %f\n", sideDistX, sideDistY);
		if (side == 0)
			perpWallDist = (sideDistX - deltaDistX);
		else
			perpWallDist = (sideDistY - deltaDistY);
		//printf("perpwalldist %f \n", perpWallDist);
		int lineheight;
		lineheight = (int)(WIN_HEIGHT / perpWallDist);
		int drawStart;
		int drawEnd;
		drawStart = -lineheight / 2 + WIN_HEIGHT / 2;
		if (drawStart < 0)
			drawStart = 0;
		drawEnd = lineheight / 2 + WIN_HEIGHT / 2;
		if (drawEnd >= WIN_HEIGHT)
			drawEnd = WIN_HEIGHT -1;
		int color;
		color = create_trgb(255, 0, 0);
		if (side == 1)
			color = color / 2;
		//printf("%d drawStart %d drawEnd %d\n", x, drawStart, drawEnd);
		for (int y = drawStart; y < drawEnd; y++)
			ft_mlx_pixel_put(&cub->img, x, y, color);
	}
	mlx_put_image_to_window(cub->win.mlx, cub->win.mlx_win, cub->img.img, 0, 0);
	return (INFINITY);
}