/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_symboles_32.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sle-lieg <sle-lieg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/25 15:34:27 by sle-lieg          #+#    #+#             */
/*   Updated: 2018/09/27 19:14:20 by sle-lieg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

static t_nlist_list	*create_new_nlist(t_nlist *nlist)
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
	int32_t	index;

	while (tmp->next)
	{
		index = get(&tmp->next->symbole->n_un.n_strx, sizeof(index));
		cur_sym = str_table + index;
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
	int32_t			index;

	tmp = file->symboles;
	new = create_new_nlist(nlist);
	index = get(&nlist->n_un.n_strx, sizeof(nlist->n_un.n_strx));
	name = file->str_table + index;
	index = get(&tmp->symbole->n_un.n_strx, sizeof(tmp->symbole->n_un.n_strx));
	if (ft_strcmp(file->str_table + index, name) > 0)
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
	// ft_printf("READ BIG 32\n");
	t_nlist		*ptr;
	t_nlist_list *last;
	uint32_t nb_sym;
	uint32_t symtab_offset;

	nb_sym = get(&file->symtab_cmd->nsyms, sizeof(file->symtab_cmd->nsyms));
	symtab_offset = get(&file->symtab_cmd->symoff, sizeof(symtab_offset));
	ptr = (t_nlist *)((char*)file->header + symtab_offset);
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
	display_title();
	print_list(file, file->symboles);
}