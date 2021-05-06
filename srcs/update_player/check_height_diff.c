/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_height_diff.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jnivala <jnivala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/03 14:01:12 by jnivala           #+#    #+#             */
/*   Updated: 2021/05/06 16:17:27 by jnivala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/doom_nukem.h"

int	check_height_diff(t_xy *dir, float *z, t_point *to)
{
	t_intersection	sect;
	t_ray			plr;
	t_height		height;

	plr.pos = vec2(0, 0);
	plr.dir = *dir;
	calc_intersection(to, &plr, &sect);
	plr.cutpoint = line_intersection(&sect);
	interpolate_y(&height, plr.cutpoint, to, to->next);
	if (height.ground - *z < 6)
	{
		*z = height.ground - *z;
		return (0);
	}
	else
		return (1);
}
