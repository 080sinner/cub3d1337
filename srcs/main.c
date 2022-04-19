/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/12 19:00:58 by eozben            #+#    #+#             */
/*   Updated: 2022/04/19 22:29:07 by fbindere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	set_starting_vectors(t_cub *cub)
{
	cub->player.dir.x = 0;
	cub->player.dir.y = 0;
	if (cub->map.p_dir == NORTH)
	{
		cub->player.dir.y = 1;
		cub->camera.plane.x = 0.66;
	}
	else if (cub->map.p_dir == EAST)
	{
		cub->player.dir.x = 1;
		cub->camera.plane.y = -0.66;
	}
	else if (cub->map.p_dir == SOUTH)
	{
		cub->player.dir.y = -1;
		cub->camera.plane.x = -0.66;
	}
	else if (cub->map.p_dir == WEST)
	{
		cub->player.dir.x = -1;
		cub->camera.plane.y = 0.66;
	}
}

void	print_cub_file(t_cub *cub)
{
	printf("no texture: %s\n", cub->map.paths[NORTH]);
	printf("so texture: %s\n", cub->map.paths[EAST]);
	printf("we texture: %s\n", cub->map.paths[SOUTH]);
	printf("ea texture: %s\n", cub->map.paths[WEST]);
	printf("f_colour: %d\n", cub->map.f_color);
	printf("c_colour: %d\n", cub->map.c_color);
	printf("player_x: %f\n", cub->player.pos.x);
	printf("player_y: %f\n", cub->player.pos.y);
	printf("dir_x: %f\n", cub->player.dir.x);
	printf("dir_y: %f\n", cub->player.dir.y);
}

int	main(int argc, char *argv[])
{
	t_cub	cub;

	(void)argv;
	if (argc != 2)
		return (printf("Error \nInvalid amount of arguments\n"));
	ft_memset((void*)&cub, 0, sizeof(t_cub));
	init_win_img(&cub);
	parse_cub_file(&cub, argv);
	print_cub_file(&cub);
	set_starting_vectors(&cub);
	cub3d(&cub);
	mlx_hooks(&cub);
	mlx_loop(cub.win.mlx);
}
