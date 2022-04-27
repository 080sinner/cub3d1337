/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eozben <eozben@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/06 16:41:45 by fbindere          #+#    #+#             */
/*   Updated: 2022/04/27 22:06:02 by eozben           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

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
