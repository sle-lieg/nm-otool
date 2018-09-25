/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   l64_read_symboles.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sle-lieg <sle-lieg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/13 14:28:43 by sle-lieg          #+#    #+#             */
/*   Updated: 2018/09/25 16:09:01 by sle-lieg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

static t_nlist64_list	*create_new_nlist_64(t_nlist_64 *nlist)
{
	t_nlist64_list *new;

	if (!(new = (t_nlist64_list*)malloc(sizeof(*new))))
		exit(EXIT_FAILURE);
	new->symbole = nlist;
	new->next = NULL;
	return (new);
}

static t_nlist64_list *find_insert(t_nlist64_list *tmp, char *new_sym, char *str_table)
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

static void	insert_nlist(t_file_64 *file, t_nlist_64 *nlist)
{
	t_nlist64_list	*tmp;
	t_nlist64_list	*new;
	char				*name;

	tmp = file->symboles;
	new = create_new_nlist_64(nlist);
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

/**
 * @brief	read all symbols in the symbol_table and create a t_sym_list *
 * @param  *file:	ptr to the file structure
 */
void	read_sym_table_64(t_file_64 *file) // add strsize, the size of string table to check if the file is valid
{
	t_nlist_64		*ptr;
	t_nlist64_list *last;
	int nb_sym;

	nb_sym = file->symtab_cmd->nsyms;
	ptr = (t_nlist_64 *)((char*)file->header + file->symtab_cmd->symoff);
	file->symboles = create_new_nlist_64(ptr);
	last = file->symboles;
	while (--nb_sym)
	{
		if (g_flags & O_UNS)
		{
			last->next = create_new_nlist_64(++ptr);
			last = last->next;
		}
		else
			insert_nlist(file, ++ptr);
	}
	print_list_64(file, file->symboles);
}