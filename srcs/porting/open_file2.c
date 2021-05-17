/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_file2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rzukale <rzukale@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/11 14:02:59 by rzukale           #+#    #+#             */
/*   Updated: 2021/05/17 12:56:53 by rzukale          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/doom_nukem.h"

t_texture	*get_texture(unsigned char *buf, unsigned int *pos, ssize_t size)
{
	t_png		png;
	int			i;
	int			idx;

	idx = 0;
	i = -1;
	while (++i < 3)
	{
		(*pos) += get_next_breaker(buf + (*pos)) + 1;
		if (*pos > (unsigned int)size)
			error_output("Pointer points outside memory address\n");
		if (i == 0)
			idx = ft_atoi((char *)(buf + (*pos)));
		else if (i == 1)
			png.source.size = ft_atoi((char *)(buf + (*pos)));
		else if (i == 2)
		{
			setup_parser(&png, png.source.size);
			ft_memcpy(png.source.buf, buf + *pos, png.source.size);
			(*pos) += png.source.size;
			if (*pos > (unsigned int)size)
				error_output("Pointer points outside memory address\n");
		}
	}
	return (return_new_texture(&png, idx));
}

t_texture	*assign_empty_texture(void)
{
	t_texture	*tex;

	tex = (t_texture *)malloc(sizeof(t_texture));
	if (!tex)
		error_output("Memory allocation of t_texture struct failed\n");
	tex->source = (unsigned char *)malloc(sizeof(unsigned char) * 1);
	if (!tex->source)
		error_output("Memory allocation of editor pixel pointer failed\n");
	tex->pixels = (unsigned int *)malloc(sizeof(unsigned int) * 1);
	if (!tex->pixels)
		error_output("Memory allocation of pixel pointer failed\n");
	return (tex);
}

void	parse_texture_data(unsigned char *buf, t_home *home,
	unsigned int *pos, ssize_t size)
{
	int				i;

	*pos = 0;
	buf = (unsigned char *)ft_strstr((char *)buf, "doom_textures");
	*pos += get_next_breaker(buf + *pos) + 1;
	if (*pos > (unsigned int)size)
		error_output("Pointer points outside memory address\n");
	home->nbr_of_textures = ft_atoi((char *)buf + *pos);
	home->editor_tex = (t_texture **)malloc(sizeof(t_texture *)
			* (home->nbr_of_textures + 1));
	if (!home->editor_tex)
		error_output("failed to allocate memory to editor textures\n");
	home->editor_tex[0] = assign_empty_texture();
	i = 1;
	while (i <= home->nbr_of_textures)
	{
		home->editor_tex[i] = get_texture(buf, pos, size);
		i++;
	}
}

void	parse_audio_data(unsigned char *buf, unsigned int *pos,
	char *path, ssize_t size)
{
	t_audio_asset	asset;

	*pos += get_next_breaker(buf + *pos) + 1;
	if (*pos > (unsigned int)size)
		error_output("Pointer points outside memory address\n");
	asset.size = ft_atoi((char *)buf + *pos);
	asset.buf = malloc(sizeof(unsigned char *) * asset.size);
	if (!asset.buf)
		error_output("failed to allocate memory to music buffer\n");
	*pos += get_next_breaker(buf + *pos) + 1;
	if (*pos > (unsigned int)size)
		error_output("Pointer points outside memory address\n");
	ft_memcpy(asset.buf, buf + *pos, asset.size);
	*pos += asset.size;
	if (*pos > (unsigned int)size)
		error_output("Pointer points outside memory address\n");
	if (create_temp_audio_file(asset.buf, asset.size, path) == -1)
		error_output("Failed to create temp audio file\n");
	free(asset.buf);
}

int	open_file(t_home *home, char *path)
{
	int				fd;
	unsigned char	*buf;
	ssize_t			size;
	unsigned int	pos;

	doom_open(&fd, (const char **)&path, READ_ONLY, 0644);
	if (fd < 0)
		error_output("Failed to open file\n");
	else
	{
		buf = (unsigned char *)malloc(sizeof(unsigned char) * MAX_SIZE);
		if (!buf)
			error_output("Memory allocation of source buffer failed\n");
		doom_read(&size, &fd, (void **)&buf, MAX_SIZE);
		if (size <= 0)
			error_output("Failed to read file\n");
		else if (size >= MAX_SIZE)
			error_output("File is too large\n");
		if (doom_close(&fd) == -1)
			error_output("Could not close file\n");
		parse_texture_data(buf, home, &pos, size);
		if (doom_mkdir() == -1)
			printf("Failed to create temporary directory\n");
		parse_audio_data(buf, &pos, "./temp/music.wav", size);
		parse_audio_data(buf, &pos, "./temp/footstep1.wav", size);
		parse_audio_data(buf, &pos, "./temp/footstep2.wav", size);
		free(buf);
	}
	return (1);
}
