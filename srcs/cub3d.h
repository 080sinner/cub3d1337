/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/12 19:00:27 by eozben            #+#    #+#             */
/*   Updated: 2022/04/21 23:42:48 by fbindere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include "../libft/libft.h"
# include "../mlx/mlx.h"
# include <stdio.h>
# include <fcntl.h>
# include <math.h>
# include <stdbool.h>

# define ERROR -1
# define WIN_WIDTH 800
# define WIN_HEIGHT 600
# define ESC_KEY 53
# define UP_KEY 126
# define DOWN_KEY 125
# define LEFT_KEY 123
# define RIGHT_KEY 124
# define ON_KEYDOWN 2
# define W_KEY 13
# define A_KEY 0
# define S_KEY 1
# define D_KEY 2
# define FRAMETIME 0.016
# define MOVESPEED 0.08
# define ROTSPEED 0.048

typedef enum e_dir
{
	NORTH,
	EAST,
	SOUTH,
	WEST,
}			t_dir;

typedef enum e_side
{
	xSide,
	ySide,
}			t_side;

typedef struct s_point
{
	double		x;
	double		y;
}				t_point;

typedef struct s_ray
{
	t_point		dir;
	t_point		sideDist;
	t_point		deltaDist;
	t_side		hit;
	int			mapX;
	int			mapY;
	int			stepX;
	int			stepY;
	double		perpWallDist[WIN_WIDTH];
}				t_ray;

typedef struct s_camera
{
	t_point	plane;
}				t_camera;

typedef struct s_player
{
	t_point	pos;
	t_point	dir;
}				t_player;

typedef struct s_img
{
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
	int		height;
	int		width;
}			t_img;

typedef struct s_spr
{
	t_point	coord;
	t_point transf;
	int		type;
	double	distance;
	int		scrX;
	int		scrY;
	int		height;
	int		width;
	int		startX;
	int		endX;
	int		startY;
	int		endY;
}			t_spr;

typedef struct s_map
{
	int		fd;
	int		map_height;
	int		map_length;
	char	*paths[4];
	t_img	texture[4];
	t_img	sprites[3];
	t_spr	map_spr[10];
	t_point	doors[3];
	int		f_color;
	int		c_color;
	t_dir	p_dir;
	char	**map;
}			t_map;

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

typedef struct	s_dline
{
	int			height;
	int			start;
	int			end;
}				t_dline;

typedef struct	s_text
{
	t_dir		dir;
	int			x;
	int			y;
	double		pos;
	double		step;
}				t_text;

int		ft_is_empty_line(char *s);
void	map_error(t_cub *cub, char *str, char *error_msg);
int		skip_whitespaces(char *str);
void    free_map(t_cub *cub);
void	parse_cub_file(t_cub *cub, char **argv);
int		read_colour(t_cub *cub, char *str, int i);
int		read_textures(t_cub *cub);
int		init_win_img(t_cub *cub);
int		mlx_hooks(t_cub *cub);
void	read_map(t_cub *cub);
int		ft_is_whitespace(char c);
void	check_map_validity(t_cub *cub, t_player *player);
void	ft_mlx_pixel_put(t_img *data, int x, int y, int color);
int		create_trgb(int r, int g, int b);
void	calculate_frame(t_cub *cub);
void	move_forward(t_cub *cub);
void	move_backward(t_cub *cub);
void	turn_right(t_cub *cub);
void	turn_left(t_cub *cub);
void	cub3d(t_cub *cub);
unsigned int mlx_pixel_read(t_img *data, int x, int y);
void	open_texture_files(t_cub *cub);
void	set_camera_vector(t_cub *cub);
int		is_player(char tile);
void	move_right(t_cub *cub);
void	move_left(t_cub *cub);
void	cast_sprites(t_cub *cub, t_ray *ray);

#endif
