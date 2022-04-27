/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 22:47:18 by eozben            #+#    #+#             */
/*   Updated: 2022/04/27 16:41:16 by fbindere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	open_texture_sprites(t_cub *cub)
{
	t_img	*sprite;
	// hier fehlt noch error management;
	sprite = cub->map.sprites;
	sprite[BARREL].img = mlx_xpm_file_to_image(cub->win.mlx, "textures/barrel.xpm",
			&sprite[BARREL].width, &sprite[BARREL].height);
	if (!sprite[BARREL].img)
		map_error(cub, NULL, "invalid texture path");
	sprite[BARREL].addr = mlx_get_data_addr(sprite[BARREL].img, &sprite[BARREL].bits_per_pixel,
			&sprite[BARREL].line_length, &sprite[BARREL].endian);
	sprite[PILLAR].img = mlx_xpm_file_to_image(cub->win.mlx, "textures/pillar.xpm",
			&sprite[PILLAR].width, &sprite[1].height);
	sprite[PILLAR].addr = mlx_get_data_addr(sprite[PILLAR].img, &sprite[PILLAR].bits_per_pixel,
			&sprite[PILLAR].line_length, &sprite[PILLAR].endian);
	sprite[LAMP].img = mlx_xpm_file_to_image(cub->win.mlx,
			"textures/greenlight.xpm", &sprite[LAMP].width, &sprite[LAMP].height);
	sprite[LAMP].addr = mlx_get_data_addr(sprite[LAMP].img, &sprite[LAMP].bits_per_pixel,
			&sprite[LAMP].line_length, &sprite[LAMP].endian);
}

void	open_texture_door(t_cub *cub)
{
	t_img	*tex;

	tex = &cub->map.texture[DOOR];
	tex->img = mlx_xpm_file_to_image(
			cub->win.mlx, "textures/door.xpm", &tex->width, &tex->height);
	if (!tex->img)
		map_error(cub, NULL, "invalid texture path");
	tex->addr = mlx_get_data_addr(tex->img, &tex->bits_per_pixel,
			&tex->line_length, &tex->endian);
}


void	parse_cub_file(t_cub *cub, char **argv)
{	
	cub->map.fd = open(argv[1], O_RDWR);
	if (cub->map.fd == ERROR)
		map_error(cub, NULL, "Bad .cub file");
	read_textures(cub);
	open_texture_files(cub);
	open_texture_door(cub);
	open_texture_sprites(cub);
	read_map(cub);
	check_map_validity(cub, &cub->player);
	close(cub->map.fd);
}
