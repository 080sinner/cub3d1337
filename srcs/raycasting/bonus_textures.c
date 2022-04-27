/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bonus_textures.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eozben <eozben@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/27 21:48:35 by eozben            #+#    #+#             */
/*   Updated: 2022/04/27 21:48:48 by eozben           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

void	draw_minimap(t_cub *cub)
{
	int	x;
	int	y;

	cub->map.mmap.tile_width = cub->map.mmap.width / (2 * MMAPZOOM + 1);
	cub->map.mmap.tile_height = cub->map.mmap.height / (2 * MMAPZOOM + 1);
	y = 0;
	while (y < cub->map.mmap.height)
	{
		x = 0;
		while (x < cub->map.mmap.width)
		{
			colour_pixel(cub, x, y);
			x++;
		}
		y++;
	}
}

void	door(t_cub *cub)
{
	int	x;
	int	y;

	x = (int)cub->player.pos.x + cub->player.dir.x;
	y = (int)cub->player.pos.y + cub->player.dir.y;
	if (x != cub->player.pos.x && y != cub->player.pos.y)
	{
		if (cub->map.map[y][x] == 'D')
			cub->map.map[y][x] = 'O';
		else if (cub->map.map[y][x] == 'O')
			cub->map.map[y][x] = 'D';
	}
}
