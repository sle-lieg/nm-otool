/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_symboles_64.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sle-lieg <sle-lieg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/25 18:35:28 by sle-lieg          #+#    #+#             */
/*   Updated: 2018/10/01 19:09:25 by sle-lieg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

t_nlist64_list			*create_new_nlist_64(t_nlist_64 *nlist, char *str_table)
{
	t_nlist64_list	*new;
	char			*name;

	check_limit((char*)nlist + sizeof(t_nlist));
	name = str_table + get(&nlist->n_un.n_strx, sizeof(nlist->n_un.n_strx));
	check_limit(name);
	if (!(new = (t_nlist64_list*)malloc(sizeof(*new))))
		exit(EXIT_FAILURE);
	new->name = name;
	new->symbole = nlist;
	new->next = NULL;
	return (new);
}

static t_nlist64_list	*find_insert(t_nlist64_list *tmp, char *new_sym)
{
	while (tmp->next)
	{
		if (ft_strcmp(tmp->next->name, new_sym) > 0)
			return (tmp);
		tmp = tmp->next;
	}
	return (tmp);
}

static void				insert_nlist(t_file_64 *file, t_nlist_64 *nlist)
{
	t_nlist64_list *tmp;
	t_nlist64_list *new;

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

void					read_sym_table_64(t_file_64 *file)
{
	t_nlist_64		*ptr;
	t_nlist64_list	*last;
	uint32_t		nb_sym;
	uint32_t		symtab_offset;

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
