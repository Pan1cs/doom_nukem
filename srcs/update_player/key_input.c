/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_input.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rzukale <rzukale@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/18 16:07:42 by tmaarela          #+#    #+#             */
/*   Updated: 2021/05/12 16:13:58 by rzukale          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/doom_nukem.h"

static void	action_keys(t_player *plr, SDL_KeyCode *sym)
{
	if (*sym == SDLK_z && plr->input.wireframe == 1)
		plr->input.wireframe = 0;
	else if (*sym == SDLK_z && plr->input.wireframe == 0)
		plr->input.wireframe = 1;
	if (*sym == SDLK_x && plr->input.minimap == 1)
		plr->input.minimap = 0;
	else if (*sym == SDLK_x && plr->input.minimap == 0)
		plr->input.minimap = 1;
	if (*sym == SDLK_c && plr->input.info == 1)
		plr->input.info = 0;
	else if (*sym == SDLK_c && plr->input.info == 0)
		plr->input.info = 1;
	if (*sym == SDLK_m && plr->input.mouse == 0)
	{
		SDL_SetRelativeMouseMode(SDL_TRUE);
		plr->input.mouse = 1;
	}
	else if (*sym == SDLK_m && plr->input.mouse == 1)
	{
		SDL_SetRelativeMouseMode(SDL_FALSE);
		plr->input.mouse = 0;
	}
}

void	keys_down(t_player *plr, SDL_KeyCode sym, int *game_state)
{
	if (sym == SDLK_ESCAPE)
		*game_state = MAIN_MENU;
	if (sym == SDLK_s)
		plr->input.down = 1;
	if (sym == SDLK_w)
		plr->input.up = 1;
	if (sym == SDLK_d)
		plr->input.right = 1;
	if (sym == SDLK_a)
		plr->input.left = 1;
	if (sym == SDLK_q)
		plr->input.rot_left = 1;
	if (sym == SDLK_e)
		plr->input.rot_right = 1;
	if (sym == SDLK_LCTRL)
		plr->height = 3;
	if (sym == SDLK_SPACE && plr->acceleration == 0)
		plr->acceleration = 5;
	action_keys(plr, &sym);
}

void	keys_up(t_player *plr, SDL_KeyCode sym)
{
	if (sym == SDLK_s)
		plr->input.down = 0;
	if (sym == SDLK_w)
		plr->input.up = 0;
	if (sym == SDLK_d)
		plr->input.right = 0;
	if (sym == SDLK_a)
		plr->input.left = 0;
	if (sym == SDLK_q)
		plr->input.rot_left = 0;
	if (sym == SDLK_e)
		plr->input.rot_right = 0;
	if (sym == SDLK_LCTRL)
		plr->height = 6;
}

void	key_input(t_player *plr, SDL_Event *e, int *game_state)
{
	if (e->type == SDL_KEYDOWN)
		keys_down(plr, e->key.keysym.sym, game_state);
	else if (e->type == SDL_KEYUP)
		keys_up(plr, e->key.keysym.sym);
	else
		return ;
}
