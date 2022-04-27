/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colour.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eozben <eozben@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 22:46:44 by eozben            #+#    #+#             */
/*   Updated: 2022/04/27 22:01:16 by eozben           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

int	create_trgb(int r, int g, int b)
{
	return (r << 16 | g << 8 | b);
}

int	read_colour(t_cub *cub, char *str, int i)
{
	char	**colour_array;
	int		colour[3];
	int		atoi_result;
	int		j;

	i += 1;
	i += skip_whitespaces(&str[i]);
	colour_array = ft_split(&str[i], ',');
	if (!colour_array)
		map_error(cub, str, "allocating floor colour");
	j = 0;
	while (colour_array[j])
	{
		if (!ft_atoi(colour_array[j], &atoi_result)
			|| (atoi_result > 255 || atoi_result < 0) || j > 2)
		{
			ft_free_strarray(colour_array);
			map_error(cub, str, "invalid colour code floor");
		}
		colour[j] = atoi_result;
		j++;
	}
	ft_free_strarray(colour_array);
	return (create_trgb(colour[0], colour[1], colour[2]));
}

char	*append_mapline(t_cub *cub, char *map_line, char *line)
{
	char	*tmp;

	tmp = map_line;
	map_line = ft_strjoin(map_line, line);
	free(tmp);
	if (!map_line)
		map_error(cub, line, "Allocating map");
	return (map_line);
}
