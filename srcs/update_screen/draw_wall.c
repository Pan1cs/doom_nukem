/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_wall.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmaarela <tmaarela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/22 13:25:48 by tmaarela          #+#    #+#             */
/*   Updated: 2020/12/22 13:25:48 by tmaarela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../doom_nukem.h"

void			ft_draw_tex_col(t_tex_col *tex, double distance, SDL_Surface *surf)
{
	int		texy;
	double	screeny;
	Uint32		color;

	texy = 0;
	screeny = tex->top;
	while (screeny < tex->bot)
	{
		if (screeny <= 0 - tex->step)
		{
			screeny += tex->step;
			++texy;
			continue;
		}
		SDL_LockSurface(tex->tex);
		color = get_pixel(tex->tex, tex->tex_col, texy);
		SDL_UnlockSurface(tex->tex);
		ft_draw_line(
			vec2(tex->scr_col, ceil(screeny)),
			vec2(tex->scr_col, ceil(screeny + tex->step)), color, surf);
		screeny += tex->step;
		++texy;
		if (screeny > SCREEN_HEIGHT)
			return ;
	}
}

int				get_wall_side(t_wall wall, t_player *plr)
{
	if (wall.side == VERTICAL_WALL)
		return ((wall.hit.x < plr->pos.x) ? 0 : 1);
	else
		return ((wall.hit.y < plr->pos.y) ? 2 : 3);
}

void			draw_wall(int col, t_wall wall, t_xy ray, t_home *home, SDL_Surface *surf, t_player *plr)
{
	t_tex_col	tex;
	int			height;
	double		wall_x;

	height = SCREEN_HEIGHT / (wall.distance * vec2_dot(plr->dir, ray));
	tex.top = (SCREEN_HEIGHT * plr->height) - (height * WALL_HEIGHT);
	tex.bot = (SCREEN_HEIGHT * plr->height) + (height * WALL_HEIGHT);
	tex.scr_col = col;
	wall_x = (wall.side == HORIZONTAL_WALL) ?
		plr->pos.x + ray.x * wall.distance :
		plr->pos.y + ray.y * wall.distance;
	wall_x -= floor(wall_x);
	tex.tex_col = (int)(wall_x * TEX_SIZE);
	tex.line_height = tex.bot - tex.top;
	tex.step = tex.line_height / (double)TEX_SIZE;
	tex.tex = home->wall[get_wall_side(wall, home)];
	ft_draw_tex_col(&tex, wall.distance, surf);
}