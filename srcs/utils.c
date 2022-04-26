/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eozben <eozben@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 22:41:17 by eozben            #+#    #+#             */
/*   Updated: 2022/04/26 23:06:48 by eozben           ###   ########.fr       */
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

void	free_map(t_cub *cub)
{
	int	i;

	if (cub->map.fd)
		close(cub->map.fd);
	i = 0;
	while (i < 4)
		free(cub->map.paths[i++]);
	i = 0;
	while (i < 4 && cub->map.texture[i].img)
		mlx_destroy_image(cub->win.mlx, cub->map.texture[i++].img);
	i = 0;
	while (cub->map.map && cub->map.map[i])
		free(cub->map.map[i++]);
	free(cub->map.map);
}

void	map_error(t_cub *cub, char *str, char *error_msg)
{
	free_map(cub);
	if (str)
		free(str);
	printf("Error\n%s\n", error_msg);
	exit(EXIT_FAILURE);
}
