/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_activation.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jnivala <jnivala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/05 15:38:44 by jnivala           #+#    #+#             */
/*   Updated: 2021/08/05 15:50:14 by jnivala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/doom_nukem.h"

static void		change_texture(t_entity *entity)
{
	if (entity->state)
	{
		entity->top.idx = entity->sprite_index;
		entity->bot.idx = entity->sprite_index;
	}
	else
	{
		entity->top.idx = entity->alt_sprite_index;
		entity->bot.idx = entity->alt_sprite_index;
	}
}

Uint32			handle_activation(t_entity *entity)
{
	if (entity->entity_type == button)
	{
		if (entity->state)
			entity->state = 0;
		else
			entity->state = 1;
	}
	if (entity->entity_type == powerstation)
	{
		if (entity->state)
			entity->state = 0;
	}
	change_texture(entity);
	return (1);
}
