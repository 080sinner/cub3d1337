/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_vectors.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eozben <eozben@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/27 21:24:05 by eozben            #+#    #+#             */
/*   Updated: 2022/04/27 21:32:13 by eozben           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

void	set_ray_dir_vector(t_cub *cub, t_ray *ray, int x)
{
	double	camera_x;

	camera_x = 2 * x / (double)WIN_WIDTH - 1;
	ray->dir.x = cub->player.dir.x + cub->camera.plane.x * camera_x;
	ray->dir.y = cub->player.dir.y + cub->camera.plane.y * camera_x;
}

void	set_delta_dist(t_ray *ray)
{
	if (ray->dir.x == 0)
		ray->delta_dist.x = INFINITY;
	else
		ray->delta_dist.x = fabs(1 / ray->dir.x);
	if (ray->dir.y == 0)
		ray->delta_dist.y = INFINITY;
	else
		ray->delta_dist.y = fabs(1 / ray->dir.y);
}

void	set_side_dist(t_ray *ray, t_player *player)
{
	if (ray->dir.x < 0)
	{
		ray->side_dist.x = (player->pos.x - ray->map_x) * ray->delta_dist.x;
		ray->step_x = -1;
	}
	else
	{
		ray->side_dist.x = (ray->map_x + 1 - player->pos.x) * ray->delta_dist.x;
		ray->step_x = 1;
	}
	if (ray->dir.y < 0)
	{
		ray->side_dist.y = (player->pos.y - ray->map_y) * ray->delta_dist.y;
		ray->step_y = -1;
	}
	else
	{
		ray->side_dist.y = (ray->map_y + 1 - player->pos.y) * ray->delta_dist.y;
		ray->step_y = 1;
	}
}
