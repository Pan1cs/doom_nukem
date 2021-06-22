/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jnivala <jnivala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/21 14:36:51 by tmaarela          #+#    #+#             */
/*   Updated: 2021/06/22 13:34:45 by jnivala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PLAYER_H
# define PLAYER_H

enum e_plot_states
{
	beginning,
	enemy_sighted,
	contemplation,
	warning,
	plot_twist,
	ending,
};

typedef struct s_input
{
	int				right;
	int				left;
	int				up;
	int				down;
	int				rot_right;
	int				rot_left;
	int				crouch;
	int				jump;
	int				wireframe;
	int				minimap;
	int				info;
	int				mouse;
	int				jetpack;
	int				use;
	int				debug_up;
	int				debug_down;
	int				debug_left;
	int				debug_right;
}					t_input;

typedef struct s_player
{
	t_input			input;
	t_xyz			pos;
	t_xyz			look_dir;
	t_xyz			move_dir;
	t_xyz			target;
	t_xyz			up;
	t_xy			dir;
	float			yaw;
	float			pitch;
	float			height;
	float			acceleration;
	int				cur_sector;
	int				plot_state;
	int				enemy_sighted;
	Uint32			time;
	t_audio			audio;
}					t_player;

enum e_movement
{
	open_space = -9999
};


/*
** Player functions
*/

t_wall	*check_if_crossing(t_sector *sector, t_xyz pos);
t_xyz	check_y(t_sector *sector, t_player *plr, t_xyz pos);
int		check_y_diff(t_player *plr, t_xyz *test_pos, t_sector *to);
Uint32	check_distance_to_ceiling(t_sector *sector, t_xyz *new_loc);
float	check_distance_to_ground(t_sector *sector, t_player *plr, t_xyz pos);
int		check_index_of_a_wall(t_sector *sector, t_xyz pos, t_xyz dir);
Uint32	close_doors(t_sector **sectors, Uint32 nb_of_sectors,
		Uint32 current_time, Uint32 delta_time);
void	create_target_vector(t_player *plr);
void	crouch(t_player *plr);
void	gravity(t_sector *sector, t_player *plr, Uint32 delta_time);
void	init_player(t_player *plr);
int		jetpack(t_player *plr, t_home *home, Uint32 t);
void	jump(t_player *plr, t_sector *cur_sector);
int		open_door(t_sector **sectors, t_xyz look_loc,
		Uint32 current_time, Uint32 cur_sector);
int		player_move(t_player *plr, t_home *home, Uint32 delta_time);
int		plr_inside(t_sector *sector, t_xy *pos);
int		player_use(t_player *plr, t_home *home, Uint32 delta_time);
void	update_player(t_player *plr, t_home *home, Uint32 delta_time);
#endif
