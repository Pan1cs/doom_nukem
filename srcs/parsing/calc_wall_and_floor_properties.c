/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calc_wall_and_floor_properties.c                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jnivala <jnivala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/29 11:13:37 by jnivala           #+#    #+#             */
/*   Updated: 2021/07/01 13:55:52 by jnivala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/doom_nukem.h"

void	calc_wall_and_floor_properties(t_home *home)
{
	unsigned int	i;

	i = 0;
	while (i < home->nbr_of_sectors)
	{
		calc_sector_bounds(home->sectors[i]);
		home->sectors[i]->nb_of_ground
			= triangulate_floor(home->sectors[i], 'g');
		home->sectors[i]->nb_of_ceil
			= triangulate_ceiling(home->sectors[i], 'c');
		i++;
	}
	calc_extra_walls(home);
	calc_average_unit_normals(home);
	calc_light_sources(home);
}
