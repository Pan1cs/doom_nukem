#include "../../headers/doom_nukem.h"

void	editor_keyboard(SDL_Keycode keysym, t_action *action)
{
	if (action->input_active)
	{
		if (keysym == SDLK_RETURN)
		{
			action->input_active = 0;
			action->keysym = -1;
		}
		else
			action->keysym = keysym;
		return ;
	}
	else if (action->link_maps || action->open_file)
	{
		action->keysym = keysym;
		if (keysym == SDLK_RCTRL)
			action->prev_keysym = keysym;
		if (keysym == SDLK_BACKSPACE)
		{
			if (action->open_file)
				action->open_file = 0;
			if (action->link_maps)
				action->link_maps = 0;
			action->start = 0;
			action->option = 0;
		}
		if (keysym == SDLK_UP)
		{
			if (action->option > 0 && action->option < action->nbr_of_maps)
				action->option = action->option - 1;
			else if (action->option == 0)
				action->option = (action->nbr_of_maps - 1);
		}
		if (keysym == SDLK_DOWN)
		{
			if (action->option >= 0 && action->option < (action->nbr_of_maps - 1))
				action->option = action->option + 1;
			else if (action->option == (action->nbr_of_maps - 1))
			action->option = 0;
		}
		if (keysym == SDLK_RETURN ||keysym == SDLK_KP_ENTER)
		{
			if (action->open_file)
				action->open_file = 2;
			if (action->link_maps)
				action->link_maps = 2;
		}
	}
	else
	{
		if (keysym == SDLK_w)
			action->offsetf.y -= action->scalarf * 0.1f;
		if (keysym == SDLK_s)
			action->offsetf.y += action->scalarf * 0.1f;
		if (keysym == SDLK_d)
			action->offsetf.x += action->scalarf * 0.1f;
		if (keysym == SDLK_a)
			action->offsetf.x -= action->scalarf * 0.1f;
		if (keysym == SDLK_f)
		{
			if (action->create_sector == idle)
				action->create_sector = allocate;
		}
		if (keysym == SDLK_g && action->grid == 0)
			action->grid = 1;
		else if (keysym == SDLK_g && action->grid == 1)
			action->grid = 0;
		if (keysym == SDLK_e && action->create_entity == idle)
			action->create_entity = allocate;
		if (keysym == SDLK_l && action->selected_entity >= 0)
			action->link_entity = 1;
		if (keysym == SDLK_u && action->selected_entity >= 0)
			action->unlink_entity = 1;
		if (keysym == SDLK_r)
			action->delete = 1;
	}
}

void	editor_mouse(t_mouse_data *mouse_data, SDL_Event *e, t_action *action, t_buffer *buffer)
{
	t_screen_xy	pos;

	if (e->type == SDL_MOUSEMOTION)
	{
		mouse_data->x = e->motion.x;
		mouse_data->y = e->motion.y;
		SDL_GetMouseState(&pos.x, &pos.y);
		action->mouse_pos = get_ndc(buffer, pos);
		action->world_pos = ndc_to_world(action->mouse_pos, action->offsetf,
			action->scalarf);
	}
	if (e->type == SDL_MOUSEWHEEL && e->wheel.y > 0 && action->scalarf < ZOOM_IN_MAX)
		mouse_zoom(action, 1);
	if(e->type == SDL_MOUSEWHEEL && e->wheel.y < 0 && action->scalarf > ZOOM_OUT_MAX)
		mouse_zoom(action, 0);
	if (e->type == SDL_MOUSEBUTTONDOWN)
	{
		if (e->button.button == SDL_BUTTON_LEFT)
			mouse_data->i_mbleft = 1;
		if (e->button.button == SDL_BUTTON_RIGHT)
			mouse_data->i_mbright = 1;
		if ((e->button.button == SDL_BUTTON_LEFT && action->create_entity == allocate))
			action->create_entity = user_input;
		if (e->button.button == SDL_BUTTON_RIGHT && action->create_entity == allocate)
			action->create_entity = idle;
		if (e->button.button == SDL_BUTTON_LEFT && action->convert_to_portal == allocate)
			action->convert_to_portal = user_input;
		if (e->button.button == SDL_BUTTON_RIGHT && action->convert_to_portal == allocate)
			action->convert_to_portal = idle;
		if ((e->button.button == SDL_BUTTON_LEFT && action->selected_entity >= 0 && action->link_entity == 1))
		{
			action->prev_entity = action->selected_entity;
			action->link_entity = 2;
		}
		if (e->button.button == SDL_BUTTON_RIGHT && action->link_entity == 1)
		{
			action->prev_entity = -1;
			action->link_entity = 0;
		}
		if (e->button.button == SDL_BUTTON_LEFT && action->assign_player_start == 1)
			action->assign_player_start = 2;
		if (e->button.button == SDL_BUTTON_RIGHT && action->assign_player_start == 1)
			action->assign_player_start = 0;
		if (e->button.button == SDL_BUTTON_LEFT && action->assign_end_sector == 1)
			action->assign_end_sector = 2;
		if (e->button.button == SDL_BUTTON_RIGHT && action->assign_end_sector == 1)
			action->assign_end_sector = 0;
	}
	if (e->type == SDL_MOUSEBUTTONUP)
	{
		if (e->button.button == SDL_BUTTON_LEFT )
			mouse_data->i_mbleft = 0;
		if (e->button.button == SDL_BUTTON_RIGHT)
			mouse_data->i_mbright = 0;
	}
}

void	editor_events(SDL_Event *e, t_home *home, t_editor *editor)
{
	while (SDL_PollEvent(e) != 0)
	{
		if (e->type == SDL_KEYDOWN)
			editor_keyboard(e->key.keysym.sym, &editor->action);
		editor_mouse(&editor->mouse_data, e, &editor->action, &editor->buffer);
		if (e->type == SDL_QUIT)
			home->game_state = QUIT;
		if (e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_ESCAPE)
			home->game_state = MAIN_MENU;
	}
}
