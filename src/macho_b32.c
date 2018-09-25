/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   macho_b32.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sle-lieg <sle-lieg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/20 07:04:06 by sle-lieg          #+#    #+#             */
/*   Updated: 2018/09/25 17:30:27 by sle-lieg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

static void	add_sections(t_segment_list *elem, t_section *section)
{
	t_section_list *new;
	t_section_list *last;
	uint32_t nb_sections;
	uint32_t i;

	i = 0;
	nb_sections = get(&elem->seg->nsects, sizeof(elem->seg->nsects));
	last = NULL;
	while (i++ < nb_sections)
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
	if (get(&seg->nsects, sizeof(seg->nsects)) > 0)
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

void	parse_big_endian_32(char *ptr)
{
	ft_printf("PARSE BIG 32\n");
	t_file		file;
	t_load_cmd	*ldc;
	uint32_t		cmd;
	uint32_t		nb_cmds;
	uint32_t		i;

	file = (t_file){NULL, NULL, NULL, NULL, NULL, NULL};
	file.header = (t_mach_h *)ptr;
	file.ld_cmd = (t_load_cmd *)(file.header + 1);
	ldc = file.ld_cmd;
	nb_cmds = get(&file.header->ncmds, sizeof(file.header->ncmds));
	i = 0;
	ft_printf("nb cmds = %u\n", nb_cmds);
	while (i++ < nb_cmds)
	{
		cmd = get(&ldc->cmd, sizeof(ldc->cmd));
		ft_printf("ldc[%p]: type=%u size=%u\n", ldc, cmd, ldc->cmdsize);
		if (cmd == LC_SEGMENT)
			add_segment(&file, (t_segment*)ldc);
		else if (cmd == LC_SYMTAB)
			file.symtab_cmd = (t_symtab_cmd *)ldc;
		ldc = (t_load_cmd *)((char*)ldc + get(&ldc->cmdsize, sizeof(ldc->cmdsize)));
	}
	file.str_table = ptr + get(&file.symtab_cmd->stroff, sizeof(file.symtab_cmd->stroff));
	read_sym_table_b(&file);
}
