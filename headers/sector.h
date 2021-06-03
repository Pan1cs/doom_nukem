/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sector.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jnivala <jnivala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/02 14:08:38 by jnivala           #+#    #+#             */
/*   Updated: 2021/06/02 17:29:14 by jnivala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SECTOR_H
# define SECTOR_H

typedef struct s_point
{
	t_xy				x0;
	int					idx;
	float				wall_facing;
	t_xy				normal;
	t_height			height;
	t_xy				ground_uv;
	struct s_point		*next;
}						t_point;

typedef struct s_wall
{
	t_triangle			top;
	t_triangle			bottom;
	int					idx;
	float				wall_facing;
	t_xy				normal;
	struct s_wall		*next;
}						t_wall;

typedef struct s_surface
{
	t_triangle			tri;
	int					idx;
	float				wall_facing;
	t_xy				normal;
	struct s_surface	*next;
}						t_surface;

typedef struct s_skybox
{
	int					idx;
	t_triangle			*face;
}						t_skybox;

typedef struct s_point_data
{
	float		x;
	float		y;
	int			ground;
	int			ceiling;
	int			idx;
}				t_point_data;

typedef struct s_sector
{
	t_point			*points;
	t_wall			*walls;
	t_enemy			*enemies;
	unsigned int	nb_of_walls;
	int				idx_sector;
	int				tex_floor;
	int				tex_ceil;
	t_surface		*ground;
	t_surface		*ceiling;
}					t_sector;

enum e_sector_info
{
	old_sector = -6666
};

t_wall			*new_point(t_point_data *left, t_point_data *right);
void			add_point(t_wall **point, t_wall *new);

#endif
