/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_frame.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jnivala <jnivala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/14 13:27:48 by tmaarela          #+#    #+#             */
/*   Updated: 2021/09/14 11:29:04 by jnivala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/doom_nukem.h"

// static void	draw_minimap(t_home *home, t_frame *frame)
// {
// 	unsigned int	i;
// 	unsigned int	j;
// 	t_point			*temp;

// 	i = 0;
// 	while (i < home->nbr_of_sectors)
// 	{
// 		j = 0;
// 		temp = home->sectors[i]->points;
// 		while (j < home->sectors[i]->nb_of_walls)
// 		{
// 			draw_line(center_to_screen(temp->x0),
// 				center_to_screen(temp->next->x0),
// 				greenyellow, frame->buffer);
// 			temp = temp->next;
// 			j++;
// 		}
// 		i++;
// 	}
// }

// static void	draw_player(t_frame *frame)
// {
// 	draw_square(frame->buffer, center_to_screen(vec2(-2.0f, -2.0f)),
// 		yellow, 4);
// 	draw_line(center_to_screen(vec2(0.0f, 0.0f)),
// 		center_to_screen(vec2_add(vec2(0.0f, 0.0f),
// 				vec2_mul(vec2(-PLR_DIR, PLR_DIR), 400))),
// 		lightgreen, frame->buffer);
// 	draw_line(center_to_screen((vec2(0.0f, 0.0f)),
// 		center_to_screen(vec2_add(vec2(0.0f, 0.0f),
// 				vec2_mul(vec2(PLR_DIR, PLR_DIR), 400))),
// 		lightgreen, frame->buffer);
// }

void	add_skybox(t_frame *frame, t_home *home, t_player *plr,
	 t_skybox *skybox)
{
	Uint32	i;

	i = 0;
	quick_reset_queue(frame->transformed);
	while (i < 12)
	{
		if (enqueue_to_raster(frame->transformed, &skybox->face[i]))
		{
			draw_sector(frame, home, plr, frame->idx);
			quick_reset_queue(frame->transformed);
		}
		else
			i++;
	}
}

void	draw_frame(t_home *home, t_frame *frame, t_player *plr)
{
	if (plr->plot_state == start_cutscene)
		draw_cutscene(&frame->buffer, plr, home->sectors[plr->start_sector]);
	else if (plr->plot_state == end_cutscene)
		draw_cutscene(&frame->buffer, plr, home->sectors[home->end_sector]);
	else
	{
		frame->idx = plr->cur_sector;
		frame->old_idx = old_sector;
		reset_depth_buffer(frame->depth_buffer);
		add_skybox(frame, home, plr, &home->skybox);
		draw_sector(frame, home, plr, -1);
		scan_fov(home, frame, plr);
		if (plr->input.info)
			draw_info(frame, plr, (int)home->t.fps);
		draw_heads_up_display(home, frame, plr);
		draw_plot_state(home, &frame->buffer, plr);
		draw_object_data(&frame->buffer, plr);
	}
	return ;
}
