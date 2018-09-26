/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_symboles_32.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sle-lieg <sle-lieg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/25 15:50:21 by sle-lieg          #+#    #+#             */
/*   Updated: 2018/09/26 15:15:21 by sle-lieg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

static char	get_segment_type(t_segment_list *node, uint8_t n_sect)
{
	uint32_t seg_nb_sections;

	while (1)
	{
		seg_nb_sections = get(&node->seg->nsects, sizeof(node->seg->nsects));
		if (n_sect <= seg_nb_sections)
			break;
		n_sect -= seg_nb_sections;
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

static char	get_type(t_file *file, t_nlist *nlist)
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
	return (type_c);
}

static void	display_data(t_file *file, t_nlist *nlist)
{
	uint32_t n_value;
	int32_t	index;
	char		type_c;

	n_value = get(&nlist->n_value, sizeof(nlist->n_value));
	index = get(&nlist->n_un.n_strx, sizeof(nlist->n_un.n_strx));
	type_c = get_type(file, nlist);
	if (nlist->n_type & N_STAB)
		return;
	if (g_flags & O_FIL)
		ft_printf("%s: ", g_filename);
	if (n_value || type_c == 't' || type_c == 'T')
		ft_printf("%08x ", n_value);
	else
		ft_printf("         ");
	ft_printf("%c %s\n", type_c, file->str_table + index);
}

void	print_list(t_file *file, t_nlist_list *elem)
{
	// ft_printf("PRINT LIST BIG 32\n");
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