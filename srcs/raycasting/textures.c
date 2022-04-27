/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eozben <eozben@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/27 21:43:32 by eozben            #+#    #+#             */
/*   Updated: 2022/04/27 21:43:41 by eozben           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

void	get_texture_x(t_cub *cub, t_ray *ray, t_text *text, int x)
{
	double	wall_x;

	if (ray->hit == x_side)
		wall_x = cub->player.pos.y + ray->perp_wall_dist[x] * ray->dir.y;
	else
		wall_x = cub->player.pos.x + ray->perp_wall_dist[x] * ray->dir.x;
	wall_x -= floor(wall_x);
	text->x = (int)(wall_x * (double)cub->map.texture[text->dir].width);
	if (ray->hit == x_side && ray->dir.x > 0)
		text->x = cub->map.texture[text->dir].width - text->x - 1;
	if (ray->hit == y_side && ray->dir.y < 0)
		text->x = cub->map.texture[text->dir].width - text->x - 1;
}

void	get_text_values(t_cub *cub, t_dline *line, t_text *text)
{
	text->step = 1.0 * cub->map.texture[text->dir].height / line->height;
	text->pos = (line->start - WIN_HEIGHT / 2 + line->height / 2) * text->step;
}

void	get_text_type(t_cub *cub, t_ray *ray, t_text *text)
{
	if (cub->map.map[ray->map_y][ray->map_x] == 'D')
		text->dir = DOOR;
	else if (ray->hit == y_side)
	{
		if (ray->dir.y >= 0)
			text->dir = NORTH;
		else
			text->dir = SOUTH;
	}
	else if (ray->hit == x_side)
	{
		if (ray->dir.x >= 0)
			text->dir = EAST;
		else
			text->dir = WEST;
	}
}
