/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/12 19:00:27 by eozben            #+#    #+#             */
/*   Updated: 2022/03/15 21:22:31 by fbindere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include "libft/libft.h"
# include "mlx/mlx.h"
# include <stdio.h>
# include <fcntl.h>

#define	ERROR -1

typedef struct s_map
{
	int		map_fd;
	char	*no_path;
	char	*so_path;
	char	*we_path;
	char	*ea_path;
	int		f_color;
	int		c_color;
	char	**map;
}			t_map;

#endif