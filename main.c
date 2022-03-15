/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/12 19:00:58 by eozben            #+#    #+#             */
/*   Updated: 2022/03/15 22:33:49 by fbindere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"


void	map_error(t_map *map, char *str, char *error_msg);

int	ft_iswhitespace(char c)
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
	while (ft_iswhitespace(s[i]))
		i++;
	if (s[i] == '\n')
		return (1);
	return (0);
}

// int	check_valid_map(int fd, char *str)
// {	
// 	(void)fd;
// 	if (!ft_check_pattern_middle(str))
// 		printf("   wrong pattern\n");
// 	else
// 		printf("   good\n");
// 	return (1);
// }

int	skip_whitespaces(char *str)
{
	int i;

	i = 0;
	while (ft_iswhitespace(str[i]))
		i++;
	return (i);
}

void	read_path(t_map *map, char indentifier, char *str, int i)
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

void	check_texture_path(t_map *map, char *str)
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
	static int ident_count;

	while (1)
	{
		str = get_next_line(map->map_fd);
		if (!str)
			break ;
		if (!ft_is_empty_line(str))
		{
			check_texture_path(map, str);
			ident_count++;
		}
		free(str);
		if (ident_count == 4)
			break ;
	}
	return (0);
}

void	free_map(t_map *map)
{
	free(map->ea_path);
	free(map->no_path);
	free(map->so_path);
	free(map->we_path);
	free(map->map);
}

void	map_error(t_map *map, char *str, char *error_msg)
{
	if (map->map_fd)
		close(map->map_fd);
	if (map)
		free_map(map);
	if (str)
		free(str);
	printf("Error\n%s\n", error_msg);
	exit(EXIT_FAILURE);
}

int	create_trgb(int t, int r, int g, int b)
{
	return (t << 24 | r << 16 | g << 8 | b);
}

int	read_colour(t_map *map, char *str, int i)
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
	while(colour_array[j])
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
	return (create_trgb(0, colour[0], colour[1], colour[2]));
}

void	check_floor_ceiling_colours(t_map *map, char *str)
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
	static int ident_count;

	while (1)
	{
		str = get_next_line(map->map_fd);
		if (!str)
			break ;
		if (!ft_is_empty_line(str))
		{
			check_floor_ceiling_colours(map, str);
			ident_count++;
		}
		free(str);
		if (ident_count == 2)
			break ;
	}
	return (0);
}


void	read_cub_file(t_map *map, char **argv)
{	
	map->map_fd = open(argv[1], O_RDWR);
	if (map->map_fd == ERROR)
	{
		printf("Error \nBad .cub file\n");
		exit(EXIT_FAILURE);
	}
	read_wall_texture(map);
	read_floor_ceiling_colour(map);
}

int main(int argc, char *argv[])
{
	t_map	map;
	(void)argc;
	(void)argv;
	if (argc != 2)
		return (printf("Error \nInvalid amount of arguments\n"));
	read_cub_file(&map, argv);
	printf("no texture: %s\n", map.no_path);
	printf("so texture: %s\n", map.so_path);
	printf("we texture: %s\n", map.we_path);
	printf("ea texture: %s\n", map.ea_path);
	printf("f_colour: %d\n", map.f_color);
	printf("c_colour: %d\n", map.c_color);
}
