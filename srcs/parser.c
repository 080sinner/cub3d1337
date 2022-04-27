/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eozben <eozben@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 22:47:18 by eozben            #+#    #+#             */
/*   Updated: 2022/04/27 19:57:22 by eozben           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	open_xpm_file(int ident, char *path, t_cub *cub)
{
	t_img	*sprite;

	sprite = cub->map.sprites;
	sprite[ident].img = mlx_xpm_file_to_image(cub->win.mlx, path,
			&sprite[ident].width, &sprite[ident].height);
	if (!sprite[ident].img)
		map_error(cub, NULL, "invalid texture path");
	sprite[ident].addr = mlx_get_data_addr(sprite[ident].img,
			&sprite[ident].bits_per_pixel, &sprite[ident].line_length,
			&sprite[ident].endian);
	if (!sprite[ident].addr)
		map_error(cub, NULL, "invalid texture address");
}

void	open_texture_sprites(t_cub *cub)
{
	open_xpm_file(BARREL, "textures/barrel.xpm", cub);
	open_xpm_file(PILLAR, "textures/pillar.xpm", cub);
	open_xpm_file(LAMP, "textures/greenlight.xpm", cub);
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
