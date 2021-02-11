/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scan_fov.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jnivala <jnivala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/04 07:59:30 by jnivala           #+#    #+#             */
/*   Updated: 2021/02/10 11:43:28 by jnivala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../doom_nukem.h"


static float	ft_interpolate_y(t_xy p0, t_xy p1)
{
	if (p1.x - p0.x == 0)
		return (0);
	return (p0.y + (0 - p0.x) * ((p1.y - p0.y) / (p1.x - p0.x)));
}

static void		ft_draw_wall(t_xy left, t_xy right, t_frame *frame, float current_angle, int color)
{
	float		screen_wall;
	float		screen_offset;
	float		euc_offset;
	float		wall_height_left;
	float		wall_height_right;
	float		distance_left;
	float		distance_right;
	float		diff;
	float		step;
	int			i;

	i = 0;
	screen_wall = SCREEN_WIDTH / FOV * (current_angle * RAD_TO_DEG);
	screen_offset = SCREEN_WIDTH / FOV * (FOV - frame->offset * RAD_TO_DEG);
	euc_offset = screen_offset < 319 ? screen_offset - 320 : screen_offset;
	distance_left = vec2_mag(left) * (1 - euc_offset * .00224966501709f);
	if (right.x < 0)
	{
		right.y = ft_interpolate_y(left, right);
		right.x = 0;
	}
	distance_right = vec2_mag(right) * (1 - (euc_offset + screen_wall) * 0.0009152913088125);
	if ((int)left.y == 0)
		wall_height_left = 0;
	else
		wall_height_left = 500 / left.y * 10;
	if ((int)right.y == 0)
		wall_height_right = 0;
	else
		wall_height_right = 500 / right.y * 10;
	diff = wall_height_left - wall_height_right;
	step = diff / screen_wall;
	while (i < (int)screen_wall)
	{
		draw_horizon(
			vec2(screen_offset + i, 50 - wall_height_left),
			vec2(screen_offset + i, 50 + wall_height_left),
			color,
			frame->draw_surf);
		i++;
		wall_height_left = wall_height_left - step;
	}
}

int				draw_horizon(t_xy start, t_xy end, int color, SDL_Surface *surf)
{
	t_xy	length;
	t_xy	ratio;
	int		pixels;
	t_xy	pos;

	length = vec2(fabs(end.x - start.x), fabs(end.y - start.y));
	pixels = (length.x > length.y) ? (length.x) : (length.y);
	ratio.x = (start.y != end.y) ? (length.x / length.y) : 1;
	ratio.y = (start.x != end.x) ? (length.y / length.x) : 1;
	ratio.x = (ratio.x > ratio.y) ? 1 : (ratio.x);
	ratio.y = (ratio.y > ratio.x) ? 1 : (ratio.y);
	pos.x = start.x;
	pos.y = start.y;
	while (pixels-- > 0)
	{
		if (pos.x >= 0 && pos.x < SCREEN_WIDTH && pos.y >= 0 && pos.y < SCREEN_HEIGHT)
			put_pixel(surf, pos.x, pos.y, color);
		pos.x += ratio.x * ((start.x < end.x) ? 1 : -1);
		pos.y += ratio.y * ((start.y < end.y) ? 1 : -1);
	}
	return (TRUE);
}

void			scan_fov(t_home *home, t_frame *frame)
{
	t_ray_fov	fov;
	t_frame		new_frame;
	float		current_angle;

	current_angle = 0.0f;
	fov.left_point = vec2(-1,-1);
	fov.left_wall = home->sectors[frame->idx]->points;
	continue_from_last_sector(fov.left_wall, &fov, frame);
	get_left_point(fov.left_wall, &fov, frame, home->sectors[frame->idx]->nb_of_walls);
	while (frame->offset >= frame->max_fov)
	{
		if (current_angle != 0)
			continue_from_next_point(fov.left_wall, &fov, frame);
		current_angle = vec2_angle(fov.left_point, fov.right_point);
		if (check_if_portal(fov.left_wall, frame) && !check_if_same_point(current_angle, &fov))
		{
			setup_frame(frame, &new_frame, current_angle, fov.left_wall->idx);
			scan_fov(home, &new_frame);
			frame->offset = new_frame.offset;
		}
		else
		{
			ft_draw_line(
				vec2_add(fov.left_point, home->offset),
				vec2_add(fov.right_point, home->offset),
				green,
				frame->draw_surf);
			ft_draw_wall(fov.left_point, fov.right_point, frame, current_angle, 0xFF8000 + frame->offset * RAD_TO_DEG * 100);
			current_angle = (current_angle < frame->min_step) ? frame->min_step : current_angle;
			frame->offset = frame->offset - current_angle;
		}
	}
}
