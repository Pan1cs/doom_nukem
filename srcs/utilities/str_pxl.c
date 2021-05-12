/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_pxl.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rzukale <rzukale@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/28 08:38:48 by jnivala           #+#    #+#             */
/*   Updated: 2021/05/12 13:20:58 by rzukale          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/doom_nukem.h"

static t_pxl_c	letter_logic(int c, int letter_colour)
{
	if (c >= 'A' && c <= 'Z')
		return (pxl_alphabet(c, black, letter_colour));
	if (c >= '0' && c <= '9')
		return (pxl_numbers(c, black, letter_colour));
	if (c == ':' || c == '\'' || c == '!' || c == '?' || c == ' '
		|| c == '.' || c == ',' || c == '(' || c == ')' || c == '%')
		return (pxl_numbers(c, black, letter_colour));
	return (pxl_numbers(' ', black, letter_colour));
}

static void		handle_letter(Uint32 *buffer, t_xy coord, int c, int letter_colour)
{
	t_xy	cur;
	t_pxl_c	letter;
	t_xy	mod;
	int		m;

	cur.y = 0;
	m = TEXT_SIZE;
	letter = letter_logic(c, letter_colour);
	c = 0;
	while (cur.y < 7)
	{
		cur.x = 0;
		while (cur.x < 5)
		{
			c = letter.c[(int)(cur.x + cur.y * 5)];
			mod.x = cur.x * m + coord.x;
			mod.y = cur.y * m + coord.y;
			draw_rect(mod, vec2(TEXT_SIZE, TEXT_SIZE), buffer, c);
			cur.x++;
		}
		cur.y++;
	}
}

void			str_pxl(Uint32 *buffer, t_xy coord, char *str, int letter_colour)
{
	int		c;

	while (*str != '\0')
	{
		c = ft_toupper(*str);
		handle_letter(buffer, coord, c, letter_colour);
		coord.x += 5 * TEXT_SIZE;
		str++;
	}
}
