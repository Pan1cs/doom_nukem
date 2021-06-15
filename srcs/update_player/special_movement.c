/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   special_movement.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jnivala <jnivala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/11 16:02:45 by jnivala           #+#    #+#             */
/*   Updated: 2021/06/15 10:08:37 by jnivala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/doom_nukem.h"

static void	set_animation_on(Uint32 *start, Uint32 *end_time,
	Uint32 *current, Uint32 duration)
{
	if (!*start)
	{
		*start = 1;
		*end_time = *current + duration;
	}
}

static void	set_height(t_player *plr, char direction)
{
	if (direction == 'u')
	{
		plr->height += 0.1f;
		if (plr->height > 1.5f)
			plr->height = 1.5f;
		else
			plr->pos.y += 0.1f;
	}
	else if (direction == 'd')
	{
		plr->height -= 0.1f;
		if (plr->height <= 0.6f)
			plr->height = 0.6f;
		else
			plr->pos.y -= 0.1f;
	}
	else
		return ;
}

static void	set_jump_sequence(t_player *plr, Uint32 *animation_end,
	float *jump_range)
{
	if ((*animation_end - plr->time) > 400)
	{
		plr->pos.y -= 0.05f;
		*jump_range -= 0.05f;
	}
	else
	{
		plr->pos.y += 0.05f;
		*jump_range += 0.05f;
	}
}

void	crouch(t_player *plr)
{
	static Uint32	animation_start = 0;
	static Uint32	animation_end = 0;

	if (plr->input.crouch == 1)
	{
		set_animation_on(&animation_start, &animation_end, &plr->time, 100);
		if (plr->time <= animation_end)
			set_height(plr, 'd');
		else
			animation_start = 0;
	}
	else if (plr->input.crouch == 0)
	{
		set_animation_on(&animation_start, &animation_end, &plr->time, 100);
		if (plr->time <= animation_end)
			set_height(plr, 'u');
		else
			animation_start = 0;
	}
	else
		return ;
}

void	jump(t_player *plr, t_sector *sector)
{
	static Uint32	animation_start = 0;
	static Uint32	animation_end = 0;
	static float	jump_range = 0.0f;

	if (plr->input.jump == 1)
	{
		if (check_distance_to_ground(sector, plr, plr->pos) <= 0)
			set_animation_on(&animation_start, &animation_end, &plr->time, 500);
		if (plr->time <= animation_end && jump_range <= 1.0f)
			set_jump_sequence(plr, &animation_end, &jump_range);
		else
		{
			animation_start = 0;
			jump_range = 0.0f;
			plr->input.jump = 0;
		}
	}
}
