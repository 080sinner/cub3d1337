/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eozben <eozben@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 22:47:18 by eozben            #+#    #+#             */
/*   Updated: 2022/04/28 17:52:27 by eozben           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void	open_xpm_file(int ident, char *path, t_img *img, t_cub *cub)
{
	t_img	*img_arr;

	img_arr = img;
	img_arr[ident].img = mlx_xpm_file_to_image(cub->win.mlx, path,
			&img_arr[ident].width, &img_arr[ident].height);
	if (!img_arr[ident].img)
		map_error(cub, NULL, "invalid texture path");
	img_arr[ident].addr = mlx_get_data_addr(img_arr[ident].img,
			&img_arr[ident].bits_per_pixel, &img_arr[ident].line_length,
			&img_arr[ident].endian);
	if (!img_arr[ident].addr)
		map_error(cub, NULL, "invalid texture address");
}

void	open_texture_sprites(t_cub *cub)
{
	open_xpm_file(BARREL, "textures/barrel.xpm", cub->map.sprites, cub);
	open_xpm_file(PILLAR, "textures/pillar.xpm", cub->map.sprites, cub);
	open_xpm_file(LAMP, "textures/greenlight.xpm", cub->map.sprites, cub);
}

void	open_texture_enemy(t_cub *cub)
{
	open_xpm_file(0, "textures/enemy_0.xpm", cub->map.enemy, cub);
	open_xpm_file(1, "textures/enemy_1.xpm", cub->map.enemy, cub);
	open_xpm_file(2, "textures/enemy_2.xpm", cub->map.enemy, cub);
	open_xpm_file(3, "textures/enemy_3.xpm", cub->map.enemy, cub);
	open_xpm_file(4, "textures/enemy_4.xpm", cub->map.enemy, cub);
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
		map_error(cub, NULL, "Bad .cub img_arr");
	read_textures(cub);
	open_texture_files(cub);
	open_texture_door(cub);
	open_texture_sprites(cub);
	open_texture_enemy(cub);
	read_map(cub);
	check_map_validity(cub, &cub->player);
	close(cub->map.fd);
}
