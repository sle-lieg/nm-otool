/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sle-lieg <sle-lieg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/01 17:48:49 by sle-lieg          #+#    #+#             */
/*   Updated: 2018/10/01 18:49:30 by sle-lieg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

void		display_title(void)
{
	if (g_flags & O_LIB_A)
		return ;
	if ((g_flags & O_MULT) || g_arch_name)
		ft_printf("\n%s", g_filename);
	if (g_arch_name)
		ft_printf(" (for architecture %s):\n", g_arch_name);
	else if (g_flags & O_MULT)
		ft_printf(":\n");
}

void		check_limit(void *addr)
{
	if ((char*)addr > g_file_end)
	{
		ft_printf("`%s`: the file is corrupted or truncated.\n", g_filename);
		exit(EXIT_FAILURE);
	}
}

void		set_endianness(uint32_t magic)
{
	if (magic == MH_MAGIC_64 || magic == MH_MAGIC)
		g_little_endian = TRUE;
	else
		g_little_endian = FALSE;
}

uint64_t	get(void *data, size_t size)
{
	uint64_t	ret;
	uint8_t		*ptr;

	check_limit(data + size);
	ret = 0;
	if (g_little_endian)
	{
		ptr = (uint8_t *)data + size;
		while (size--)
			ret = (ret << 8) + *--ptr;
	}
	else
	{
		ptr = (uint8_t *)data;
		while (size--)
			ret = (ret << 8) + *ptr++;
	}
	return (ret);
}

void		insert_obj(t_static_obj *list, t_static_obj *new)
{
	while (list->next)
	{
		if (new->offset == list->offset)
		{
			free(new);
			return ;
		}
		if (new->offset < list->next->offset)
		{
			new->next = list->next;
			list->next = new;
			return ;
		}
		list = list->next;
	}
	if (new->offset == list->offset)
		free(new);
	else
		list->next = new;
}
