/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   macho_l64.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sle-lieg <sle-lieg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/13 14:24:45 by sle-lieg          #+#    #+#             */
/*   Updated: 2018/09/25 11:57:15 by sle-lieg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

static t_segment64_list *create_new_seg64(t_segment_64 *segment)
{
	t_segment64_list *new;

	if (!(new = (t_segment64_list *)malloc(sizeof(*new))))
		exit(EXIT_FAILURE);
	new->seg = segment;
	new->sections = NULL;
	new->next = NULL;
	return (new);
}

static void	add_sections(t_segment64_list *elem, t_section_64 *section)
{
	t_section64_list *new;
	t_section64_list *last;
	uint32_t i;

	i = 0;
	last = NULL;
	while (i++ < elem->seg->nsects)
	{
		if (!(new = (t_section64_list *)malloc(sizeof(*new))))
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

static void	add_segment_64(t_file_64 *file, t_segment_64 *seg)
{
	t_segment64_list	*tmp;
	t_segment64_list	*new;

	tmp = file->segments;
	new = create_new_seg64(seg);
	if (seg->nsects > 0)
		add_sections(new, (t_section_64*)(seg + 1));
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
** @note		handle mach-o little endian 64 bits binary
** @param	*ptr: start of the file mapping
*/
void	parse_l64(char *ptr)
{
	t_file_64	file;
	t_load_cmd	*ldc;
	uint32_t		i;

	file.header = (t_mach_h_64 *)ptr;
	file.ld_cmd = (t_load_cmd *)(file.header + 1);
	ldc = file.ld_cmd;
	i = 0;
	while (i++ < file.header->ncmds)
	{
		// ft_printf("ldc[%p]: type=%u size=%u\n", ldc, ldc->cmd, ldc->cmdsize);
		if (ldc->cmd == LC_SEGMENT_64)
			add_segment_64(&file, (t_segment_64*)ldc);
		if (ldc->cmd == LC_SYMTAB)
			file.symtab_cmd = (t_symtab_cmd *)ldc;
		ldc = (struct load_command *)((char*)ldc + ldc->cmdsize);
	}
	file.str_table = ptr + file.symtab_cmd->stroff;
	read_sym_table(&file);
	// read_sym_table((struct nlist_64 *)(ptr + scmd->symoff), ptr + scmd->stroff, scmd->nsyms);
}