/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   macho_64.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sle-lieg <sle-lieg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/20 07:03:17 by sle-lieg          #+#    #+#             */
/*   Updated: 2018/10/01 19:04:52 by sle-lieg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

static void				destroy_lists(t_file_64 *file)
{
	t_nlist64_list		*tmp;
	t_segment64_list	*tmp2;

	tmp = file->symboles;
	tmp2 = file->segments;
	while (tmp)
	{
		free(tmp);
		tmp = tmp->next;
	}
	while (tmp2)
	{
		while (tmp2->sections)
		{
			free(tmp2->sections);
			tmp2->sections = tmp2->sections->next;
		}
	}
}

static t_segment64_list	*create_new_seg_64(t_segment_64 *segment)
{
	t_segment64_list *new;

	if (!(new = (t_segment64_list *)malloc(sizeof(*new))))
		exit(EXIT_FAILURE);
	new->seg = segment;
	new->sections = NULL;
	new->next = NULL;
	return (new);
}

static void				add_sections(t_segment64_list *elem, \
									t_section_64 *section)
{
	t_section64_list	*new;
	t_section64_list	*last;
	uint32_t			nb_sections;
	uint32_t			i;

	i = 0;
	nb_sections = get(&elem->seg->nsects, sizeof(elem->seg->nsects));
	last = NULL;
	while (i++ < nb_sections)
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

static void				add_segment(t_file_64 *file, t_segment_64 *seg)
{
	t_segment64_list *tmp;
	t_segment64_list *new;

	tmp = file->segments;
	new = create_new_seg_64(seg);
	if (get(&seg->nsects, sizeof(seg->nsects)) > 0)
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

void					parse_64(char *ptr)
{
	t_file_64	file;
	t_load_cmd	*ldc;
	uint32_t	nb_cmds;
	uint32_t	i;

	file = (t_file_64){NULL, NULL, NULL, NULL, NULL, NULL};
	file.header = (t_mach_h_64 *)ptr;
	file.ld_cmd = (t_load_cmd *)(file.header + 1);
	ldc = file.ld_cmd;
	nb_cmds = get(&file.header->ncmds, sizeof(file.header->ncmds));
	i = 0;
	while (i++ < nb_cmds)
	{
		if (get(&ldc->cmd, sizeof(ldc->cmd)) == LC_SEGMENT_64)
			add_segment(&file, (t_segment_64 *)ldc);
		else if (get(&ldc->cmd, sizeof(ldc->cmd)) == LC_SYMTAB)
			file.symtab_cmd = (t_symtab_cmd *)ldc;
		ldc = (t_load_cmd *)((char*)ldc \
									+ get(&ldc->cmdsize, sizeof(ldc->cmdsize)));
	}
	file.str_table = ptr \
		+ get(&file.symtab_cmd->stroff, sizeof(file.symtab_cmd->stroff));
	read_sym_table_64(&file);
	destroy_lists(&file);
}
