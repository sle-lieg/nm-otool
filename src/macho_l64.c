/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   macho_l64.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sle-lieg <sle-lieg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/13 14:24:45 by sle-lieg          #+#    #+#             */
/*   Updated: 2018/09/20 20:05:09 by sle-lieg         ###   ########.fr       */
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

void	add_segment_64(t_file_64 *file, t_segment_64 *seg)
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

t_nlist64_list	*create_new_nlist64(t_nlist_64 *nlist)
{
	t_nlist64_list *new;

	if (!(new = (t_nlist64_list*)malloc(sizeof(*new))))
		exit(EXIT_FAILURE);
	new->symbole = nlist;
	new->next = NULL;
	return (new);
}

t_nlist64_list *find_insert(t_nlist64_list *tmp, char *new_sym, char *str_table)
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

void	insert_nlist(t_file_64 *file, t_nlist_64 *nlist)
{
	t_nlist64_list	*tmp;
	t_nlist64_list	*new;
	char				*name;

	tmp = file->symboles;
	new = create_new_nlist64(nlist);
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

// char	get_segment_type(t_segment64_list *seg, t_nlist_64 *nlist)
char	get_segment_type(t_segment64_list *node, uint32_t n_sect)
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

void	display_data(t_file_64 *file, t_nlist_64 *nlist)
{
	if ((nlist->n_type & N_STAB) && !(g_flags & O_ALL))
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

void	print_list(t_file_64 *file, t_nlist64_list *elem)
{
	if (elem)
	{
		if ((g_flags & O_REV) && !(g_flags & O_UNS))
		{
			print_list(file, elem->next);
			display_data(file, elem->symbole);
		}
		else
		{
			display_data(file, elem->symbole);
			print_list(file, elem->next);
		}
	}
}

/**
 * @brief	read all symbols in the symbol_table and create a t_sym_list *
 * @param  *file:	ptr to the file structure
 */
void	read_sym_table(t_file_64 *file) // add strsize, the size of string table to check if the file is valid
{
	t_nlist_64		*ptr;
	t_nlist64_list *last;
	int nb_sym;

	nb_sym = file->symtab_cmd->nsyms;
	ptr = (t_nlist_64 *)((char*)file->header + file->symtab_cmd->symoff);
	file->symboles = create_new_nlist64(ptr);
	last = file->symboles;
	while (--nb_sym)
	{
		if (g_flags & O_UNS)
		{
			last->next = create_new_nlist64(++ptr);
			last = last->next;
		}
		else
			insert_nlist(file, ++ptr);
	}
	print_list(file, file->symboles);
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