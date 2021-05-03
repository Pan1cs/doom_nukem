/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_segment.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jnivala <jnivala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/04 13:50:43 by jnivala           #+#    #+#             */
/*   Updated: 2021/05/03 11:40:07 by jnivala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/doom_nukem.h"

static void	draw_vertical_floor_strip(t_xyz offset, int height,
							Uint32 colour, t_frame *frame)
{
	int		cur_y;
	float	scale;
	float	step_z;

	if (offset.x < 0 || offset.x > SCREEN_WIDTH)
		return ;
	cur_y = -1;
	scale = (SCREEN_HEIGHT - offset.y) / (SCREEN_HEIGHT - frame->pitch);
	step_z = (1 - scale) / offset.y;
	scale = 0.3f;
	while (cur_y < height)
	{
		if (cur_y + offset.y >= 0 && cur_y + offset.y < SCREEN_HEIGHT)
			put_pixel(frame->buffer, offset.x,
				cur_y + offset.y,
				colour_scale(colour, scale));
		cur_y++;
		scale += step_z;
	}
}

static void	fit_to_screen_space(t_xy *offset, t_xyz *texel,
	int *height, float *uv_step_y)
{
	if (offset->y < 0)
	{
		texel->y += *uv_step_y * -offset->y;
		*height += offset->y;
		offset->y = 0;
	}
	if (*height > SCREEN_HEIGHT)
		*height = SCREEN_HEIGHT;
}

static void	draw_vertical_wall_strip(t_xy offset, int height,
							t_texture *tex, t_frame *frame)
{
	int		cur_y;
	t_xyz	corr_texel;
	t_xyz	texel;
	Uint32	colour;

	cur_y = 0;
	texel = frame->middle_uv.top_left;
	corr_texel = texel;
	fit_to_screen_space(&offset, &texel, &height, &frame->uv_step.y);
	while (cur_y < height)
	{
		if (cur_y % 16)
			corr_texel = inv_z(texel);
		if (cur_y + offset.y >= 0 && cur_y + offset.y < SCREEN_HEIGHT)
		{
			colour = colour_scale(get_texel(corr_texel.x * (tex->w - 1),
						corr_texel.y * (tex->h - 1), tex),
					frame->left.wall->wall_facing);
			put_pixel(frame->buffer, offset.x, cur_y + offset.y, colour);
		}
		cur_y++;
		texel.y += frame->uv_step.y;
	}
}

void	draw_vertically(t_frame *frame, t_sector *sector, t_texture *wall_tex)
{
	Uint32		tex_floor;

	sector = sector;
	while (frame->outer_box.top_left.x + 1 < 0)
		step_one(frame);
	tex_floor = get_floor(sector->tex_floor);
	while (frame->outer_box.top_left.x < frame->outer_box.top_right.x)
	{
		if (frame->draw_top)
			draw_vertical_wall_strip(
				vec3_to_vec2(frame->outer_box.top_left), (frame->inner_box.top_left.y - frame->outer_box.top_left.y),
				wall_tex, frame);
		if (frame->draw_middle)
			draw_vertical_wall_strip(
				vec3_to_vec2(frame->outer_box.top_left), (frame->outer_box.bottom_left.y - frame->outer_box.top_left.y),
				wall_tex, frame);
		if (frame->draw_bottom)
			draw_vertical_wall_strip(
				vec3_to_vec2(frame->inner_box.bottom_left), (frame->outer_box.bottom_left.y - frame->inner_box.bottom_left.y),
				wall_tex, frame);
		draw_vertical_floor_strip(
			vec3(frame->outer_box.top_left.x, frame->outer_box.bottom_left.y, frame->outer_box.bottom_left.z),
			((SCREEN_HEIGHT - frame->outer_box.bottom_left.y) < 0 ? 0 : SCREEN_HEIGHT - frame->outer_box.bottom_left.y),
			tex_floor, frame);
		step_one(frame);
	}
}

void	draw_segment(t_frame *frame, t_home *home, t_player *plr)
{
	t_texture	*wall_tex;
	Uint32		colour;

	wall_tex = get_tex(-1, home->editor_tex);
	if (frame->left.wall->idx < 0)
		wall_tex = get_tex(frame->left.wall->idx, home->editor_tex);
	calc_dimensions(frame, plr, home);
	calc_wall_texels(frame, wall_tex->w);
	if (plr->input.wireframe == 0)
		draw_vertically(frame, home->sectors[frame->idx], wall_tex);
	else
	{
		colour = get_floor(home->sectors[frame->idx]->tex_floor);
		if (frame->draw_top)
		{
			draw_line(vec3_to_vec2(frame->inner_box.top_left),
				vec3_to_vec2(frame->inner_box.top_right), limegreen, frame->buffer);
			draw_line(vec3_to_vec2(frame->outer_box.top_left),
				vec3_to_vec2(frame->outer_box.top_right), colour, frame->buffer);
			// draw_line(vec3_to_vec2(frame->outer_box.top_left),
			// 	vec3_to_vec2(frame->outer_box.bottom_left), limegreen, frame->buffer);
			// draw_line(vec3_to_vec2(frame->outer_box.top_right),
			// 	vec3_to_vec2(frame->outer_box.bottom_right), limegreen, frame->buffer);
		}
		if (frame->draw_middle)
		{
			draw_line(vec3_to_vec2(frame->outer_box.top_left),
				vec3_to_vec2(frame->outer_box.top_right), colour, frame->buffer);
			draw_line(vec3_to_vec2(frame->outer_box.bottom_left),
				vec3_to_vec2(frame->outer_box.bottom_right), colour, frame->buffer);
			draw_line(vec3_to_vec2(frame->outer_box.top_left),
				vec3_to_vec2(frame->outer_box.bottom_left), colour, frame->buffer);
			draw_line(vec3_to_vec2(frame->outer_box.top_right),
				vec3_to_vec2(frame->outer_box.bottom_right), colour, frame->buffer);
		}
	}
}
