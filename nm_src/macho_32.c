/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   macho_32.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sle-lieg <sle-lieg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/20 07:04:06 by sle-lieg          #+#    #+#             */
/*   Updated: 2018/10/02 16:02:31 by sle-lieg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

static void				destroy_lists(t_file *file)
{
	t_nlist_list	*tmp;
	t_segment_list	*tmp2;
	void			*to_delete;

	tmp = file->symboles;
	tmp2 = file->segments;
	while (tmp)
	{
		to_delete = tmp;
		tmp = tmp->next;
		free(to_delete);
	}
	while (tmp2)
	{
		while (tmp2->sections)
		{
			to_delete = tmp2->sections;
			tmp2->sections = tmp2->sections->next;
			free(to_delete);
		}
		to_delete = tmp2;
		tmp2 = tmp2->next;
		free(to_delete);
	}
}

static t_segment_list	*create_new_seg_32(t_segment *segment)
{
	t_segment_list *new;

	if (!(new = (t_segment_list *)malloc(sizeof(*new))))
		exit(EXIT_FAILURE);
	new->seg = segment;
	new->sections = NULL;
	new->next = NULL;
	return (new);
}

static void				add_sections(t_segment_list *elem, t_section *section)
{
	t_section_list	*new;
	t_section_list	*last;
	uint32_t		nb_sections;
	uint32_t		i;

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

static void				add_segment(t_file *file, t_segment *seg)
{
	t_segment_list *tmp;
	t_segment_list *new;

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

void					parse_32(char *ptr)
{
	t_file		file;
	t_load_cmd	*ldc;
	uint32_t	nb_cmds;
	uint32_t	i;

	file = (t_file){NULL, NULL, NULL, NULL, NULL, NULL};
	file.header = (t_mach_h *)ptr;
	file.ld_cmd = (t_load_cmd *)(file.header + 1);
	ldc = file.ld_cmd;
	nb_cmds = get(&file.header->ncmds, sizeof(file.header->ncmds));
	i = 0;
	while (i++ < nb_cmds)
	{
		if (get(&ldc->cmd, sizeof(ldc->cmd)) == LC_SEGMENT)
			add_segment(&file, (t_segment*)ldc);
		else if (get(&ldc->cmd, sizeof(ldc->cmd)) == LC_SYMTAB)
			file.symtab_cmd = (t_symtab_cmd *)ldc;
		ldc = (t_load_cmd *)((char*)ldc + get(&ldc->cmdsize, sizeof(uint32_t)));
	}
	if (file.symtab_cmd)
	{
		file.str_table = ptr + get(&file.symtab_cmd->stroff, sizeof(uint32_t));
		read_sym_table(&file);
	}
	destroy_lists(&file);
}
