/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 22:47:18 by eozben            #+#    #+#             */
/*   Updated: 2022/04/21 18:24:04 by fbindere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"


void	open_texture_sprites(t_cub *cub)
{
	t_img *sprite;

	sprite = cub->map.sprites;
	sprite[0].img = mlx_xpm_file_to_image(
		cub->win.mlx, "textures/barrel.xpm", &sprite[0].width, &sprite[0].height);
	// if (sprite[0].img)
	// 	map_error(cub, NULL, "invalid texture path");
	sprite[0].addr = mlx_get_data_addr(
		sprite[0].img , &sprite[0].bits_per_pixel, &sprite[0].line_length, &sprite[0].endian);
	sprite[1].img = mlx_xpm_file_to_image(
		cub->win.mlx, "textures/pillar.xpm", &sprite[1].width, &sprite[1].height);
	sprite[1].addr = mlx_get_data_addr(
		sprite[1].img , &sprite[1].bits_per_pixel, &sprite[1].line_length, &sprite[1].endian);
	sprite[2].img = mlx_xpm_file_to_image(
		cub->win.mlx, "textures/greenlight.xpm", &sprite[2].width, &sprite[2].height);
	sprite[2].addr = mlx_get_data_addr(
		sprite[2].img , &sprite[2].bits_per_pixel, &sprite[2].line_length, &sprite[2].endian);
}

void	parse_cub_file(t_cub *cub, char **argv)
{	
	cub->map.fd = open(argv[1], O_RDWR);
	if (cub->map.fd == ERROR)
		map_error(cub, NULL, "Bad .cub file");
	read_textures(cub);
	open_texture_files(cub);
	open_texture_sprites(cub);
	read_map(cub);
	check_map_validity(cub, &cub->player);
	close(cub->map.fd);
}
