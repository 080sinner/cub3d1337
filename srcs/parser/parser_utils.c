/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eozben <eozben@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/27 21:53:30 by eozben            #+#    #+#             */
/*   Updated: 2022/04/28 17:52:27 by eozben           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void	get_map_size(char *map_line, int *longest_line, int *line_count)
{
	int	i;
	int	current_line;

	current_line = 0;
	*longest_line = 0;
	*line_count = 0;
	i = 0;
	while (map_line[i])
	{
		if (map_line[i] == '\n')
		{
			*line_count += 1;
			if (current_line > *longest_line)
				*longest_line = current_line;
			current_line = 0;
		}
		i++;
		current_line++;
	}
	if (map_line[i - 1] != '\n' && !map_line[i])
		*line_count += 1;
}

int	get_line_length(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (line[i] == '\n')
			return (i);
		i++;
	}
	return (i - 1);
}

void	create_map_array(t_cub *cub, char *map_line)
{
	int	i;
	int	j;

	get_map_size(map_line, &cub->map.map_length, &cub->map.map_height);
	cub->map.map = ft_calloc(cub->map.map_height + 1, cub->map.map_length);
	if (!cub->map.map)
		map_error(cub, map_line, "Allocating map");
	i = 0;
	j = 0;
	while (map_line[i])
	{
		cub->map.map[j] = ft_substr(map_line, i,
				get_line_length(&map_line[i]) + 1);
		if (cub->map.map[j][ft_strlen(cub->map.map[j]) - 1] == '\n')
			cub->map.map[j][ft_strlen(cub->map.map[j]) - 1] = '\0';
		i += get_line_length(&map_line[i]) + 1;
		j++;
	}
	printf("map height : %d\n", cub->map.map_height);
	j = 0;
	while (cub->map.map[j])
		printf("map: %s\n", cub->map.map[j++]);
	free(map_line);
}

char	*get_next_written_line(int fd)
{
	char	*line;

	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			return (NULL);
		if (!ft_is_empty_line(line))
			return (line);
		free(line);
	}
}

int	is_eof(int fd)
{
	char	*line;

	line = get_next_written_line(fd);
	if (line)
	{
		free(line);
		return (0);
	}
	return (1);
}
