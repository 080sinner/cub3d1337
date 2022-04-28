/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eozben <eozben@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 22:47:18 by eozben            #+#    #+#             */
/*   Updated: 2022/04/28 21:16:59 by eozben           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void	parse_cub_file(t_cub *cub, char **argv)
{	
	cub->map.fd = open(argv[1], O_RDWR);
	if (cub->map.fd == ERROR)
		map_error(cub, NULL, "Bad .cub img_arr");
	read_textures(cub);
	read_map(cub);
	check_map_validity(cub, &cub->player);
	close(cub->map.fd);
}
