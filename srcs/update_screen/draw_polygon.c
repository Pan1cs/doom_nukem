/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_polygon.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jnivala <jnivala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/23 11:35:04 by jnivala           #+#    #+#             */
/*   Updated: 2021/05/26 11:33:14 by jnivala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/doom_nukem.h"

int	draw_polygon(t_frame *frame, t_triangle *tri)
{
	draw_line(vec2(tri->p[0].x, tri->p[0].y), vec2(tri->p[1].x, tri->p[1].y),
		white, frame->buffer);
	draw_line(vec2(tri->p[1].x, tri->p[1].y), vec2(tri->p[2].x, tri->p[2].y),
		white, frame->buffer);
	draw_line(vec2(tri->p[2].x, tri->p[2].y), vec2(tri->p[0].x, tri->p[0].y),
		white, frame->buffer);
	return (TRUE);
}

static void	swap_xyz(t_xyz *p0, t_xyz *p1)
{
	t_xyz	swap;

	swap.x = p0->x;
	swap.y = p0->y;
	swap.z = p0->z;
	p0->x = p1->x;
	p0->y = p1->y;
	p0->z = p1->z;
	p1->x = swap.x;
	p1->y = swap.y;
	p1->z = swap.z;
}

static void	swap_uvz(t_uvz *p0, t_uvz *p1)
{
	t_uvz	swap;

	swap.u = p0->u;
	swap.v = p0->v;
	swap.z = p0->z;
	p0->u = p1->u;
	p0->v = p1->v;
	p0->z = p1->z;
	p1->u = swap.u;
	p1->v = swap.v;
	p1->z = swap.z;
}

static void	sort_vertices(t_triangle *tri)
{
	if (tri->p[0].y > tri->p[1].y)
	{
		swap_xyz(&tri->p[0], &tri->p[1]);
		swap_uvz(&tri->uv[0], &tri->uv[1]);
	}
	if (tri->p[0].y > tri->p[2].y)
	{
		swap_xyz(&tri->p[0], &tri->p[2]);
		swap_uvz(&tri->uv[0], &tri->uv[2]);
	}
	if (tri->p[1].y > tri->p[2].y)
	{
		swap_xyz(&tri->p[1], &tri->p[2]);
		swap_uvz(&tri->uv[1], &tri->uv[2]);
	}
}

static t_uvz	calculate_texel_delta_x(t_triangle *calculate, float denom)
{
	t_uvz texel_delta;

	texel_delta.z = (
		((calculate->uv[2].z - calculate->uv[0].z)
		* (calculate->p[1].y - calculate->p[0].y))
		- ((calculate->uv[1].z - calculate->uv[0].z)
		* (calculate->p[2].y - calculate->p[0].y))) * denom;
	texel_delta.u = (
		((calculate->uv[2].u - calculate->uv[0].u)
		* (calculate->p[1].y - calculate->p[0].y))
		- ((calculate->uv[1].u - calculate->uv[0].u)
		* (calculate->p[2].y - calculate->p[0].y))) * denom;
	texel_delta.v = (
		((calculate->uv[2].v - calculate->uv[0].v)
		* (calculate->p[1].y - calculate->p[0].y))
		- ((calculate->uv[1].v - calculate->uv[0].v)
		* (calculate->p[2].y - calculate->p[0].y))) * denom;
	return (texel_delta);
}

static t_uvz	calculate_texel_delta_y(t_triangle *calculate, float denom)
{
	t_uvz texel_delta;

	texel_delta.z = (
		((calculate->uv[1].z - calculate->uv[0].z)
		* (calculate->p[2].x - calculate->p[0].x))
		- ((calculate->uv[2].z - calculate->uv[0].z)
		* (calculate->p[1].x - calculate->p[0].x))) * denom;
	texel_delta.u = (
		((calculate->uv[1].u - calculate->uv[0].u)
		* (calculate->p[2].x - calculate->p[0].x))
		- ((calculate->uv[2].u - calculate->uv[0].u)
		* (calculate->p[1].x - calculate->p[0].x))) * denom;
	texel_delta.v = (
		((calculate->uv[1].v - calculate->uv[0].v)
		* (calculate->p[2].x - calculate->p[0].x))
		- ((calculate->uv[2].v - calculate->uv[0].v)
		* (calculate->p[1].x - calculate->p[0].x))) * denom;
	return (texel_delta);
}

static void fill_triangle(Uint32 *buffer, t_xy left, t_xy right,
	t_uvz texel_delta_x, t_uvz texel_delta_left, t_uvz uv,
	float pixel_delta_left, float pixel_delta_right, t_texture *tex)
{
	int		x1;
	int		x2;
	t_uvz	texel;
	t_uvz	corr_texel;
	Uint32	colour;
	int		left_y;
	int		right_y;

	left_y = left.y;
	right_y = right.y;
	while (left_y < right_y)
	{
		x1 = left.x;
		x2 = right.x;
		texel.z = uv.z + (1 - (left.x - x1)) * texel_delta_x.z;
		texel.u = uv.u + (1 - (left.x - x1)) * texel_delta_x.u;
		texel.v = uv.v + (1 - (left.x - x1)) * texel_delta_x.v;
		while (x1++ < x2)
		{
			corr_texel = texel_inv_z(texel);
			colour = get_texel(corr_texel.u * (tex->w - 1),
						corr_texel.v * (tex->h - 1), tex);
			put_pixel(buffer, x1, left_y, colour);
			texel.z += texel_delta_x.z;
			texel.u += texel_delta_x.u;
			texel.v += texel_delta_x.v;
		}
		left.x += pixel_delta_left;
		right.x += pixel_delta_right;
		uv.z += texel_delta_left.z;
		uv.u += texel_delta_left.u;
		uv.v += texel_delta_left.v;
		left_y++;
	}
}

void	calculate_triangle(t_frame *frame, t_triangle *tri, t_texture *tex)
{
	float	dxdy1;
	float	dxdy2;
	float	dxdy3;
	float	denom;
	float	dy;
	int		y1i;
	int		y2i;
	int		y3i;
	int		side;
	t_uvz	texel_delta_x;
	t_uvz	texel_delta_y;
	t_uvz	texel_delta_left;
	t_uvz	texel_left;
	float	pixel_delta_left;
	float	pixel_delta_right;
	float	x_left;
	float	x_right;
	t_triangle	calculate;

	calculate = *tri;
	calculate.p[0].x = tri->p[0].x + 0.5;
	calculate.p[0].y = tri->p[0].y + 0.5;
	calculate.p[1].x = tri->p[1].x + 0.5;
	calculate.p[1].y = tri->p[1].y + 0.5;
	calculate.p[2].x = tri->p[2].x + 0.5;
	calculate.p[2].y = tri->p[2].y + 0.5;
	calculate.uv[0] = texel_inv_z(tri->uv[0]);
	calculate.uv[1] = texel_inv_z(tri->uv[1]);
	calculate.uv[2] = texel_inv_z(tri->uv[2]);
	sort_vertices(&calculate);
	y1i = calculate.p[0].y;
	y2i = calculate.p[1].y;
	y3i = calculate.p[2].y;
	if ((y1i == y2i && y1i == y3i)
		|| ((int) calculate.p[0].x == (int) calculate.p[1].x
		&& (int) calculate.p[0].x == (int) calculate.p[2].x))
		return;
	denom = ((calculate.p[2].x - calculate.p[0].x) * (calculate.p[1].y - calculate.p[0].y)
			- (calculate.p[1].x - calculate.p[0].x) * (calculate.p[2].y - calculate.p[0].y));
	if (!denom)
		return;
	denom = 1 / denom;
	texel_delta_x = calculate_texel_delta_x(&calculate, denom);
	texel_delta_y = calculate_texel_delta_y(&calculate, denom);
	if (calculate.p[1].y > calculate.p[0].y)
		dxdy1 = interpolate_points(calculate.p[0].x, calculate.p[1].x, calculate.p[0].y, calculate.p[1].y);
	if (calculate.p[2].y > calculate.p[0].y)
		dxdy2 = interpolate_points(calculate.p[0].x, calculate.p[2].x, calculate.p[0].y, calculate.p[2].y);
	if (calculate.p[2].y > calculate.p[1].y)
		dxdy3 = interpolate_points(calculate.p[1].x, calculate.p[2].x, calculate.p[1].y, calculate.p[2].y);
	side = dxdy2 > dxdy1;
	if (calculate.p[0].y == calculate.p[1].y)
		side = calculate.p[0].x > calculate.p[1].x;
	if (calculate.p[1].y == calculate.p[2].y)
		side = calculate.p[2].x > calculate.p[1].x;
	if (!side)
	{
		pixel_delta_left = dxdy2;
		texel_delta_left = uvz_calculate_value_with_delta(pixel_delta_left, texel_delta_x, texel_delta_y);
		dy = 1 - (calculate.p[0].y - y1i);
		x_left = calculate.p[0].x + dy * pixel_delta_left;
		texel_left.z = calculate.uv[0].z + dy * texel_delta_left.z;
		texel_left.u = calculate.uv[0].u + dy * texel_delta_left.u;
		texel_left.v = calculate.uv[0].v + dy * texel_delta_left.v;
		if (y1i < y2i)
		{
			x_right = calculate.p[0].x + dy * dxdy1;
			pixel_delta_right = dxdy1;
			fill_triangle(frame->buffer,
				(t_xy){x_left, y1i}, (t_xy){x_right, y2i},
				texel_delta_x, texel_delta_left, texel_left,
				pixel_delta_left, pixel_delta_right, tex);
		}
		if (y2i < y3i)
		{
			x_right = calculate.p[1].x + (1 - (calculate.p[1].y - y2i)) * dxdy3;
			pixel_delta_right = dxdy3;
			fill_triangle(frame->buffer,
				(t_xy){x_left, y2i}, (t_xy){x_right, y3i},
				texel_delta_x, texel_delta_left, texel_left,
				pixel_delta_left, pixel_delta_right, tex);
		}
	}
	else
	{
		pixel_delta_right = dxdy2;
		dy = 1 - (calculate.p[0].y - y1i);
		x_right = calculate.p[0].x + dy * pixel_delta_right;
		if (y1i < y2i)
		{
			pixel_delta_left = dxdy1;
			texel_delta_left = uvz_calculate_value_with_delta(pixel_delta_left, texel_delta_x, texel_delta_y);
			x_left = calculate.p[0].x + dy * pixel_delta_left;
			texel_left.z = calculate.uv[0].z + dy * texel_delta_left.z;
			texel_left.u = calculate.uv[0].u + dy * texel_delta_left.u;
			texel_left.v = calculate.uv[0].v + dy * texel_delta_left.v;
			fill_triangle(frame->buffer,
				(t_xy){x_left, y1i}, (t_xy){x_right, y2i},
				texel_delta_x, texel_delta_left, texel_left,
				pixel_delta_left, pixel_delta_right, tex);
		}
		if (y2i < y3i)
		{
			pixel_delta_left = dxdy3;
			texel_delta_left = uvz_calculate_value_with_delta(pixel_delta_left, texel_delta_x, texel_delta_y);
			dy = 1 - (calculate.p[1].y - y2i);
			x_left = calculate.p[1].x + dy * pixel_delta_left;
			texel_left.z = calculate.uv[1].z + dy * texel_delta_left.z;
			texel_left.u = calculate.uv[1].u + dy * texel_delta_left.u;
			texel_left.v = calculate.uv[1].v + dy * texel_delta_left.v;
			fill_triangle(frame->buffer,
				(t_xy){x_left, y2i}, (t_xy){x_right, y3i},
				texel_delta_x, texel_delta_left, texel_left,
				pixel_delta_left, pixel_delta_right, tex);
		}
	}
}

int	painters_algorithm(const void *tri1, const void *tri2)
{
	if (triangle_calculate_average_z((t_triangle *)tri1)
		> triangle_calculate_average_z((t_triangle *)tri2))
		return (1);
	else
		return (0);
}

int	draw_cube(t_frame *frame, t_home *home, t_player *plr)
{
	int				i;
	int				r;
	t_xyz			scale;
	t_xyz			normal;
	t_xyz			view_offset;
	t_m4x4			matrix;
	static float	degree = 0.0f;
	// t_xyz			light_direction;
	// static Uint32	cur_time;
	// t_texture		*tex;

	// (void)home;
	// tex = get_tex(-1, home->editor_tex);
	// if (frame->last_frame - cur_time > 66)
	// {
	// 	cur_time = frame->last_frame;
	// 	degree += 0.1f;
	// }
	// if (degree > TWO_PI)
	// 	degree = 0.0f;
	// light_direction = (t_xyz){0,0,-1};
	//SOUTH
	home->cube[0] = (t_triangle){
		{(t_xyz){0.0f,0.0f,0.0f,1.0f}, (t_xyz){0.0f,1.0f,0.0f,1.0f}, (t_xyz){1.0f,1.0f,0.0f,1.0f}},
		{(t_uvz){0.0f,1.0f,0.0f}, (t_uvz){0.0f,0.0f,0.0f}, (t_uvz){1.0f,0.0f,0.0f}}};
	home->cube[1] = (t_triangle){
		(t_xyz){0.0f,0.0f,0.0f,1.0f}, (t_xyz){1.0f,1.0f,0.0f,1.0f}, (t_xyz){1.0f,0.0f,0.0f,1.0f},
		{(t_uvz){0.0f,1.0f,0.0f}, (t_uvz){1.0f,0.0f,0.0f}, (t_uvz){1.0f,1.0f,0.0f}}};
	//EAST
	home->cube[2] = (t_triangle){
		{(t_xyz){1.0f,0.0f,0.0f,1.0f}, (t_xyz){1.0f,1.0f,0.0f,1.0f}, (t_xyz){1.0f,1.0f,1.0f,1.0f}},
		{(t_uvz){0.0f,1.0f,0.0f}, (t_uvz){0.0f,0.0f,0.0f}, (t_uvz){1.0f,0.0f,1.0f}}};
	home->cube[3] = (t_triangle){
		{(t_xyz){1.0f,0.0f,0.0f,1.0f}, (t_xyz){1.0f,1.0f,1.0f,1.0f}, (t_xyz){1.0f,0.0f,1.0f,1.0f}},
		{(t_uvz){0.0f,1.0f,0.0f}, (t_uvz){1.0f,0.0f,1.0f}, (t_uvz){1.0f,1.0f,1.0f}}};
	//NORTH
	home->cube[4] = (t_triangle){
		{(t_xyz){1.0f,0.0f,1.0f,1.0f}, (t_xyz){1.0f,1.0f,1.0f,1.0f}, (t_xyz){0.0f,1.0f,1.0f,1.0f}},
		{(t_uvz){0.0f,1.0f,1.0f}, (t_uvz){0.0f,0.0f,1.0f}, (t_uvz){1.0f,0.0f,1.0f}}};
	home->cube[5] = (t_triangle){
		{(t_xyz){1.0f,0.0f,1.0f,1.0f}, (t_xyz){0.0f,1.0f,1.0f,1.0f}, (t_xyz){0.0f,0.0f,1.0f,1.0f}},
		{(t_uvz){0.0f,1.0f,1.0f}, (t_uvz){1.0f,0.0f,1.0f}, (t_uvz){1.0f,1.0f,1.0f}}};
	//WEST
	home->cube[6] = (t_triangle){
		{(t_xyz){0.0f,0.0f,1.0f,1.0f}, (t_xyz){0.0f,1.0f,1.0f,1.0f}, (t_xyz){0.0f,1.0f,0.0f,1.0f}},
		{(t_uvz){0.0f,1.0f,1.0f}, (t_uvz){0.0f,0.0f,1.0f}, (t_uvz){1.0f,0.0f,0.0f}}};
	home->cube[7] = (t_triangle){
		{(t_xyz){0.0f,0.0f,1.0f,1.0f}, (t_xyz){0.0f,1.0f,0.0f,1.0f}, (t_xyz){0.0f,0.0f,0.0f,1.0f}},
		{(t_uvz){0.0f,1.0f,1.0f}, (t_uvz){1.0f,0.0f,0.0f}, (t_uvz){1.0f,1.0f,0.0f}}};
	//TOP
	home->cube[8] = (t_triangle){
		{(t_xyz){0.0f,1.0f,0.0f,1.0f}, (t_xyz){0.0f,1.0f,1.0f,1.0f}, (t_xyz){1.0f,1.0f,1.0f,1.0f}},
		{(t_uvz){0.0f,1.0f,0.0f}, (t_uvz){0.0f,0.0f,1.0f}, (t_uvz){1.0f,0.0f,1.0f}}};
	home->cube[9] = (t_triangle){
		{(t_xyz){0.0f,1.0f,0.0f,1.0f}, (t_xyz){1.0f,1.0f,1.0f,1.0f}, (t_xyz){1.0f,1.0f,1.0f,1.0f}},
		{(t_uvz){0.0f,1.0f,0.0f}, (t_uvz){1.0f,0.0f,1.0f}, (t_uvz){1.0f,1.0f,1.0f}}};
	//BOTTOM
	home->cube[10] = (t_triangle){
		{(t_xyz){1.0f,0.0f,1.0f,1.0f}, (t_xyz){0.0f,0.0f,1.0f,1.0f}, (t_xyz){0.0f,0.0f,0.0f,1.0f}},
		{(t_uvz){0.0f,1.0f,1.0f}, (t_uvz){0.0f,0.0f,1.0f}, (t_uvz){1.0f,0.0f,0.0f}}};
	home->cube[11] = (t_triangle){
		{(t_xyz){1.0f,0.0f,1.0f,1.0f}, (t_xyz){0.0f,0.0f,0.0f,1.0f}, (t_xyz){1.0f,0.0f,0.0f,1.0f}},
		{(t_uvz){0.0f,1.0f,1.0f}, (t_uvz){1.0f,0.0f,0.0f}, (t_uvz){1.0f,1.0f,0.0f}}};
	view_offset = (t_xyz){1.0f, 1.0f, 0.0f, 1.0f};
	i = 0;
	while (i < 12)
	{
		home->transformed_cube[i] = apply_world_matrix(degree, degree, (t_xyz){0.0f, 0.0f, 5.0f, 0.0f}, &home->cube[i]);
		i++;
	}
	i = 0;
	plr->up = (t_xyz){0.0f,1.0f,0.0f,1.0f}; //Position player straight.
	plr->target = (t_xyz){0.0f, 0.0f, 1.0f, 0.0f}; //Position player's gaze forwards.
	plr->look_dir = multi_vec_matrix(&plr->target, &matrix);
	plr->target = vec3_add(plr->camera, plr->look_dir);
	while (i < 12)
	{
		home->view_cube[i] = apply_camera(plr->camera, plr->target, plr->up, &home->transformed_cube[i]);
		i++;
	}
	i = 0;
	r = 0;
	scale = (t_xyz){0.5 * SCREEN_WIDTH, 0.5 * SCREEN_HEIGHT, 1.0f, 0.0f};
	while (i < 12)
	{
		normal = triangle_normal(&home->view_cube[i]);
		if (vec3_dot_product(normal, vec3_dec(home->view_cube[i].p[0], plr->camera)) < 0)
		{
			home->project_cube[i] = create_projection(&home->view_cube[i]);
			home->project_cube[i].p[0] = vec3_div(home->project_cube[i].p[0], home->project_cube[i].p[0].w);
			home->project_cube[i].p[1] = vec3_div(home->project_cube[i].p[1], home->project_cube[i].p[1].w);
			home->project_cube[i].p[2] = vec3_div(home->project_cube[i].p[2], home->project_cube[i].p[2].w);
			home->project_cube[i].p[0].x *= -1.0f;
			home->project_cube[i].p[0].y *= -1.0f;
			home->project_cube[i].p[1].x *= -1.0f;
			home->project_cube[i].p[1].y *= -1.0f;
			home->project_cube[i].p[2].x *= -1.0f;
			home->project_cube[i].p[2].y *= -1.0f;
			home->project_cube[i].p[0] = vec3_add(home->project_cube[i].p[0], view_offset);
			home->project_cube[i].p[1] = vec3_add(home->project_cube[i].p[1], view_offset);
			home->project_cube[i].p[2] = vec3_add(home->project_cube[i].p[2], view_offset);
			home->project_cube[i] = scale_triangle(&home->project_cube[i], scale);
			home->triangles_to_raster[r] = home->project_cube[i];
			r++;
		}
		i++;
	}
	i = r;
	r = 0;
	qsort((void *)home->triangles_to_raster, i, sizeof(home->triangles_to_raster[0]), painters_algorithm);
	while (r < i)
	{
		// calculate_triangle(frame, &home->triangles_to_raster[r], tex);
		draw_polygon(frame, &home->triangles_to_raster[r]);
		r++;
	}
	str_pxl(frame->buffer, (t_xy){5.0f, 10.0f}, "player_y", (t_plx_modifier){green, 2});
	str_pxl(frame->buffer, (t_xy){5.0f, 24.0f}, ft_ftoa(plr->camera.y, 6), (t_plx_modifier){green, 2});
	str_pxl(frame->buffer, (t_xy){5.0f, 38.0f}, ft_ftoa(plr->camera.x, 6), (t_plx_modifier){green, 2});
	return (TRUE);
}
