/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sector_actions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rzukale <rzukale@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/11 14:51:17 by jnivala           #+#    #+#             */
/*   Updated: 2021/09/23 15:59:33 by rzukale          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/doom_nukem.h"

void	editor_free_walls(t_editor_walls **head, int nbr_of_walls)
{
	t_editor_walls	*item;

	while (nbr_of_walls--)
	{
		item = *head;
		*head = item->next;
		free(item);
		item = NULL;
	}
}

int	editor_new_sector_wallpoints(t_editor *editor,
	t_mouse_data *mouse_data, t_action *action)
{
	t_editor_sector	*temp;

	temp = get_editor_sector_with_idx(&editor->sector_list,
			action->selected_sector);
	if (temp && mouse_data->i_mbright)
		return (editor_sector_remove_wallpoint(editor, temp));
	if (temp && mouse_data->i_mbleft)
	{
		if (add_point_to_list(editor, temp, action) == 3)
		{
			editor->temp_sector = NULL;
			editor->action.draw_depth = depth_zero;
			editor->action.create_sector = idle;
			editor->mouse_data.i_mbleft = 0;
			return (1);
		}
	}
	mouse_data->i_mbleft = 0;
	return (0);
}

static void	loop_for_portals_and_set_them_to_new_idx(t_editor_sector *sector, t_editor_sector **head, int new_idx)
{
	t_editor_walls	*wall;
	t_editor_walls	*temp_wall;
	t_editor_sector	*temp;
	int				i;

	i = 0;
	wall = sector->walls;
	while (i < sector->nb_of_walls)
	{
		if (wall->type >= 0)
		{
			temp = get_editor_sector_with_idx(head, wall->type);
			if (temp)
			{
				temp_wall = get_editor_wall_with_type(&temp->walls, temp->nb_of_walls, sector->idx_sector);
				if (temp_wall)
					temp_wall->type = new_idx;
			}
		}
		i++;
		wall = wall->next;
	}
}

void	reset_sector_indexes(t_editor_sector **head)
{
	t_editor_sector	*temp;
	int				idx;

	idx = 0;
	temp = *head;
	while (temp)
	{
		loop_for_portals_and_set_them_to_new_idx(temp, head, idx);
		temp->idx_sector = idx;
		temp = temp->next;
		idx++;
	}
}

void	editor_free_sector_data(t_editor_sector **sector,
		t_entity_list **entity_head)
{
	t_editor_sector	*temp;

	temp = *sector;
	delete_entities_from_sector(entity_head, temp->idx_sector);
	editor_free_walls(&temp->walls, temp->nb_of_walls);
	if (temp->plot_line)
		free(temp->plot_line);
	free(temp);
}

void	editor_free_selected_sector(t_editor_sector **head,
		t_entity_list **entity_head, t_action *action)
{
	t_editor_sector	*temp;
	t_editor_sector	*prev;

	if (!*head)
		return ;
	temp = *head;
	if (temp != NULL && temp->idx_sector == action->selected_sector)
	{
		*head = temp->next;
		check_for_portals_and_set_defaults(temp, head);
		editor_free_sector_data(&temp, entity_head);
		reset_sector_indexes(head);
		return ;
	}
	while (temp != NULL && temp->idx_sector != action->selected_sector)
	{
		prev = temp;
		temp = temp->next;
	}
	if (temp == NULL)
		return ;
	prev->next = temp->next;
	check_for_portals_and_set_defaults(temp, head);
	editor_free_sector_data(&temp, entity_head);
	reset_sector_indexes(head);
}
