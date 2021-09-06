/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   turn_on_lights.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jnivala <jnivala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/06 14:53:58 by jnivala           #+#    #+#             */
/*   Updated: 2021/08/09 12:34:57 by jnivala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/doom_nukem.h"

void	turn_on_lights(t_entity *entity, t_home *home)
{
	if (entity->is_linked > 1)
	{
		if (home->sectors[entity->is_linked - 2] && entity->state)
			home->sectors[entity->is_linked - 2]->lights.state = 1;
		else
			home->sectors[entity->is_linked - 2]->lights.state = 0;
	}
}