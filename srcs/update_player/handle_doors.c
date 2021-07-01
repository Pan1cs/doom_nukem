/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_doors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jnivala <jnivala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/21 12:18:32 by jnivala           #+#    #+#             */
/*   Updated: 2021/07/01 13:58:17 by jnivala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/doom_nukem.h"

static void	lock_the_door(t_wall *wall)
{
	wall->top.p[0] = (t_xyz){wall->top.p[0].x, (int)wall->top.p[0].y,
		wall->top.p[0].z, 1.0f};
	wall->bottom.p[1] = (t_xyz){wall->bottom.p[1].x,
		ceilf(wall->bottom.p[1].y), wall->bottom.p[1].z, 1.0f};
	wall->top.uv[1].v = 0.0f;
	wall->top.uv[2].v = 0.0f;
	wall->bottom.uv[0].v = 1.0f;
	wall->bottom.uv[2].v = 1.0f;
}

static void	translate_door(t_wall *wall, char dir, float speed, Uint32 t)
{
	t_xyz	translation_top;
	t_xyz	translation_bottom;

	if (dir == 'o')
	{
		translation_top = (t_xyz){0.0f, speed * t * 0.001f, 0.0f, 1.0f};
		translation_bottom = (t_xyz){0.0f, -speed * t * 0.001f, 0.0f, 1.0f};
		wall->top.uv[1].v += t * 0.0002f;
		wall->top.uv[2].v += t * 0.0002f;
		wall->bottom.uv[0].v -= t * 0.0002f;
		wall->bottom.uv[2].v -= t * 0.0002f;
	}
	else if (dir == 'c')
	{
		translation_top = (t_xyz){0.0f, -speed * t * 0.001f, 0.0f, 1.0f};
		translation_bottom = (t_xyz){0.0f, speed * t * 0.001f, 0.0f, 1.0f};
		wall->top.uv[1].v -= t * 0.0002f;
		wall->top.uv[2].v -= t * 0.0002f;
		wall->bottom.uv[0].v += t * 0.0002f;
		wall->bottom.uv[2].v += t * 0.0002f;
	}
	wall->top.p[0] = translate_point(&wall->top.p[0], translation_top);
	wall->bottom.p[1] = translate_point(&wall->bottom.p[1], translation_bottom);
}

static Uint32	handle_door_logic(t_wall *wall, Uint32 current_time,
			Uint32 delta_time)
{
	if (wall->open_until < current_time)
	{
		lock_the_door(wall->next);
		wall->is_closed = 1;
		return (1);
	}
	else
	{
		if (wall->open_until - 2500 > current_time)
			translate_door(wall->next, 'o', wall->next->height * 0.2f,
				delta_time);
		else
		{
			wall->is_closed = 0;
			translate_door(wall->next, 'c', wall->next->height * 0.2f,
				delta_time);
		}
	}
	return (0);
}

Uint32	close_doors(t_sector **sectors, Uint32 nb_of_sectors,
		Uint32 current_time, Uint32 delta_time)
{
	Uint32	i;
	Uint32	walls;
	t_wall	*wall;
	Uint32	volume;

	i = 0;
	volume = 0;
	while (i < nb_of_sectors)
	{
		walls = sectors[i]->nb_of_walls;
		wall = sectors[i]->walls;
		while (walls)
		{
			if (wall->is_door)
				volume += handle_door_logic(wall, current_time, delta_time);
			wall = wall->next;
			walls--;
		}
		i++;
	}
	return (volume);
}

int	open_door(t_sector **sectors, t_xyz look_loc,
	Uint32 current_time, Uint32 cur_sector)
{
	t_wall			*wall;
	t_wall			*portal_behind;

	wall = check_if_crossing(sectors[cur_sector], look_loc);
	if (wall && wall->is_door)
	{
		portal_behind = get_portal_by_idx(cur_sector,
				sectors[wall->top.idx]);
		portal_behind->open_until = current_time + 5000;
		wall->open_until = current_time + 5000;
		return (1);
	}
	return (0);
}
