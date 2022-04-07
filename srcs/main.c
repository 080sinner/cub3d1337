/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/12 19:00:58 by eozben            #+#    #+#             */
/*   Updated: 2022/04/07 16:34:04 by fbindere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	cub3d(t_cub *cub);

void	print_cub_file(t_cub *cub)
{
	printf("no texture: %s\n", cub->map.no_path);
	printf("so texture: %s\n", cub->map.so_path);
	printf("we texture: %s\n", cub->map.we_path);
	printf("ea texture: %s\n", cub->map.ea_path);
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
	calculate_frame2(&cub);
	//calculate_frame(&cub);
	mlx_hooks(&cub);
	mlx_loop(cub.win.mlx);
}
