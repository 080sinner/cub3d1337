/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 22:47:18 by eozben            #+#    #+#             */
/*   Updated: 2022/04/18 00:14:18 by fbindere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	parse_cub_file(t_cub *cub, char **argv)
{	
	cub->map.fd = open(argv[1], O_RDWR);
	if (cub->map.fd == ERROR)
		map_error(&cub->map, NULL, "Bad .cub file");
	read_textures(&cub->map);
	open_texture_files(cub);
	read_map(&cub->map);
	check_map_validity(&cub->map, &cub->player);
	close(cub->map.fd);
}
