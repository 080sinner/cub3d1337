/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/12 19:00:27 by eozben            #+#    #+#             */
/*   Updated: 2022/04/27 23:39:39 by fbindere         ###   ########.fr       */
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
# define ENTER_KEY 36
# define FRAMETIME 0.016
# define MOVESPEED 0.08
# define ROTSPEED 0.048
# define MMAPZOOM 5
# define MMAPSIZE 0.3


typedef struct s_coord
{
	int	x;
	int	y;
}				t_coord;

typedef enum e_sprite_type
{
	BARREL,
	PILLAR,
	LAMP,
}			t_stype;

typedef enum e_dir
{
	NORTH,
	EAST,
	SOUTH,
	WEST,
	DOOR,
}			t_dir;

typedef enum e_side
{
	x_side,
	y_side,
}			t_side;

typedef struct s_point
{
	double		x;
	double		y;
}				t_point;

typedef struct s_ray
{
	t_point		dir;
	t_point		side_dist;
	t_point		delta_dist;
	t_side		hit;
	int			map_x;
	int			map_y;
	int			step_x;
	int			step_y;
	double		perp_wall_dist[WIN_WIDTH];
}				t_ray;

typedef struct s_camera
{
	t_point	plane;
	int		pressed_mb;
	int		pressed_keys[6];
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
	t_point	transf;
	t_stype	type;
	double	distance;
	int		scr_x;
	int		scr_y;
	int		height;
	int		width;
	int		start_x;
	int		end_x;
	int		start_y;
	int		end_y;
}			t_spr;

typedef struct s_minimap
{
	int		height;
	int		width;
	double	tile_width;
	double	tile_height;
}				t_mm;

typedef struct s_map
{
	int		fd;
	int		map_height;
	int		map_length;
	char	*paths[4];
	t_img	texture[5];
	t_img	sprites[3];
	t_img	enemy[5];
	t_img	doors[1];
	t_spr	map_spr[10];
	t_point	map_doors[3];
	t_mm	mmap;
	int		spr_count;
	int		door_count;
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

typedef struct s_dline
{
	int			height;
	int			start;
	int			end;
}				t_dline;

typedef struct s_text
{
	t_dir		dir;
	int			x;
	int			y;
	double		pos;
	double		step;
}				t_text;

int				ft_is_empty_line(char *s);
void			map_error(t_cub *cub, char *str, char *error_msg);
int				skip_whitespaces(char *str);
void			free_map(t_cub *cub);
void			parse_cub_file(t_cub *cub, char **argv);
int				read_colour(t_cub *cub, char *str, int i);
int				read_textures(t_cub *cub);
int				init_win_img(t_cub *cub);
int				mlx_hooks(t_cub *cub);
void			read_map(t_cub *cub);
int				ft_is_whitespace(char c);
void			check_map_validity(t_cub *cub, t_player *player);
void			ft_mlx_pixel_put(t_img *data, int x, int y, int color);
int				create_trgb(int r, int g, int b);
void			calculate_frame(t_cub *cub);
void			move_forward(t_cub *cub);
void			move_backward(t_cub *cub);
void			turn_right(t_cub *cub);
void			turn_left(t_cub *cub);
int				cub3d(t_cub *cub);
unsigned int	mlx_pixel_read(t_img *data, int x, int y);
void			open_texture_files(t_cub *cub);
void			set_camera_vector(t_cub *cub);
int				is_player(char tile);
void			move_right(t_cub *cub);
void			move_left(t_cub *cub);
void			cast_sprites(t_cub *cub, t_ray *ray);
int				key_hooks(t_cub *cub);
void			door(t_cub *cub);
void			open_xpm_file(int ident, char *path, t_img *img, t_cub *cub);
void			set_ray_dir_vector(t_cub *cub, t_ray *ray, int x);
void			set_delta_dist(t_ray *ray);
void			set_side_dist(t_ray *ray, t_player *player);
void			move_forward(t_cub *cub);
void			move_backward(t_cub *cub);
int				is_obstacle(char c);
int				mouse_move(int x, int y, t_cub *cub);
int				press_keys(int keycode, t_cub *cub);
int				key_hooks(t_cub *cub);
int				release_keys(int keycode, t_cub *cub);
int				unregister_mouseclick(int button, int x, int y, t_cub *cub);
int				register_mouseclick(int button, int x, int y, t_cub *cub);
void			get_text_type(t_cub *cub, t_ray *ray, t_text *text);
void			get_text_values(t_cub *cub, t_dline *line, t_text *text);
void			get_texture_x(t_cub *cub, t_ray *ray, t_text *text, int x);
void			get_line_values(t_dline *line, t_ray *ray, int x);
void			draw_minimap(t_cub *cub);
void			door(t_cub *cub);
void			colour_pixel(t_cub *cub, int x, int y);
void			get_map_size(char *map_line, int *longest_line,
					int *line_count);
int				get_line_length(char *line);
void			create_map_array(t_cub *cub, char *map_line);
char			*get_next_written_line(int fd);
int				is_eof(int fd);
int				is_empty_tile(char tile);
void			check_valid_zero(t_cub *cub, int x, int y);
int				is_player(char tile);
int				check_x_border(char *s, int i);
int				is_sprite(char c);
char			*append_mapline(t_cub *cub, char *map_line, char *line);
int				close_win(t_cub *cub);

#endif
