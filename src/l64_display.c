/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   l64_display.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sle-lieg <sle-lieg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/25 11:55:05 by sle-lieg          #+#    #+#             */
/*   Updated: 2018/09/25 11:56:43 by sle-lieg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

// char	get_segment_type(t_segment64_list *seg, t_nlist_64 *nlist)
static char	get_segment_type(t_segment64_list *node, uint32_t n_sect)
{
	while (n_sect > node->seg->nsects)
	{
		n_sect -= node->seg->nsects;
		node = node->next;
	}
	if (!ft_strcmp(node->sections->sect[n_sect-1].sectname, SECT_TEXT))
		return ('T');
	if (!ft_strcmp(node->sections->sect[n_sect-1].sectname, SECT_DATA))
		return ('D');
	if (!ft_strcmp(node->sections->sect[n_sect-1].sectname, SECT_BSS))
		return ('B');
	return ('S');
}

static void	print_type(t_file_64 *file, t_nlist_64 *nlist)
{
	uint8_t	type;
	char		type_c;

	type = nlist->n_type & N_TYPE;
	type_c = ' ';
	if (nlist->n_type & N_STAB)
		type_c = '-';
	else if (type == N_UNDF)
		type_c = 'U';
	else if (type == N_ABS)
		type_c = 'A';
	else if (type == N_INDR)
		type_c = 'I';
	else if (type == N_UNDF && (nlist->n_type & N_EXT))
		type_c = 'C';
	else if (type == N_SECT)
		type_c = get_segment_type(file->segments, nlist->n_sect);
	if (!(nlist->n_type & N_EXT))
		type_c |= 0x20;
	ft_printf("%c ", type_c);
}

static void	display_data64(t_file_64 *file, t_nlist_64 *nlist)
{
	if (nlist->n_type & N_STAB)
		return;
	if (g_flags & O_FIL)
		ft_printf("%s: ", g_filename);
	if (nlist->n_value || (nlist->n_type & N_STAB))
		ft_printf("%016lx ", nlist->n_value);
	else
		ft_printf("                 ");
	print_type(file, nlist);
	if (g_flags & O_ALL && (nlist->n_type & N_STAB))
		ft_printf("%02x %04x %5s ", nlist->n_sect, nlist->n_desc, get_stab_type(nlist->n_type));
	ft_printf("%s\n", file->str_table + nlist->n_un.n_strx);
}

void	print_list64(t_file_64 *file, t_nlist64_list *elem)
{
	if (elem)
	{
		if (!(g_flags & O_REV))
		{
			display_data(file, elem->symbole);
			print_list(file, elem->next);
		}
		else
		{
			print_list(file, elem->next);
			display_data(file, elem->symbole);
		}
	}
}