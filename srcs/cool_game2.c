/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cool_game2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/01 16:08:23 by fbindere          #+#    #+#             */
/*   Updated: 2022/04/02 20:03:22 by fbindere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

double posX;
double posY;
double dirX;
double dirY;
double cameraX;
double planeX = 0;
double planeY = 0.66;
double time = 0;
double oldtime = 0;
double rayDirX;
double rayDirY;

int calculate_frame(t_cub *cub)
{	
	printf("pos x %f pos y %f dirx %f diry %f\n", cub->player.pos_x, cub->player.pos_y, cub->player.dir_x, cub->player.dir_y);
	double dirX = cub->player.dir_x;
	double dirY = cub->player.dir_y;
	for (int x = 0; x < WIN_WIDTH; x++)
	{   
		
		//calculate ray position and direction
		cameraX = 2 * x / (double)(WIN_WIDTH) - 1;
		rayDirX = dirX + planeX * cameraX;
		rayDirY = dirY + planeY * cameraX;
		
		//printf("raydirX %f raydirY %f\n", rayDirX, rayDirY);
		//which box of the map we're in
		int mapX = (int)cub->player.pos_x;
		int mapY = (int)cub->player.pos_y;
		//length of ray from current position to next x or y-side
		double sideDistX;
		double sideDistY;
	 	//length of ray from one x or y-side to next x or y-side
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
		//what direction to step in x or y-direction (either +1 or -1)
		int stepX;
		int stepY;
		int hit = 0;
		int side = 0;
		//calculate step and initial sideDist
		if (rayDirX < 0)
		{
			stepX = -1;
			sideDistX = (cub->player.pos_x - mapX) * deltaDistX;
		}
		else
		{
			stepX = 1;
			sideDistX = (mapX + 1.0 - cub->player.pos_x) * deltaDistX;
		}
		if (rayDirY < 0)
		{
			stepY = -1;
			sideDistY = (cub->player.pos_y - mapY) * deltaDistY;
		}
		else
		{
			stepY = 1;
			sideDistY = (mapY + 1.0 - cub->player.pos_x) * deltaDistY;
		}
		//perform DDA
		while(hit == 0)
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
			printf("mapx %d, mapY %d, len %d, height %d, player posx %f, player posy %f\n", mapX, mapY, cub->map.map_length, cub->map.map_height \
			,cub->player.pos_x, cub->player.pos_y);
			if (cub->map.map[mapX][mapY] == '1')
				hit = 1;

		}
		exit(EXIT_FAILURE);
		double perpWallDist;
		if (side == 0)
			perpWallDist = (sideDistX - deltaDistX);
		else
			perpWallDist = (sideDistY - deltaDistY);
		//Calculate height of line to draw on screen
		int lineHeight = (int)(WIN_HEIGHT / perpWallDist);
	    //calculate lowest and highest pixel to fill in current stripe
		int drawStart = -lineHeight / 2 + WIN_HEIGHT / 2;
		if (drawStart < 0)
			drawStart = 0;
		int drawEnd = lineHeight / 2 + WIN_HEIGHT / 2;
		if (drawEnd >= WIN_HEIGHT)
			drawEnd = WIN_HEIGHT - 1;
		printf("start:%d end:%d\n", drawStart, drawEnd);
		int color;
		color = create_trgb(255, 0, 0);
		if (side == 1)
			color = color / 2;
		for (int y = drawStart; y < drawEnd; y++)
			ft_mlx_pixel_put(&cub->img, x, y, color);
	}
	mlx_put_image_to_window(cub->win.mlx, cub->win.mlx_win, cub->img.img, 0, 0);
	return (INFINITY);
}
