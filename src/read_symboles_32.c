/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_symboles_32.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sle-lieg <sle-lieg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/25 15:34:27 by sle-lieg          #+#    #+#             */
/*   Updated: 2018/09/29 21:00:24 by sle-lieg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

// static t_nlist_list	*create_new_nlist(t_nlist *nlist)
// {
// 	t_nlist_list *new;

// 	if (!(new = (t_nlist_list*)malloc(sizeof(*new))))
// 		exit(EXIT_FAILURE);
// 	new->symbole = nlist;
// 	new->next = NULL;
// 	return (new);
// }

static t_nlist_list	*create_new_nlist(t_nlist *nlist, char *str_table)
{
	t_nlist_list	*new;
	char				*name;

	check_limit((char*)nlist + sizeof(t_nlist));
	name = str_table + get(&nlist->n_un.n_strx, sizeof(nlist->n_un.n_strx));
	check_limit(name);
	if (!(new = (t_nlist_list*)malloc(sizeof(*new))))
		exit(EXIT_FAILURE);
	new->name = name;
	new->symbole = nlist;
	new->next = NULL;
	return (new);
}

// static t_nlist_list *find_insert(t_nlist_list *tmp, char *new_sym, char *str_table)
// {
// 	char *cur_sym;
// 	int32_t	index;

// 	while (tmp->next)
// 	{
// 		index = get(&tmp->next->symbole->n_un.n_strx, sizeof(index));
// 		cur_sym = str_table + index;
// 		if (ft_strcmp(cur_sym, new_sym) > 0)
// 			return (tmp);
// 		tmp = tmp->next;
// 	}
// 	return (tmp);
// }

static t_nlist_list *find_insert(t_nlist_list *tmp, char *new_sym)
{
	while (tmp->next)
	{
		if (ft_strcmp(tmp->next->name, new_sym) > 0)
			return (tmp);
		tmp = tmp->next;
	}
	return (tmp);
}

static void	insert_nlist(t_file *file, t_nlist *nlist)
{
	t_nlist_list	*tmp;
	t_nlist_list	*new;

	tmp = file->symboles;
	new = create_new_nlist(nlist, file->str_table);
	if (ft_strcmp(tmp->name, new->name) > 0)
	{
		file->symboles = new;
		file->symboles->next = tmp;
	}
	else
	{
		tmp = find_insert(file->symboles, new->name);
		new->next = tmp->next;
		tmp->next = new;
	}
}

// static void	insert_nlist(t_file *file, t_nlist *nlist)
// {
// 	t_nlist_list	*tmp;
// 	t_nlist_list	*new;
// 	char				*name;
// 	int32_t			index;

// 	tmp = file->symboles;
// 	new = create_new_nlist(nlist);
// 	index = get(&nlist->n_un.n_strx, sizeof(nlist->n_un.n_strx));
// 	name = file->str_table + index;
// 	index = get(&tmp->symbole->n_un.n_strx, sizeof(tmp->symbole->n_un.n_strx));
// 	if (ft_strcmp(file->str_table + index, name) > 0)
// 	{
// 		file->symboles = new;
// 		file->symboles->next = tmp;
// 	}
// 	else
// 	{
// 		tmp = find_insert(file->symboles, name, file->str_table);
// 		new->next = tmp->next;
// 		tmp->next = new;
// 	}
// }

void	read_sym_table(t_file *file) // add strsize, the size of string table to check if the file is valid
{
	t_nlist			*ptr;
	t_nlist_list	*last;
	uint32_t			nb_sym;
	uint32_t			symtab_offset;

	nb_sym = get(&file->symtab_cmd->nsyms, sizeof(file->symtab_cmd->nsyms));
	symtab_offset = get(&file->symtab_cmd->symoff, sizeof(symtab_offset));
	ptr = (t_nlist *)((char*)file->header + symtab_offset);
	file->symboles = create_new_nlist(ptr, file->str_table);
	last = file->symboles;
	while (--nb_sym)
	{
		if (g_flags & O_UNS)
		{
			last->next = create_new_nlist(++ptr, file->str_table);
			last = last->next;
		}
		else
			insert_nlist(file, ++ptr);
	}
	display_title();
	print_list(file, file->symboles);
}

// void	read_sym_table(t_file *file) // add strsize, the size of string table to check if the file is valid
// {
// 	// ft_printf("READ BIG 32\n");
// 	t_nlist		*ptr;
// 	t_nlist_list *last;
// 	uint32_t nb_sym;
// 	uint32_t symtab_offset;

// 	nb_sym = get(&file->symtab_cmd->nsyms, sizeof(file->symtab_cmd->nsyms));
// 	symtab_offset = get(&file->symtab_cmd->symoff, sizeof(symtab_offset));
// 	ptr = (t_nlist *)((char*)file->header + symtab_offset);
// 	file->symboles = create_new_nlist(ptr);
// 	last = file->symboles;
// 	while (--nb_sym)
// 	{
// 		if (g_flags & O_UNS)
// 		{
// 			last->next = create_new_nlist(++ptr);
// 			last = last->next;
// 		}
// 		else
// 			insert_nlist(file, ++ptr);
// 	}
// 	display_title();
// 	print_list(file, file->symboles);
// }