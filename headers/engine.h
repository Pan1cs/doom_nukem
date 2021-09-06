/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jnivala <jnivala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/26 16:58:35 by jnivala           #+#    #+#             */
/*   Updated: 2021/09/06 16:14:06 by jnivala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENGINE_H
# define ENGINE_H

typedef struct s_ray_pt
{
	t_xy			dir;
	t_xy			isection;
	t_wall			*wall;
}					t_ray_pt;

typedef struct s_frame
{
	t_raster_queue	**raster_queue;
	t_raster_queue	*triangles_in_view;
	t_raster_queue	*transformed;
	t_buffer		buffer;
	float			*depth_buffer;
	Uint32			last_frame;
	t_ray_pt		left;
	t_ray_pt		right;
	t_sides			viewport;
	int				idx;
	int				old_idx;
}					t_frame;

enum e_lines
{
	colinear = 0,
	clockwise = 1,
	c_clockwise = 2,
};

void			calc_intersection(t_wall *pgon, t_ray *ray,
					t_intersection *sect);
void			calc_sector_bounds(t_sector *sector);
void			calc_extra_walls(t_home *home);
t_bool			check_connection(t_wall *point, t_frame *frame);
t_bool			check_if_portal(t_wall *point);
int				check_if_same_pt(int *current_pxl, t_ray_pt *fov);
int				check_if_lseg_intersects(t_xy *p0, t_xy *p1, t_xy *pos,
					t_xy *dir);
void			continue_from_last_sector(t_wall *start, t_ray_pt *fov,
					t_frame *frame);
int				get_next_wall_tex(t_wall **current_head, int nbr_of_walls);
t_texel			*get_tex(int idx, t_texture	**textures);
void			get_wall_pts(t_frame *frame, int walls, t_player *plr);
void			get_l_pt(t_wall *start, t_ray ray, t_frame *frame, int walls);
void			get_r_pt(t_wall *start, t_ray ray, t_frame *frame, int walls);
float			get_wall_height(float left_ground, float right_ground,
					float left_ceiling, float right_ceiling);
t_wall			*get_portal_by_idx(int idx, t_sector *sector);
t_bool			vec2_get_scalar_to_intersection(t_xy pos, t_xy dir,
					t_wall *wall, float *t);
void			interpolate_y(t_height *height, t_xy cutpoint,
					t_wall *p0, t_wall *p1);
void			interpolate_uv(t_xy *ground_uv, t_xy cutpoint,
					t_wall *p0, t_wall *p1);
t_xy			line_intersection(t_intersection *sect);
void			precalc_ground_texels(t_home *home);
void			scan_fov(t_home *home, t_frame *frame, t_player *plr);
void			setup_frame(t_frame *frame, t_frame *new_frame, int idx);
void			step_one(t_frame *frame);
void			*clip_to_viewport_edges(void *args);
void			free_queues(t_frame *frame);
t_raster_queue	*delete_raster_queue(t_raster_queue **queue);
t_raster_queue	*create_raster_queue(size_t capacity);
int				raster_queue_is_full(t_raster_queue *queue);
int				raster_queue_is_empty(t_raster_queue *queue);
int				enqueue_to_raster(t_raster_queue *queue, t_triangle *item);
int				dequeue(t_raster_queue *queue);
int				front(t_raster_queue *queue, t_triangle *triangle);
int				orientation_of_three_points(t_xy a, t_xy b, t_xy c);
int				point_is_on_the_lseg(t_xy a, t_xy c, t_xy b);
int				check_intersection(t_wall *p0, t_xy pos, t_xy dir);
void			quick_reset_queue(t_raster_queue *queue);
void			draw_queue_empty(t_frame *frame, t_home *home,
					t_player *plr, int *idx);
void			add_ceiling(t_home *home, t_frame *frame, t_player *plr);
void			add_ground(t_home *home, t_frame *frame, t_player *plr);
void			add_walls(t_home *home, t_frame *frame, t_player *plr);
void			add_entities(t_home *home, t_frame *frame, t_player *plr);
void			add_projectiles(t_home *home, t_frame *frame, t_player *plr);
t_wall			*check_if_too_close_to_walls(t_sector *sector, float width,
					t_xyz pos, t_xyz dir);
void			check_if_moved_through_portal(int *cur_sector, t_xyz pos,
					t_home *home);
#endif