/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_enemies.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jnivala <jnivala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/12 10:36:37 by jnivala           #+#    #+#             */
/*   Updated: 2021/08/18 16:30:07 by jnivala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/doom_nukem.h"

void	update_enemies(t_home *home, t_player *plr, Uint32 delta_time)
{
	Uint32		i;
	t_entity	*cur_enemy;
	static t_xyz	pos;

	i = 0;
	// (void)plr;
	(void)delta_time;
	while (i < home->nbr_of_entities)
	{
		cur_enemy = home->entity_pool[i];
		if (cur_enemy->entity_type != ENTITY_DEAD)
		{
			if (cur_enemy->entity_type == skull_skulker
				|| cur_enemy->entity_type == thing)
			{
				// entity_gravity(home->sectors[cur_enemy->sector_idx],
				// 	cur_enemy, delta_time);
				// entity_move(cur_enemy, home, delta_time);
				determine_angle_between_entity_and_plr(cur_enemy,
					plr);
				set_entity_texels_for_frame(cur_enemy);
				// printf("%f\n", vec3_eucl_dist(vec3_dec(plr->pos, cur_enemy->pos)));
				if (!check_if_same_point(pos, cur_enemy->pos))
				{
					printf("x: %f y: %f z: %f\n", cur_enemy->pos.x,
						cur_enemy->pos.y, cur_enemy->pos.z);
					pos = cur_enemy->pos;
				}
			}
		}
		i++;
	}

}
