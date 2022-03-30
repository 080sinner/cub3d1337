/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 22:41:17 by eozben            #+#    #+#             */
/*   Updated: 2022/03/30 21:31:54 by fbindere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	ft_is_whitespace(char c)
{
	if (c == ' ' || c == '\t' || c == '\r')
		return (1);
	else
		return (0);
}

int	ft_is_empty_line(char *s)
{
	int	i;

	i = 0;
	while (ft_is_whitespace(s[i]))
		i++;
	if (s[i] == '\n')
		return (1);
	return (0);
}

int	skip_whitespaces(char *str)
{
	int	i;

	i = 0;
	while (ft_is_whitespace(str[i]))
		i++;
	return (i);
}

void	free_map(t_map *map)
{
	if (map->fd)
		close(map->fd);
	free(map->ea_path);
	free(map->no_path);
	free(map->so_path);
	free(map->we_path);
	free(map->map);
}

void	map_error(t_map *map, char *str, char *error_msg)
{
	if (map)
		free_map(map);
	if (str)
		free(str);
	printf("Error\n%s\n", error_msg);
	exit(EXIT_FAILURE);
}
