/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/12 19:00:27 by eozben            #+#    #+#             */
/*   Updated: 2022/04/28 19:19:34 by fbindere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include "../../libft/libft.h"
# include "../../mlx/mlx.h"
# include "cub3d_defines.h"
# include "cub3d_structs.h"
# include <stdio.h>
# include <fcntl.h>
# include <math.h>
# include <stdbool.h>

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
void			open_textures(t_cub *cub);
int				read_textures(t_cub *cub);

#endif
