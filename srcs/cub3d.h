/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/12 19:00:27 by eozben            #+#    #+#             */
/*   Updated: 2022/03/17 22:14:16 by fbindere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include "../libft/libft.h"
# include "../mlx/mlx.h"
# include <stdio.h>
# include <fcntl.h>

# define ERROR -1
# define WIN_WIDTH 800
# define WIN_HEIGHT 600
# define ESC_KEY 53
# define UP_KEY 126
# define DOWN_KEY 125
# define LEFT_KEY 123
# define RIGHT_KEY 124
# define W_KEY 13
# define A_KEY 0
# define S_KEY 1
# define D_KEY 2

typedef struct s_map
{
	int		map_fd;
	int		map_height;
	int		map_length;
	char	*no_path;
	char	*so_path;
	char	*we_path;
	char	*ea_path;
	int		f_color;
	int		c_color;
	char	**map;
}			t_map;

typedef struct s_img
{
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}			t_img;

typedef struct s_win
{
	void	*mlx;
	t_map	*map;
	void	*mlx_win;
	t_img	img;
}			t_win;

int		ft_iswhitespace(char c);
int		ft_is_empty_line(char *s);
void	map_error(t_map *map, char *str, char *error_msg);
int		skip_whitespaces(char *str);
void	free_map(t_map *map);
void	read_cub_file(t_map *map, char **argv);
int		read_colour(t_map *map, char *str, int i);
int		read_textures(t_map *map);
int		init_window(t_win *win, t_img *img, t_map *map);
int		mlx_hooks(t_win *win);
void	read_map(t_map *map);
int		ft_is_whitespace(char c);
void	check_map_validity(t_map *map);

#endif