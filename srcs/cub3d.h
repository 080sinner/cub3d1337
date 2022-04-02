/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/12 19:00:27 by eozben            #+#    #+#             */
/*   Updated: 2022/04/02 20:07:56 by fbindere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include "../libft/libft.h"
# include "../mlx/mlx.h"
# include <stdio.h>
# include <fcntl.h>
# include <math.h>

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

typedef struct s_camera
{
	double	plane_x;
	double	plane_y;
	double	time;
	double	time_old;
}				t_camera;

typedef struct s_player
{
	double	pos_x;
	double	pos_y;
	double	dir_x;
	double	dir_y;
}				t_player;

typedef struct s_map
{
	int		fd;
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
	void	*mlx_win;
}			t_win;

typedef struct s_cub
{
	t_camera	camera;
	t_map		map;
	t_player	player;
	t_img		img;
	t_win		win;
}				t_cub;

int		ft_is_empty_line(char *s);
void	map_error(t_map *map, char *str, char *error_msg);
int		skip_whitespaces(char *str);
void	free_map(t_map *map);
void	parse_cub_file(t_cub *cub, char **argv);
int		read_colour(t_map *map, char *str, int i);
int		read_textures(t_map *map);
int		init_win_img(t_win *win, t_img *img);
int		mlx_hooks(t_cub *cub);
void	read_map(t_map *map);
int		ft_is_whitespace(char c);
void	check_map_validity(t_map *map, t_player *player);
int		create_trgb(int r, int g, int b);
void	ft_mlx_pixel_put(t_img *data, int x, int y, int color);
int		calculate_frame(t_cub *cub);


#endif