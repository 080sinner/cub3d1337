/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eozben <eozben@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 22:46:44 by eozben            #+#    #+#             */
/*   Updated: 2022/03/15 22:48:07 by eozben           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	create_trgb(int r, int g, int b)
{
	return (r << 16 | g << 8 | b);
}

static int	read_colour(t_map *map, char *str, int i)
{
	char	**colour_array;
	int		colour[3];
	int		atoi_result;
	int		j;

	i += 1;
	i += skip_whitespaces(&str[i]);
	colour_array = ft_split(&str[i], ',');
	if (!colour_array)
		map_error(map, str, "allocating floor colour");
	j = 0;
	while (colour_array[j])
	{
		if (!ft_atoi(colour_array[j], &atoi_result)
			|| (atoi_result > 255 || atoi_result < 0) || j > 2)
		{
			ft_free_strarray(colour_array);
			map_error(map, str, "invalid colour code floor");
		}
		colour[j] = atoi_result;
		j++;
	}
	ft_free_strarray(colour_array);
	return (create_trgb(colour[0], colour[1], colour[2]));
}

static void	check_floor_ceiling_colours(t_map *map, char *str)
{
	int	i;

	i = 0;
	str[ft_strlen(str) - 1] = '\0';
	i = skip_whitespaces(&str[i]);
	if (!ft_strncmp("F", &str[i], 1))
		map->f_color = read_colour(map, str, i);
	else if (!ft_strncmp("C", &str[i], 1))
		map->c_color = read_colour(map, str, i);
	else
		map_error(map, str, "no valid type identifier");
}

int	read_floor_ceiling_colour(t_map *map)
{
	char	*str;
	int		line_count;

	line_count = 0;
	while (line_count < 2)
	{
		str = get_next_line(map->map_fd);
		if (!str)
			break ;
		if (!ft_is_empty_line(str))
		{
			check_floor_ceiling_colours(map, str);
			line_count++;
		}
		free(str);
	}
	return (0);
}
