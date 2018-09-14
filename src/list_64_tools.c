/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_64_tools.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sle-lieg <sle-lieg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/13 14:28:43 by sle-lieg          #+#    #+#             */
/*   Updated: 2018/09/14 12:04:39 by sle-lieg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

t_sym_list	*new_nlist(struct nlist_64 *nlist)
{
	t_sym_list *new_elem;

	if (!(new_elem = (t_sym_list*)malloc(sizeof(*new_elem))))
		exit(EXIT_FAILURE);
	new_elem->ptr = nlist;
	new_elem->next = NULL;
	return (new_elem);
}

// MAKE IT GENERIC OR COPY FOR EACH TYPE (nlist_32, nlist_64)
/*
** Insertion sort by alphabetical symbol order
** @param  **head:		start of the t_sym_list that holds nlist_64 struct
** @param  *nlist:		struct nlist_64 to add to the list
** @param  *str_table:	string table
*/
void	insert_nlist(t_sym_list **head, struct nlist_64 *nlist, char *str_table)
{
	t_sym_list	*tmp;
	t_sym_list	*new;

	tmp = *head;
	new = new_nlist(nlist);

	assert(nlist->n_un.n_strx != 0);

	if (ft_strcmp(str_table + (*head)->ptr->n_un.n_strx, str_table + nlist->n_un.n_strx) > 0)
	{
		*head = new;
		(*head)->next = tmp;
	}
	else
	{
		while (tmp->next &&
		(ft_strcmp(str_table + tmp->next->ptr->n_un.n_strx, str_table + nlist->n_un.n_strx) < 0))
		{
			tmp = tmp->next;
		}
		new->next = tmp->next;
		tmp->next = new;
	}
}

void	print_list(t_sym_list *tmp, char *str_table)
{
	while (tmp)
	{
		ft_printf("+	%s\n", str_table + tmp->ptr->n_un.n_strx);
		tmp = tmp->next;
	}
	ft_printf("\n");
}