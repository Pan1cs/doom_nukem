/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rzukale <rzukale@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/05 19:13:54 by tmaarela          #+#    #+#             */
/*   Updated: 2021/05/11 12:25:47 by rzukale          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/doom_nukem.h"

/*
** if (create_map_file(&home) < 0)
**	 	ft_putendl_fd("File creation failed\n", 2);
*/

void	exit_game(t_home *home, Uint32 *buffer, t_audio *audio)
{
	int i;

	// free_sectors(home);
	free(buffer);
	cleanup_audio(audio);
	i = 0;
	if (home->map_count > 0)
	{
		while (i < home->map_count)
		{
			free(home->map_names[i]);
			i++;
		}
		free(home->map_names);
	}
	free(home->t.frame_times);
	ft_putendl("User closed the window");
	SDL_Quit();
}

void	return_to_main_from_game(t_home *home, t_player *plr)
{
	int i;

	free_sectors(home);
	i = -1;
	while (++i < (home->nbr_of_textures + 1))
		free_texture(home->editor_tex[i]);
	free(home->editor_tex);
	init_player(plr);
}

void	launch_game(t_home *home, t_player *plr, t_frame *frame, SDL_Event *e)
{
	while (home->game_state == GAME_LOOP && home->game_state)
	{
		fps_timer(&home->t);
		update_player(plr, home, e);
		update_screen(home, frame, plr);
		render_buffer(frame->buffer, home->win.ScreenSurface);
		SDL_UpdateWindowSurface(home->win.window);
	}
	return_to_main_from_game(home, plr);
}

void	process_inputs(int *game_state, SDL_Event *e)
{
	while (SDL_PollEvent(e) != 0)
	{
		if (e->type == SDL_KEYDOWN)
		{
			if (e->type == SDL_QUIT)
				*game_state = QUIT;
			if (*game_state == MAIN_MENU && e->key.keysym.sym == SDLK_ESCAPE)
			{
				printf("game state is main menu, calling exit\n");
				*game_state = QUIT;
			}
			if (*game_state != MAIN_MENU && *game_state != QUIT && e->key.keysym.sym == SDLK_ESCAPE)
			{
				printf("going back to main menu\n");
				*game_state = MAIN_MENU;
			}
			if (e->key.keysym.sym == SDLK_2 && *game_state == MAIN_MENU)
				*game_state = MAP_MENU;
			if (e->key.keysym.sym == SDLK_3)
				*game_state = GAME_LOOP;
			if (e->key.keysym.sym == SDLK_1 && *game_state == MAIN_MENU)
				*game_state = EDITOR;
		}
	}
}

static void	get_map_count(int *count, DIR **dir, struct dirent **dir_entry)
{
	char	*found;

	*dir_entry = readdir(*dir);
	while (*dir_entry != NULL)
	{
		found = ft_strstr((*dir_entry)->d_name, ".TEST");
		if (found != NULL)
			(*count)++;
		*dir_entry = readdir(*dir);
	}
	rewinddir(*dir);
}

void	load_map_menu(t_home *home)
{
	DIR				*dir;
	struct dirent	*dir_entry;
	char			*buf;
	char			*found;
	int				i;

	dir = opendir("map_files/");
	if (dir == NULL)
		error_output("Failed to open map_files directory.\n");
	get_map_count(&home->map_count, &dir, &dir_entry);
	if (home->map_count == 0)
	{
		printf("no maps to load\n");
		return ;
	}
	home->map_names = (char **)malloc(sizeof(char *) * home->map_count);
	if (!home->map_names)
		error_output("Failed to allocate memory to map file names.\n");
	dir_entry = readdir(dir);
	i = 0;
	while (dir_entry != NULL)
	{
		found = ft_strstr(dir_entry->d_name, ".TEST");
		if (found != NULL)
		{
			buf = ft_strjoin("map_files/", dir_entry->d_name);
			home->map_names[i] = (char *)malloc(sizeof(char) * ft_strlen((const char *)buf));
			if (!home->map_names[i])
				error_output("Failed to allocate memory to map file name.\n");
			home->map_names[i] = ft_strcpy(home->map_names[i], (const char *)buf);
			ft_strdel(&buf);
		}
		i++;
		dir_entry = readdir(dir);
	}
	closedir(dir);
}

int	main(int argc, char **argv)
{
	t_home		home;
	t_player	plr;
	t_frame		frame;
	SDL_Event	e;

	// TODO: master setup that initializes SDL functions and mallocs buffers etc.
	// launch main_menu loop
	// boolean statement on which branch to launch, editor or game
	// editor state: init all assets from sources, on exit free all assets and go back to main_menu loop
	// game state: request map file, init assets from file and launch game loop. on exit free assets and return to main_menu loop
	// on exit from main_menu loop: free assets initialized during master setup and call exit
	if (argc > 2)
		error_output("usage: ./doom-nukem");
	if (argc == 2)
	{
		printf("%s\n", argv[1]);
		setup(&home, &plr, &frame);
		// setup_editor(&home);
		// create_map_file(&home);
		while (home.game_state != QUIT)
		{
			process_inputs(&home.game_state, &e);
			// setup main_menu graphics
			if (home.game_state == EDITOR)
			{
				printf("olen editorissa\n");
				//load_editor();
			}
			if (home.game_state == MAP_MENU)
			{
				printf("olen map menussa\n");
				if (home.map_count == 0)
					load_map_menu(&home);
				// launch_load_loop();
			}
			if (home.game_state == GAME_LOOP)
			{
				printf("olen game loopissa\n");
				setup_game_loop(argv[1], &home, &plr);
				launch_game(&home, &plr, &frame, &e);
			}
			render_buffer(frame.buffer, home.win.ScreenSurface);
			SDL_UpdateWindowSurface(home.win.window);
		}
	}
	else
		error_output("fuck off\n"); // TODO: Launch main menu
	exit_game(&home, frame.buffer, &plr.audio);
	return (EXIT_SUCCESS);
}
