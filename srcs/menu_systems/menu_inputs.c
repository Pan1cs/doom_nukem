/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   menu_inputs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rzukale <rzukale@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/11 13:55:49 by rzukale           #+#    #+#             */
/*   Updated: 2021/05/12 13:59:50 by rzukale          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/doom_nukem.h"

int		get_game_state(int *option)
{
	if (*option == 0)
		return (EDITOR);
	if (*option == 1)
		return (MAP_MENU);
	if (*option == 2)
		return (MAIN_MENU);
	if (*option == 3)
		return (QUIT);
	return (QUIT);
}

void	process_inputs_main_menu(int *game_state, SDL_Event *e, int *option)
{
	while (SDL_PollEvent(e) != 0)
	{
		if (e->type == SDL_KEYDOWN)
		{
			if (e->type == SDL_QUIT)
				*game_state = QUIT;
			if (e->key.keysym.sym == SDLK_ESCAPE)
				*game_state = QUIT;
			if (e->key.keysym.sym == SDLK_UP)
			{
				if (*option > 0 && *option < 4)
					*option = *option - 1;
				else if (*option == 0)
					*option = 3;
			}
			if (e->key.keysym.sym == SDLK_DOWN)
			{
				if (*option >= 0 && *option < 3)
					*option = *option + 1;
				else if (*option == 3)
					*option = 0;
			}
			if (e->key.keysym.sym == SDLK_RETURN || e->key.keysym.sym == SDLK_KP_ENTER)
				*game_state = get_game_state(option);
			
		}
	}
}

void	process_inputs_load_menu(int *game_state, SDL_Event *e, t_menu *menu)
{
	while (SDL_PollEvent(e) != 0)
	{
		if (e->type == SDL_KEYDOWN)
		{
			if (e->type == SDL_QUIT)
				*game_state = QUIT;
			if (e->key.keysym.sym == SDLK_ESCAPE)
				*game_state = MAIN_MENU;
			if (e->key.keysym.sym == SDLK_UP)
			{
				if (menu->option > 0 && menu->option < menu->nbr_of_maps)
					menu->option = menu->option - 1;
				else if (menu->option == 0)
					menu->option = (menu->nbr_of_maps - 1);
			}
			if (e->key.keysym.sym == SDLK_DOWN)
			{
				if (menu->option >= 0 && menu->option < (menu->nbr_of_maps - 1))
					menu->option = menu->option + 1;
				else if (menu->option == (menu->nbr_of_maps - 1))
					menu->option = 0;
			}
			if (e->key.keysym.sym == SDLK_RETURN || e->key.keysym.sym == SDLK_KP_ENTER)
			{
				*game_state = GAME_LOOP;
				menu->selected = TRUE;
			}
			
		}
	}
}
