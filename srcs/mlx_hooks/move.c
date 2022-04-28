/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eozben <eozben@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/27 21:27:14 by eozben            #+#    #+#             */
/*   Updated: 2022/04/28 17:52:27 by eozben           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void	move_forward(t_cub *cub)
{
	int	newpos_x;
	int	newpos_y;

	newpos_x = (int)(cub->player.pos.x + cub->player.dir.x * MOVESPEED);
	newpos_y = (int)(cub->player.pos.y + cub->player.dir.y * MOVESPEED);
	if (!is_obstacle(cub->map.map[(int)cub->player.pos.y][newpos_x]))
		cub->player.pos.x += cub->player.dir.x * MOVESPEED;
	if (!is_obstacle(cub->map.map[newpos_y][(int)cub->player.pos.x]))
		cub->player.pos.y += cub->player.dir.y * MOVESPEED;
}

void	move_right(t_cub *cub)
{
	int	newpos_x;
	int	newpos_y;

	newpos_x = (int)(cub->player.pos.x + cub->camera.plane.x * MOVESPEED);
	newpos_y = (int)(cub->player.pos.y + cub->camera.plane.y * MOVESPEED);
	if (!is_obstacle(cub->map.map[(int)cub->player.pos.y][newpos_x]))
		cub->player.pos.x += cub->camera.plane.x * MOVESPEED;
	if (!is_obstacle(cub->map.map[newpos_y][(int)cub->player.pos.x]))
		cub->player.pos.y += cub->camera.plane.y * MOVESPEED;
}

void	move_left(t_cub *cub)
{
	int	newpos_x;
	int	newpos_y;

	newpos_x = (int)(cub->player.pos.x - cub->camera.plane.x * MOVESPEED);
	newpos_y = (int)(cub->player.pos.y - cub->camera.plane.y * MOVESPEED);
	if (!is_obstacle(cub->map.map[(int)cub->player.pos.y][newpos_x]))
		cub->player.pos.x -= cub->camera.plane.x * MOVESPEED;
	if (!is_obstacle(cub->map.map[newpos_y][(int)cub->player.pos.x]))
		cub->player.pos.y -= cub->camera.plane.y * MOVESPEED;
}

void	move_backward(t_cub *cub)
{
	int	newpos_x;
	int	newpos_y;

	newpos_x = (int)(cub->player.pos.x - cub->player.dir.x * MOVESPEED);
	newpos_y = (int)(cub->player.pos.y - cub->player.dir.y * MOVESPEED);
	if (!is_obstacle(cub->map.map[(int)cub->player.pos.y][newpos_x]))
		cub->player.pos.x -= cub->player.dir.x * MOVESPEED;
	if (!is_obstacle(cub->map.map[newpos_y][(int)cub->player.pos.x]))
		cub->player.pos.y -= cub->player.dir.y * MOVESPEED;
}
