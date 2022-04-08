/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 22:48:51 by eozben            #+#    #+#             */
/*   Updated: 2022/04/08 22:32:28 by fbindere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static char	*read_path(t_map *map, char *str, int i)
{
	char	*path;

	i += (2 + skip_whitespaces(&str[i]));
	path = ft_strdup(&str[i]);
	if (!path)
		map_error(map, str, "allocating texture path");
	return (path);
}

static void	check_identifier(t_map *map, char *str)
{
	int	i;

	i = 0;
	str[ft_strlen(str) - 1] = '\0';
	i = skip_whitespaces(&str[i]);
	if (!ft_strncmp("NO", &str[i], 2))
		map->paths[NORTH] = read_path(map, str, i);
	else if (!ft_strncmp("SO", &str[i], 2))
		map->paths[SOUTH] = read_path(map, str, i);
	else if (!ft_strncmp("WE", &str[i], 2))
		map->paths[WEST] = read_path(map, str, i);
	else if (!ft_strncmp("EA", &str[i], 2))
		map->paths[EAST] = read_path(map, str, i);
	else if (str[i] == 'F')
		map->f_color = read_colour(map, &str[i], i);
	else if (str[i] == 'C')
		map->c_color = read_colour(map, &str[i], i);
	else
		map_error(map, str, "no valid type identifier");
}

void	open_texture_files(t_cub *cub)
{
	int	i;
	t_img *tex;

	i = 0;
	while (i < 4)
	{
		tex = &cub->map.texture[i];
		tex->img = mlx_xpm_file_to_image(
			cub->win.mlx, cub->map.paths[i], &tex->width, &tex->height);
		mlx_get_data_addr(
			tex->img, &tex->bits_per_pixel, &tex->line_length, &tex->endian);
		i++; 
	}
}

int	read_textures(t_map *map)
{
	char	*str;
	int		line_count;

	line_count = 0;
	while (line_count < 6)
	{
		str = get_next_line(map->fd);
		if (!str)
			break ;
		if (!ft_is_empty_line(str))
		{
			check_identifier(map, str);
			line_count++;
		}
		free(str);
	}
	if (line_count != 6)
		map_error(map, NULL, "Invalid texture configuration");
	return (0);
}
