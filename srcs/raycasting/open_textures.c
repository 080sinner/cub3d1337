/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_textures.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eozben <eozben@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 22:48:51 by eozben            #+#    #+#             */
/*   Updated: 2022/04/27 21:40:52 by eozben           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

static char	*read_path(t_cub *cub, char *str, int i)
{
	char	*path;

	i += 2;
	i += skip_whitespaces(&str[i]);
	path = ft_strdup(&str[i]);
	if (!path)
		map_error(cub, str, "allocating texture path");
	return (path);
}

static void	check_identifier(t_cub *cub, char *str)
{
	int	i;

	i = 0;
	str[ft_strlen(str) - 1] = '\0';
	i = skip_whitespaces(&str[i]);
	if (!ft_strncmp("NO", &str[i], 2))
		cub->map.paths[NORTH] = read_path(cub, str, i);
	else if (!ft_strncmp("SO", &str[i], 2))
		cub->map.paths[SOUTH] = read_path(cub, str, i);
	else if (!ft_strncmp("WE", &str[i], 2))
		cub->map.paths[WEST] = read_path(cub, str, i);
	else if (!ft_strncmp("EA", &str[i], 2))
		cub->map.paths[EAST] = read_path(cub, str, i);
	else if (str[i] == 'F')
		cub->map.f_color = read_colour(cub, &str[i], i);
	else if (str[i] == 'C')
		cub->map.c_color = read_colour(cub, &str[i], i);
	else
		map_error(cub, str, "no valid type identifier");
}

void	open_texture_files(t_cub *cub)
{
	int		i;
	t_img	*tex;

	i = 0;
	while (i < 4)
	{
		tex = &cub->map.texture[i];
		tex->img = mlx_xpm_file_to_image(
				cub->win.mlx, cub->map.paths[i], &tex->width, &tex->height);
		if (!tex->img)
			map_error(cub, NULL, "invalid texture path");
		tex->addr = mlx_get_data_addr(tex->img, &tex->bits_per_pixel,
				&tex->line_length, &tex->endian);
		i++;
	}
}

int	read_textures(t_cub *cub)
{
	char	*str;
	int		line_count;

	line_count = 0;
	while (line_count < 6)
	{
		str = get_next_line(cub->map.fd);
		if (!str)
			break ;
		if (!ft_is_empty_line(str))
		{
			check_identifier(cub, str);
			line_count++;
		}
		free(str);
	}
	if (line_count != 6)
		map_error(cub, NULL, "Invalid texture configuration");
	return (0);
}
