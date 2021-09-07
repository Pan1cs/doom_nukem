/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_all_audio_data.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jnivala <jnivala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/07 13:15:40 by jnivala           #+#    #+#             */
/*   Updated: 2021/09/07 15:54:29 by jnivala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/doom_nukem.h"

static void	write_rahikainen(int *fd)
{
	write_audio_data(fd, "./audio/rahikainen_1.wav", "doom_rahikainen_1 #");
	write_audio_data(fd, "./audio/rahikainen_2.wav", "doom_rahikainen_2 #");
	write_audio_data(fd, "./audio/rahikainen_3.wav", "doom_rahikainen_3 #");
	write_audio_data(fd, "./audio/rahikainen_4.wav", "doom_rahikainen_4 #");
	write_audio_data(fd, "./audio/rahikainen_aah_1.wav",
		 "doom_rahikainen_aah_1 #");
	write_audio_data(fd, "./audio/rahikainen_ugh.wav",
		"doom_rahikainen_ugh #");
	write_audio_data(fd, "./audio/rahikainen_ugh_2.wav",
		"doom_rahikainen_ugh_2 #");
	write_audio_data(fd, "./audio/rahikainen_ugh_3.wav",
		"doom_rahikainen_ugh_3 #");
	write_audio_data(fd, "./audio/rahikainen_damage.wav",
		"doom_rahikainen_damage #");
}

static void	write_enemy(int *fd)
{
	write_audio_data(fd, "./audio/skull_skulker_aggro.wav",
		"doom_skull_skulker_aggro #");
	write_audio_data(fd, "./audio/skull_skulker_attack.wav",
		"doom_skull_skulker_attack #");
	write_audio_data(fd, "./audio/skull_skulker_damage.wav",
		"doom_skull_skulker_damage #");
	write_audio_data(fd, "./audio/skull_skulker_death.wav",
		"doom_skull_skulker_death #");
	write_audio_data(fd, "./audio/thing_aggro.wav", "doom_thing_aggro #");
	write_audio_data(fd, "./audio/thing_attack.wav", "doom_thing_attack #");
	write_audio_data(fd, "./audio/thing_damage.wav", "doom_thing_damage #");
	write_audio_data(fd, "./audio/thing_die.wav", "doom_thing_die #");
}

int	write_all_audio_data(int *fd)
{
	const char	*line = "#doom_nukem_audio#";

	if (doom_write(fd, (const void **)&line, 18) == -1)
		error_output("Error: Audio couldn't be written");
	write_audio_data(fd, "./audio/eerie_by_eparviai.wav",
		"doom_music #");
	write_audio_data(fd, "./audio/footstep1.wav", "doom_footstep1 #");
	write_audio_data(fd, "./audio/footstep2.wav", "doom_footstep2 #");
	write_audio_data(fd, "./audio/door_opens_and_closes.wav",
		"doom_door_opens_and_closes #");
	write_audio_data(fd, "./audio/button.wav", "doom_button #");
	write_audio_data(fd, "./audio/plasma_gun.wav", "doom_plasma_gun #");
	write_audio_data(fd, "./audio/error.wav", "doom_error #");
	write_rahikainen(fd);
	write_enemy(fd);
	write_audio_data(fd, "./audio/bolt_locked.wav", "doom_bolt_locked #");
	write_audio_data(fd, "./audio/bolt_unlocked.wav", "doom_bolt_unlocked #");
	write_audio_data(fd, "./audio/lift.wav", "doom_lift #");
	write_audio_data(fd, "./audio/out_of_ammo.wav", "doom_out_of_ammo #");
	write_audio_data(fd, "./audio/reload.wav", "doom_reload #");
	write_audio_data(fd, "./audio/door_unlocked.wav", "doom_door_unlocked #");
	write_audio_data(fd, "./audio/power_station_depleted.wav",
		"doom_power_station_depleted #");
	return (0);
}