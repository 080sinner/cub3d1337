/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cool_game.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/30 22:20:27 by fbindere          #+#    #+#             */
/*   Updated: 2022/03/31 22:43:47 by fbindere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

double planeX = 0;
double planeY = 0.66;
double time = 0;
double old_time = 0;
double cameraX = 0;
double rayDirX = 0;
double rayDirY = 0;
int		mapX;
int		mapY;

void	cub3d(t_cub *cub)
{
	int x;
	x = 0;
	while (x < WIN_WIDTH - 1)
	{
		//x-coordinate in camera space
		cameraX = 2 * x / (double)WIN_WIDTH - 1;
		//calculate ray position and direction
		rayDirX = cub->player.dir_x + planeX * cameraX;
		rayDirY = cub->player.dir_y + planeY * cameraX;
      	//which box of the map we're in
		mapX = cub->player.pos_x;
		mapY = cub->player.pos_y;

      	//length of ray from current position to next x or y-side
		double sideDistX;
		double sideDistY;

		//length of ray from one x or y-side to next x or y-side
		double DeltaDistX;
		double DeltaDistY;
	
		if (rayDirX == 0)
			DeltaDistX = 1e30;
		else
			DeltaDistX = fabs(1 / rayDirX);
		if (rayDirY == 0)
			DeltaDistY = 1e30;
		else
			DeltaDistY = fabs(1 / rayDirY);
		printf("%.300f %.300f\n", DeltaDistX, rayDirX);
		//what direction to step in x or y-direction (either +1 or -1)
		int stepX;
		int stepY;
		
		//was there a wall hit?
		int hit = 0;
		// was a x-side or y-side hit?
		int side;
      	//calculate step and initial sideDist
		if (rayDirX < 0)
		{
			stepX = -1;
			sideDistX = (cub->player.pos_x - mapX) * DeltaDistX;
		}
		else
		{
			stepX = 1;
			sideDistX = (mapX + 1 - cub->player.pos_x) * DeltaDistX;
		}
		if (rayDirY < 0)
		{
			stepY = - 1;
			sideDistY = (cub->player.pos_y - mapY) *DeltaDistY;
		}
		else
		{
			stepY = 1;
			sideDistY = (mapY + 1 - cub->player.pos_y) * DeltaDistY;
		}
		while (hit == 0)
		{
			if (sideDistX < sideDistY)
			{
				sideDistX += DeltaDistX;
				mapX += stepX;
				side = xSide;
			}
			else
			{
				sideDistY += DeltaDistY;
				mapY += stepY;
				side = ySide;
			}
			if (cub->map.map[mapY][mapX] > 0)
				hit = 1;
		}
		//Calculate distance projected on camera direction
		double perpWallDist;
		if (side == xSide) 
			perpWallDist = (sideDistX - DeltaDistX);
		else
			perpWallDist = (sideDistY - DeltaDistY);
		//Calculate height of line to draw on screen
		int lineHeight;
		lineHeight = (int)(WIN_HEIGHT / perpWallDist);
		int drawStart;
		drawStart = -lineHeight / 2 + WIN_HEIGHT / 2;
		if (drawStart < 0)
			drawStart = 0;
		int drawEnd;
		drawEnd = lineHeight / 2 + WIN_HEIGHT / 2;
		if (drawEnd > WIN_HEIGHT)
			drawEnd = WIN_HEIGHT - 1;
		//choose wall color
		int	color;
		color =	create_trgb(255,0,0);
		//give x and y sides different brightness
		if (side == ySide)
			color = color / 2;
		exit(EXIT_FAILURE);
		for (int y = drawStart; y < drawEnd; y++)
		{
			printf("x:%d y:%d\n", x, y);
			ft_mlx_pixel_put(cub->img.img, x, y, color);
		}
		x++;
	}
	mlx_put_image_to_window(cub->win.mlx, cub->win.mlx_win,cub->img.img, 0, 0);
}