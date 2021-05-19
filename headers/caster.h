/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   caster.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jnivala <jnivala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/22 13:51:14 by tmaarela          #+#    #+#             */
/*   Updated: 2021/05/17 16:44:42 by jnivala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CASTER_H
# define CASTER_H

/*
** Raycaster
*/

void			draw_fov_floor(t_home *home, t_player *plr);

t_xy			init_ray(t_xy pos, t_xy plane, t_xy left, float angle);
void			draw_fov(t_home *home, t_player *plr);
void			draw_frame(t_home *home, t_frame *frame, t_player *plr);

#endif
