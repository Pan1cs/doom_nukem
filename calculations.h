/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doom_nukem.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jnivala <jnivala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/05 18:48:09 by tmaarela          #+#    #+#             */
/*   Updated: 2021/01/21 09:40:09 by jnivala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	CALCULATIONS_H
# define CALCULATIONS_H

typedef struct		s_time
{
	clock_t			beginfps;
	clock_t			endfps;
	clock_t			delta_time;
	size_t			frames;
	long int		fps;
}					t_time;

typedef	struct		s_intersection
{
	float	x1;
	float	x2;
	float 	y1;
	float	y2;
	float	den;
	float	neg;
	float	pos;
}					t_intersection;

void		calc_intersection(t_polygon *pgon, t_polygon *first, t_player *plr, 
					t_intersection *sect);
t_xy		line_intersection(t_intersection *sect);

#endif