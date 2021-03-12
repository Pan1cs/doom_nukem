/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calc_distances.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jnivala <joonas.hj.nivala@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/02 09:27:42 by jnivala           #+#    #+#             */
/*   Updated: 2021/03/12 17:09:10 by jnivala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../doom_nukem.h"

void	calc_distances(t_frame *frame, t_texture *tex, t_player *plr)
{
	float	z1;
	float	z2;
	
	z1 = vec2_perp_dist(frame->left.l_pt);
	z2 = vec2_perp_dist(frame->left.r_pt);
	frame->top_left.x = SCREEN_WIDTH - ((SCREEN_HEIGHT /
		z1) * frame->left.l_pt.x) + 15;
	frame->top_right.x = SCREEN_WIDTH - ((SCREEN_HEIGHT /
		z2) * frame->left.r_pt.x) + 15;
	frame->bottom_left = frame->top_left;
	frame->bottom_right = frame->top_right;
	frame->top_left.y = plr->pitch - SCREEN_HEIGHT / z1 * 20;
	frame->top_right.y = plr->pitch - SCREEN_HEIGHT / z2 * 20;
	frame->bottom_left.y = plr->pitch + SCREEN_HEIGHT / z1 * 20;
	frame->bottom_right.y = plr->pitch + SCREEN_HEIGHT / z2 * 20;
	frame->visible_wall_dist = get_distance(frame->left.l_pt, frame->left.r_pt);
	frame->full_wall_dist = get_distance(frame->left.wall->x0, frame->left.wall->next->x0);
	frame->screen_wall_len = frame->top_right.x - frame->top_left.x;
	frame->full_wall_len = (frame->screen_wall_len * frame->full_wall_dist) / frame->visible_wall_dist;
	frame->ratio = frame->visible_wall_dist / frame->full_wall_dist;
	frame->tex_mult = frame->full_wall_dist / tex->w;
	if (frame->left.wall == frame->right.wall)
	{
		// frame->wall_fract_len = get_distance(frame->left.wall->x0, frame->left.l_pt) / frame->full_wall_dist;
		frame->wall_fract_len = frame->full_wall_len 
			* get_distance(frame->left.wall->x0, frame->left.l_pt)
			/ frame->full_wall_dist;
	}
}
