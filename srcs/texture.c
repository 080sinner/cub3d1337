/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eozben <eozben@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 22:48:51 by eozben            #+#    #+#             */
/*   Updated: 2022/03/15 23:23:33 by eozben           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	read_path(t_map *map, char indentifier, char *str, int i)
{
	i += (2 + skip_whitespaces(&str[i]));
	if (indentifier == 'N')
	{
		map->no_path = ft_strdup(&str[i]);
		if (!map->no_path)
			map_error(map, str, "allocating texture path");
	}
	else if (indentifier == 'E')
	{
		map->ea_path = ft_strdup(&str[i]);
		if (!map->ea_path)
			map_error(map, str, "allocating texture path");
	}
	else if (indentifier == 'S')
	{
		map->so_path = ft_strdup(&str[i]);
		if (!map->so_path)
			map_error(map, str, "allocating texture path");
	}
	else if (indentifier == 'W')
	{
		map->we_path = ft_strdup(&str[i]);
		if (!map->we_path)
			map_error(map, str, "allocating texture path");
	}
}

static void	check_texture_path(t_map *map, char *str)
{
	int	i;

	i = 0;
	i = skip_whitespaces(&str[i]);
	if (!ft_strncmp("NO", &str[i], 2))
		return (read_path(map, 'N', str, i));
	else if (!ft_strncmp("SO", &str[i], 2))
		return (read_path(map, 'S', str, i));
	else if (!ft_strncmp("WE", &str[i], 2))
		return (read_path(map, 'W', str, i));
	else if (!ft_strncmp("EA", &str[i], 2))
		return (read_path(map, 'E', str, i));
	map_error(map, str, "no valid type identifier");
}

int	read_wall_texture(t_map *map)
{
	char	*str;
	int		line_count;

	line_count = 0;
	while (line_count < 4)
	{
		str = get_next_line(map->map_fd);
		if (!str)
			break ;
		if (!ft_is_empty_line(str))
		{
			check_texture_path(map, str);
			line_count++;
		}
		free(str);
	}
	return (0);
}
