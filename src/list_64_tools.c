/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_64_tools.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sle-lieg <sle-lieg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/13 14:28:43 by sle-lieg          #+#    #+#             */
/*   Updated: 2018/09/17 16:42:42 by sle-lieg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

t__nlist_64_list	*new_nlist64(struct nlist_64 *nlist)
{
	t__nlist_64_list *new_elem;

	if (!(new_elem = (t__nlist_64_list*)malloc(sizeof(*new_elem))))
		exit(EXIT_FAILURE);
	new_elem->ptr = nlist;
	new_elem->next = NULL;
	return (new_elem);
}

// MAKE IT GENERIC OR COPY FOR EACH TYPE (nlist_32, nlist_64)
/*
** Insertion sort by alphabetical symbol order
** @param  **head:		start of the t__nlist_64_list that holds nlist_64 struct
** @param  *nlist:		struct nlist_64 to add to the list
** @param  *str_table:	string table
*/
// void	insert_nlist(t__nlist_64_list **head, struct nlist_64 *nlist, char *str_table)
// {
// 	t__nlist_64_list	*tmp;
// 	t__nlist_64_list	*new;

// 	tmp = *head;
// 	new = new_nlist(nlist);

// 	assert(nlist->n_un.n_strx != 0);

// 	if (ft_strcmp(str_table + (*head)->ptr->n_un.n_strx, str_table + nlist->n_un.n_strx) > 0)
// 	{
// 		*head = new;
// 		(*head)->next = tmp;
// 	}
// 	else
// 	{
// 		while (tmp->next &&
// 		(ft_strcmp(str_table + tmp->next->ptr->n_un.n_strx, str_table + nlist->n_un.n_strx) < 0))
// 		{
// 			tmp = tmp->next;
// 		}
// 		new->next = tmp->next;
// 		tmp->next = new;
// 	}
// }

void	insert_nlist64(t_file_map *file, struct nlist_64 *nlist)
{
	t__nlist_64_list	*tmp;
	t__nlist_64_list	*new;

	tmp = file->symboles;
	new = new_nlist64(nlist);

	assert(nlist->n_un.n_strx != 0);

	if (ft_strcmp(file->str_table + file->symboles->ptr->n_un.n_strx, file->str_table + nlist->n_un.n_strx) > 0)
	{
		file->symboles = new;
		file->symboles->next = tmp;
	}
	else
	{
		while (tmp->next &&
		(ft_strcmp(file->str_table + tmp->next->ptr->n_un.n_strx, file->str_table + nlist->n_un.n_strx) < 0))
			tmp = tmp->next;
		new->next = tmp->next;
		tmp->next = new;
	}
}

static char get_segment_type(t_file_map *file, struct nlist_64 *ptr)
{
	(void)file;
	(void)ptr;
	return ('T');
}

static void	print_type(t_file_map *file, struct nlist_64 *ptr)
{
	uint8_t	type;
	char		type_c;

	type = ptr->n_type & N_TYPE;
	type_c = ' ';
	if (type == N_UNDF)
		type_c = 'U';
	else if (type == N_ABS)
		type_c = 'A';
	else if (type == N_INDR)
		type_c = 'I';
	else if (type == N_SECT)
		type_c = get_segment_type(file, ptr);
	if (!(ptr->n_type & N_EXT))
		type_c += ('a' - 'A');
	ft_printf("%c ", type_c);
}

void	print_list(t_file_map *file)
{
	t__nlist_64_list *tmp;

	tmp = file->symboles;
	while (tmp)
	{
		if (tmp->ptr->n_value)
			ft_printf("%016lx ", tmp->ptr->n_value);
		else
			ft_printf("                 ", tmp->ptr->n_value);
		print_type(file, tmp->ptr);
		ft_printf("%s\n", file->str_table + tmp->ptr->n_un.n_strx);
		tmp = tmp->next;
	}
	ft_printf("\n");
}