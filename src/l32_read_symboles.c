/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   l32_read_symboles.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sle-lieg <sle-lieg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/25 13:30:21 by sle-lieg          #+#    #+#             */
/*   Updated: 2018/09/25 17:11:06 by sle-lieg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

t_nlist_list	*create_new_nlist(t_nlist *nlist)
{
	t_nlist_list *new;

	if (!(new = (t_nlist_list*)malloc(sizeof(*new))))
		exit(EXIT_FAILURE);
	new->symbole = nlist;
	new->next = NULL;
	return (new);
}

static t_nlist_list *find_insert(t_nlist_list *tmp, char *new_sym, char *str_table)
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

static void	insert_nlist(t_file *file, t_nlist *nlist)
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

void	read_sym_table(t_file *file) // add strsize, the size of string table to check if the file is valid
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
