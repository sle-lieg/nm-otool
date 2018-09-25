/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   macho_b64.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sle-lieg <sle-lieg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/20 07:03:17 by sle-lieg          #+#    #+#             */
/*   Updated: 2018/09/25 10:52:54 by sle-lieg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

void	print_list(t_file *file, t_nlist_list *elem)
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

t_nlist_list *find_insert(t_nlist_list *tmp, char *new_sym, char *str_table)
{
	char *cur_sym;

	while (tmp->next)
	{
		cur_sym = str_table + tmp->next->symbole->n_un.n_strx;
		if (ft_strcmp(cur_sym, new_sym) > 0)
			return (tmp);
		tmp = tmp->next;
	}
	return (tmp);
}

void	insert_nlist(t_file *file, t_nlist *nlist)
{
	t_nlist_list	*tmp;
	t_nlist_list	*new;
	char				*name;

	tmp = file->symboles;
	new = create_new_nlist(nlist);
	name = file->str_table + nlist->n_un.n_strx;
	if (ft_strcmp(file->str_table + tmp->symbole->n_un.n_strx, name) > 0)
	{
		file->symboles = new;
		file->symboles->next = tmp;
	}
	else
	{
		tmp = find_insert(file->symboles, name, file->str_table);
		new->next = tmp->next;
		tmp->next = new;
	}
}

t_nlist_list	*create_new_nlist(t_nlist *nlist)
{
	t_nlist_list *new;

	if (!(new = (t_nlist_list*)malloc(sizeof(*new))))
		exit(EXIT_FAILURE);
	new->symbole = nlist;
	new->next = NULL;
	return (new);
}

static void	read_sym_table(t_file *file) // add strsize, the size of string table to check if the file is valid
{
	t_nlist		*ptr;
	t_nlist_list *last;
	int nb_sym;

	nb_sym = file->symtab_cmd->nsyms;
	ptr = (t_nlist *)((char*)file->header + file->symtab_cmd->symoff);
	file->symboles = create_new_nlist(ptr);
	last = file->symboles;
	while (--nb_sym)
	{
		if (g_flags & O_UNS)
		{
			last->next = create_new_nlist(++ptr);
			last = last->next;
		}
		else
			insert_nlist(file, ++ptr);
	}
	print_list(file, file->symboles);
}

static t_segment_list *create_new_seg(t_segment *segment)
{
	t_segment_list *new;

	if (!(new = (t_segment_list *)malloc(sizeof(*new))))
		exit(EXIT_FAILURE);
	new->seg = segment;
	new->sections = NULL;
	new->next = NULL;
	return (new);
}

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

void	add_segment(t_file *file, t_segment *seg)
{
	t_segment_list	*tmp;
	t_segment_list	*new;

	tmp = file->segments;
	new = create_new_seg(seg);
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
void	parse_b64(char *ptr)
{
	ft_printf("%x: Big endian 64 bits\n", *(uint32_t*)ptr);

	t_file		file;
	t_load_cmd	*ldc;
	uint32_t		i;

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