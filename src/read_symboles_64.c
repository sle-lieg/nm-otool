/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_symboles_64.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sle-lieg <sle-lieg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/25 18:35:28 by sle-lieg          #+#    #+#             */
/*   Updated: 2018/09/30 15:52:55 by sle-lieg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

// t_nlist64_list	*create_new_nlist_64(t_nlist_64 *nlist)
// {
// 	t_nlist64_list *new;

// 	check_limit((char*)nlist + sizeof(t_nlist_64));
// 	if (!(new = (t_nlist64_list*)malloc(sizeof(*new))))
// 		exit(EXIT_FAILURE);
// 	new->symbole = nlist;
// 	new->next = NULL;
// 	return (new);
// }

t_nlist64_list	*create_new_nlist_64(t_nlist_64 *nlist, char *str_table)
{
	t_nlist64_list *new;
	char				*name;

	check_limit((char*)nlist + sizeof(t_nlist_64));
	name = str_table + get(&nlist->n_un.n_strx, sizeof(nlist->n_un.n_strx));
	check_limit(name);
	if (!(new = (t_nlist64_list*)malloc(sizeof(*new))))
		exit(EXIT_FAILURE);
	new->name = name;
	new->symbole = nlist;
	new->next = NULL;
	return (new);
}

// static t_nlist64_list *find_insert(t_nlist64_list *tmp, char *new_sym, char *str_table)
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

static t_nlist64_list *find_insert(t_nlist64_list *tmp, char *new_sym)
{
	while (tmp->next)
	{
		if (ft_strcmp(tmp->next->name, new_sym) > 0)
			return (tmp);
		tmp = tmp->next;
	}
	return (tmp);
}

static void	insert_nlist(t_file_64 *file, t_nlist_64 *nlist)
{
	t_nlist64_list	*tmp;
	t_nlist64_list	*new;

	tmp = file->symboles;
	new = create_new_nlist_64(nlist, file->str_table);
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

// static void	insert_nlist(t_file_64 *file, t_nlist_64 *nlist)
// {
// 	t_nlist64_list	*tmp;
// 	t_nlist64_list	*new;
// 	char				*name;
// 	int32_t			index;

// 	tmp = file->symboles;
// 	new = create_new_nlist_64(nlist);
// 	// index of nlist to insert name
// 	index = get(&nlist->n_un.n_strx, sizeof(nlist->n_un.n_strx));
// 	name = file->str_table + index;
// 	check_limit(name + ft_strlen(name));
// 	// index of the name to compare
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

void	display_title()
{
	if (g_flags & O_LIB_A)
		return ;
	if ((g_flags & O_MULT) || g_arch_name)
		ft_printf("\n%s", g_filename);
	if (g_arch_name)
		ft_printf(" (for architecture %s):\n", g_arch_name);
	else if (g_flags & O_MULT)
		ft_printf(":\n");
}

void	read_sym_table_64(t_file_64 *file)
{
	t_nlist_64		*ptr;
	t_nlist64_list *last;
	uint32_t nb_sym;
	uint32_t symtab_offset;

	nb_sym = get(&file->symtab_cmd->nsyms, sizeof(file->symtab_cmd->nsyms));
	symtab_offset = get(&file->symtab_cmd->symoff, sizeof(symtab_offset));
	ptr = (t_nlist_64 *)((char*)file->header + symtab_offset);
	file->symboles = create_new_nlist_64(ptr, file->str_table);
	last = file->symboles;
	while (--nb_sym)
	{
		if (g_flags & O_UNS)
		{
			last->next = create_new_nlist_64(++ptr, file->str_table);
			last = last->next;
		}
		else
			insert_nlist(file, ++ptr);
	}
	display_title();
	print_list_64(file, file->symboles);
}

// void	read_sym_table_64(t_file_64 *file)
// {
// 	t_nlist_64		*ptr;
// 	t_nlist64_list *last;
// 	uint32_t nb_sym;
// 	uint32_t symtab_offset;

// 	nb_sym = get(&file->symtab_cmd->nsyms, sizeof(file->symtab_cmd->nsyms));
// 	symtab_offset = get(&file->symtab_cmd->symoff, sizeof(symtab_offset));
// 	ptr = (t_nlist_64 *)((char*)file->header + symtab_offset);
// 	file->symboles = create_new_nlist_64(ptr);
// 	last = file->symboles;
// 	while (--nb_sym)
// 	{
// 		if (g_flags & O_UNS)
// 		{
// 			last->next = create_new_nlist_64(++ptr);
// 			last = last->next;
// 		}
// 		else
// 			insert_nlist(file, ++ptr);
// 	}
// 	display_title();
// 	print_list_64(file, file->symboles);
// }