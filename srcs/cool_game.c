/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cool_game.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/06 16:41:45 by fbindere          #+#    #+#             */
/*   Updated: 2022/04/27 17:20:57 by fbindere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	set_ray_dir_vector(t_cub *cub, t_ray *ray, int x)
{
	double	camera_x;

	camera_x = 2 * x / (double)WIN_WIDTH - 1;
	ray->dir.x = cub->player.dir.x + cub->camera.plane.x * camera_x;
	ray->dir.y = cub->player.dir.y + cub->camera.plane.y * camera_x;
}

void	set_delta_dist(t_ray *ray)
{
	if (ray->dir.x == 0)
		ray->delta_dist.x = INFINITY;
	else
		ray->delta_dist.x = fabs(1 / ray->dir.x);
	if (ray->dir.y == 0)
		ray->delta_dist.y = INFINITY;
	else
		ray->delta_dist.y = fabs(1 / ray->dir.y);
}

void	set_side_dist(t_ray *ray, t_player *player)
{
	if (ray->dir.x < 0)
	{
		ray->side_dist.x = (player->pos.x - ray->map_x) * ray->delta_dist.x;
		ray->step_x = -1;
	}
	else
	{
		ray->side_dist.x = (ray->map_x + 1 - player->pos.x) * ray->delta_dist.x;
		ray->step_x = 1;
	}
	if (ray->dir.y < 0)
	{
		ray->side_dist.y = (player->pos.y - ray->map_y) * ray->delta_dist.y;
		ray->step_y = -1;
	}
	else
	{
		ray->side_dist.y = (ray->map_y + 1 - player->pos.y) * ray->delta_dist.y;
		ray->step_y = 1;
	}
}

int	is_obstacle(char c)
{
	if (c == '1' || c == 'D')
		return (1);
	return (0);
}

void	perform_dda(t_ray *ray, t_cub *cub, int x)
{
	int	hit;

	hit = 0;
	while (hit == 0)
	{
		if (ray->side_dist.x < ray->side_dist.y)
		{
			ray->side_dist.x += ray->delta_dist.x;
			ray->map_x += ray->step_x;
			ray->hit = x_side;
		}
		else
		{
			ray->side_dist.y += ray->delta_dist.y;
			ray->map_y += ray->step_y;
			ray->hit = y_side;
		}
		if (is_obstacle(cub->map.map[ray->map_y][ray->map_x]))
			hit = 1;
	}
	if (ray->hit == x_side)
		ray->perp_wall_dist[x] = ray->side_dist.x - ray->delta_dist.x;
	else
		ray->perp_wall_dist[x] = ray->side_dist.y - ray->delta_dist.y;
}

void	get_texture_x(t_cub *cub, t_ray *ray, t_text *text, int x)
{
	double	wall_x;

	if (ray->hit == x_side)
		wall_x = cub->player.pos.y + ray->perp_wall_dist[x] * ray->dir.y;
	else
		wall_x = cub->player.pos.x + ray->perp_wall_dist[x] * ray->dir.x;
	wall_x -= floor(wall_x);
	text->x = (int)(wall_x * (double)cub->map.texture[text->dir].width);
	if (ray->hit == x_side && ray->dir.x > 0)
		text->x = cub->map.texture[text->dir].width - text->x - 1;
	if (ray->hit == y_side && ray->dir.y < 0)
		text->x = cub->map.texture[text->dir].width - text->x - 1;
}

void	get_line_values(t_dline *line, t_ray *ray, int x)
{
	line->height = (int)(WIN_HEIGHT / ray->perp_wall_dist[x]);
	line->start = -line->height / 2 + WIN_HEIGHT / 2;
	if (line->start < 0)
		line->start = 0;
	line->end = line->height / 2 + WIN_HEIGHT / 2;
	if (line->end >= WIN_HEIGHT)
		line->end = WIN_HEIGHT - 1;
}

void	get_text_values(t_cub *cub, t_dline *line, t_text *text)
{
	text->step = 1.0 * cub->map.texture[text->dir].height / line->height;
	text->pos = (line->start - WIN_HEIGHT / 2 + line->height / 2) * text->step;
}

void	get_text_type(t_cub *cub, t_ray *ray, t_text *text)
{
	if (cub->map.map[ray->map_y][ray->map_x] == 'D')
		text->dir = DOOR;
	else if (ray->hit == y_side)
	{
		if (ray->dir.y >= 0)
			text->dir = NORTH;
		else
			text->dir = SOUTH;
	}
	else if (ray->hit == x_side)
	{
		if (ray->dir.x >= 0)
			text->dir = EAST;
		else
			text->dir = WEST;
	}
}

void	draw_line(t_ray *ray, t_cub *cub, int x)
{
	t_dline			line;
	t_text			text;	
	unsigned int	color;
	int				y;

	get_line_values(&line, ray, x);
	get_text_type(cub, ray, &text);
	get_text_values(cub, &line, &text);
	get_texture_x(cub, ray, &text, x);
	y = line.start;
	while (y < line.end)
	{
		text.y = (int)text.pos & (cub->map.texture[text.dir].height - 1);
		text.pos += text.step;
		color = mlx_pixel_read(&cub->map.texture[text.dir], text.x, text.y);
		if (ray->hit == y_side)
			color = (color >> 1) & 8355711;
		ft_mlx_pixel_put(&cub->img, x, y, color);
		y++;
	}
}

void	cast_walls(t_cub *cub, t_ray *ray)
{
	int		x;

	x = 0;
	while (x < WIN_WIDTH)
	{
		set_ray_dir_vector(cub, ray, x);
		ray->map_x = (int)cub->player.pos.x;
		ray->map_y = (int)cub->player.pos.y;
		set_delta_dist(ray);
		set_side_dist(ray, &cub->player);
		perform_dda(ray, cub, x);
		draw_line(ray, cub, x);
		x++;
	}
}

void	turn_left(t_cub *cub)
{
	double	old_dir_x;
	double	old_plane_x;

	old_dir_x = cub->player.dir.x;
	old_plane_x = cub->camera.plane.x;
	cub->player.dir.x = cub->player.dir.x * cos(ROTSPEED)
		- cub->player.dir.y * sin(ROTSPEED);
	cub->player.dir.y = old_dir_x * sin(ROTSPEED)
		+ cub->player.dir.y *cos(ROTSPEED);
	cub->camera.plane.x = cub->camera.plane.x * cos(ROTSPEED)
		- cub->camera.plane.y * sin(ROTSPEED);
	cub->camera.plane.y = old_plane_x * sin(ROTSPEED)
		+ cub->camera.plane.y * cos(ROTSPEED);
}

void	turn_right(t_cub *cub)
{
	double	old_dir_x;
	double	old_plane_x;

	old_dir_x = cub->player.dir.x;
	old_plane_x = cub->camera.plane.x;
	cub->player.dir.x = cub->player.dir.x * cos(-ROTSPEED)
		- cub->player.dir.y * sin(-ROTSPEED);
	cub->player.dir.y = old_dir_x * sin(-ROTSPEED)
		+ cub->player.dir.y *cos(-ROTSPEED);
	cub->camera.plane.x = cub->camera.plane.x * cos(-ROTSPEED)
		- cub->camera.plane.y * sin(-ROTSPEED);
	cub->camera.plane.y = old_plane_x * sin(-ROTSPEED)
		+ cub->camera.plane.y * cos(-ROTSPEED);
}

void	move_forward(t_cub *cub)
{
	int	newpos_x;
	int	newpos_y;

	newpos_x = (int)(cub->player.pos.x + cub->player.dir.x * MOVESPEED);
	newpos_y = (int)(cub->player.pos.y + cub->player.dir.y * MOVESPEED);
	if (!is_obstacle(cub->map.map[(int)cub->player.pos.y][newpos_x]))
		cub->player.pos.x += cub->player.dir.x * MOVESPEED;
	if (!is_obstacle(cub->map.map[newpos_y][(int)cub->player.pos.x]))
		cub->player.pos.y += cub->player.dir.y * MOVESPEED;
}

void	move_right(t_cub *cub)
{
	int	newpos_x;
	int	newpos_y;

	newpos_x = (int)(cub->player.pos.x + cub->camera.plane.x * MOVESPEED);
	newpos_y = (int)(cub->player.pos.y + cub->camera.plane.y * MOVESPEED);
	if (!is_obstacle(cub->map.map[(int)cub->player.pos.y][newpos_x]))
		cub->player.pos.x += cub->camera.plane.x * MOVESPEED;
	if (!is_obstacle(cub->map.map[newpos_y][(int)cub->player.pos.x]))
		cub->player.pos.y += cub->camera.plane.y * MOVESPEED;
}

void	move_left(t_cub *cub)
{
	int	newpos_x;
	int	newpos_y;

	newpos_x = (int)(cub->player.pos.x - cub->camera.plane.x * MOVESPEED);
	newpos_y = (int)(cub->player.pos.y - cub->camera.plane.y * MOVESPEED);
	if (!is_obstacle(cub->map.map[(int)cub->player.pos.y][newpos_x]))
		cub->player.pos.x -= cub->camera.plane.x * MOVESPEED;
	if (!is_obstacle(cub->map.map[newpos_y][(int)cub->player.pos.x]))
		cub->player.pos.y -= cub->camera.plane.y * MOVESPEED;
}

void move_backward(t_cub *cub)
{
	int	newpos_x;
	int	newpos_y;

	newpos_x = (int)(cub->player.pos.x - cub->player.dir.x * MOVESPEED);
	newpos_y = (int)(cub->player.pos.y - cub->player.dir.y * MOVESPEED);
	if (!is_obstacle(cub->map.map[(int)cub->player.pos.y][newpos_x]))
		cub->player.pos.x -= cub->player.dir.x * MOVESPEED;
	if (!is_obstacle(cub->map.map[newpos_y][(int)cub->player.pos.x]))
		cub->player.pos.y -= cub->player.dir.y * MOVESPEED;
}

void	cast_floor_ceiling(t_cub *cub)
{
	unsigned int	color;
	int				y;
	int				x;

	y = 0;
	while (y < WIN_HEIGHT)
	{
		x = 0;
		while (x < WIN_WIDTH)
		{
			color = cub->map.f_color;
			color = (color >> 1) & 8355711;
			ft_mlx_pixel_put(&cub->img, x, y, color);
			color = cub->map.c_color;
			color = (color >> 1) & 8355711;
			ft_mlx_pixel_put(&cub->img, x, WIN_HEIGHT - y - 1, color);
			x++;
		}
		y++;
	}
}

void	colour_pixel(t_cub *cub, int x, int y)
{
	int map_y;
	int map_x;
	
	map_y = (int)((y / cub->map.mmap.tile_height) + 
		cub->player.pos.y - MMAPZOOM);
	map_x = (int)((x / cub->map.mmap.tile_width) + 
		cub->player.pos.x - MMAPZOOM);
	if (map_x >= 0 && map_x <= cub->map.map_length - 1 &&
	map_y >= 0 && map_y <= cub->map.map_height - 1 &&
	cub->map.map[map_y][map_x] == '1')
		ft_mlx_pixel_put(&cub->img, x, y, 0);
	else if (map_x == (int)cub->player.pos.x && 
		map_y == (int)cub->player.pos.y)
		ft_mlx_pixel_put(&cub->img, x, y, 25600);
}


void	draw_minimap(t_cub *cub)
{
	int x;
	int y;

	cub->map.mmap.tile_width = cub->map.mmap.width / (2 * MMAPZOOM + 1);
	cub->map.mmap.tile_height = cub->map.mmap.height / (2 * MMAPZOOM + 1);
	y = 0;
	while(y < cub->map.mmap.height)
	{
		x = 0;
		while(x < cub->map.mmap.width)
		{
			colour_pixel(cub, x, y);
			x++;
		}
		y++;
	}
}

void	door(t_cub *cub)
{
	int x;
	int y;
	x = (int)cub->player.pos.x + cub->player.dir.x;
	y = (int)cub->player.pos.y + cub->player.dir.y;

	if (x !=  cub->player.pos.x && y != cub->player.pos.y)
	{
		if (cub->map.map[y][x] == 'D')
			cub->map.map[y][x] = 'O';
		else if (cub->map.map[y][x] == 'O')
			cub->map.map[y][x] = 'D';
	}
}

int	cub3d(t_cub *cub)
{
	t_ray	ray;

	key_hooks(cub);
	cast_floor_ceiling(cub);
	cast_walls(cub, &ray);
	cast_sprites(cub, &ray);
	draw_minimap(cub);
	mlx_put_image_to_window(cub->win.mlx, cub->win.mlx_win, cub->img.img, 0, 0);
	return (0);
}
