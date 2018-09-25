/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   macho_l32.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sle-lieg <sle-lieg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/20 07:03:41 by sle-lieg          #+#    #+#             */
/*   Updated: 2018/09/25 19:07:58 by sle-lieg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

static void	add_sections(t_segment_list *elem, t_section *section)
{
	t_section_list *new;
	t_section_list *last;
	uint32_t i;

	i = 0;
	last = NULL;
	while (i++ < elem->seg->nsects)
	{
		if (!(new = (t_section_list *)malloc(sizeof(*new))))
			exit(EXIT_FAILURE);
		new->sect = section;
		new->next = NULL;
		if (last)
			last->next = new;
		else
			elem->sections = new;
		last = new;
		++section;
	}
}

static void	add_segment(t_file *file, t_segment *seg)
{
	t_segment_list	*tmp;
	t_segment_list	*new;

	tmp = file->segments;
	new = create_new_seg_32(seg);
	if (seg->nsects > 0)
		add_sections(new, (t_section*)(seg + 1));
	if (!file->segments)
		file->segments = new;
	else
	{
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
}

/*
** @note		handle mach-o big endian 32 bits binary
** @param	*ptr: start of the file mapping
*/
void	parse_little_endian_32(char *ptr)
{
	ft_printf("PARSE LITTLE 32\n");

	t_file		file;
	t_load_cmd	*ldc;
	uint32_t		i;

	file = (t_file){NULL, NULL, NULL, NULL, NULL, NULL};
	file.header = (t_mach_h *)ptr;
	file.ld_cmd = (t_load_cmd *)(file.header + 1);
	ldc = file.ld_cmd;
	i = 0;
	while (i++ < file.header->ncmds)
	{
		// ft_printf("ldc[%p]: type=%u size=%u\n", ldc, ldc->cmd, ldc->cmdsize);
		if (ldc->cmd == LC_SEGMENT)
			add_segment(&file, (t_segment*)ldc);
		if (ldc->cmd == LC_SYMTAB)
			file.symtab_cmd = (t_symtab_cmd *)ldc;
		ldc = (t_load_cmd *)((char*)ldc + ldc->cmdsize);
	}
	file.str_table = ptr + file.symtab_cmd->stroff;
	read_sym_table(&file);
}