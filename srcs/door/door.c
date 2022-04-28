/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   door.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/28 20:22:33 by fbindere          #+#    #+#             */
/*   Updated: 2022/04/28 20:32:57 by fbindere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

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
