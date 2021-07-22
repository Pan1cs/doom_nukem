/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   advance_story.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jnivala <jnivala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/09 14:59:56 by jnivala           #+#    #+#             */
/*   Updated: 2021/07/19 19:20:09 by jnivala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/doom_nukem.h"

void	free_story(char ***array, Uint32 nb_of_strings)
{
	Uint32 i;

	i = nb_of_strings;
	while (i--)
	{
		free(*(*array + i));
		*(*array + i) = NULL;
	}
	free(*array);
	*array = NULL;
}

Uint32	evolve_story(t_player *plr, t_sector *sector, t_sector *msg_sector)
{
	if (plr->message_time > plr->time)
		return (FALSE);
	else
	{
		if (plr->plot_state == sector_plot)
		{
			if (msg_sector->cur_msg < msg_sector->nb_of_msgs)
				msg_sector->cur_msg++;
			if (plr->cur_sector != plr->msg_sector)
				msg_sector->cur_msg = msg_sector->nb_of_msgs;
			plr->plot_state = no_plot;
		}
		if (plr->plot_state == no_plot
			&& sector->cur_msg < sector->nb_of_msgs)
		{
			plr->plot_state = sector_plot;
			plr->msg_sector = plr->cur_sector;
			plr->message_time = plr->time + 10000;
		}
		return (TRUE);
		/*
		**	Here we can also implement player action that affects story items that are played
		**	based on world interaction.
		*/
	}
}

void	write_message(Uint32 *buffer, t_player *plr, char *msg, t_plx_modifier *mod)
{
	float			percentage;

	percentage = 1.0f - ((plr->message_time - plr->time - 5000) * 0.0002f);
	mod->len = (size_t)(ft_strlen(msg) * percentage);
	ft_str_pxl(buffer, (t_xy){100, SCREEN_HEIGHT - 30},
		msg, *mod);
}

void	draw_plot_state(t_home *home, Uint32 *buffer, t_player *plr)
{
	Uint32			cur_story;
	t_plx_modifier	mod;

	mod.colour = get_color(white);
	mod.size = TEXT_SIZE;
	cur_story = home->sectors[plr->msg_sector]->cur_msg;
	if (plr->plot_state == sector_plot && plr->message_time > plr->time)
		write_message(buffer, plr, home->sectors[plr->msg_sector]->story[cur_story], &mod);
	if (plr->plot_state != sector_plot && plr->message_time > (plr->time + 3000))
		write_message(buffer, plr, home->story[plr->plot_state], &mod);
	else if (plr->plot_state != sector_plot && plr->message_time > plr->time)
	{
		mod.len = (size_t)(ft_strlen(home->story[plr->plot_state]));
		ft_str_pxl(buffer, (t_xy){100, SCREEN_HEIGHT - 30},
			home->story[plr->plot_state], mod);
	}
}

void	end_level(t_home *home, t_player *plr)
{
	if (plr->cur_sector == home->end_sector)
	{
		if (home->linked_map)
			home->game_state = GAME_CONTINUE;
		else
			home->game_state = MAIN_MENU;
	}
}
