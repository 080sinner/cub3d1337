/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   casting_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eozben <eozben@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/27 21:29:21 by eozben            #+#    #+#             */
/*   Updated: 2022/04/28 17:52:27 by eozben           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

int	is_obstacle(char c)
{
	if (c == '1' || c == 'D')
		return (1);
	return (0);
}

void	get_line_values(t_dline *line, t_ray *ray, int x)
{
	line->height = (int)(WIN_HEIGHT / ray->perp_wall_dist[x]);
	line->start = -line->height / 2 + WIN_HEIGHT / 2;
	if (line->start < 0)
		line->start = 0;
	line->end = line->height / 2 + WIN_HEIGHT / 2;
	if (line->end >= WIN_HEIGHT)
		line->end = WIN_HEIGHT - 1;
}

void	colour_pixel(t_cub *cub, int x, int y)
{
	int	map_y;
	int	map_x;

	map_y = (int)((y / cub->map.mmap.tile_height)
			+ cub->player.pos.y - MMAPZOOM);
	map_x = (int)((x / cub->map.mmap.tile_width)
			+ cub->player.pos.x - MMAPZOOM);
	if (map_x >= 0 && map_x <= cub->map.map_length - 1
		&& map_y >= 0 && map_y <= cub->map.map_height - 1
		&& cub->map.map[map_y][map_x] == '1')
		ft_mlx_pixel_put(&cub->img, x, y, 0);
	else if (map_x == (int)cub->player.pos.x
		&& map_y == (int)cub->player.pos.y)
		ft_mlx_pixel_put(&cub->img, x, y, 25600);
}
