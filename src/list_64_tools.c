/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_64_tools.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sle-lieg <sle-lieg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/13 14:28:43 by sle-lieg          #+#    #+#             */
/*   Updated: 2018/09/13 20:32:24 by sle-lieg         ###   ########.fr       */
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
** Insertion sort by increasing order n_strx(idx of the sym in the symbol table)
** if (head->idx > nlist->idx) : add it to the front
** else : move forward in the list until list->next->idx is > than nlist->idx,
** and then insert it between list and list->next
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

	ft_printf("to insert: %s\n", str_table + nlist->n_un.n_strx);
	if (ft_strcmp(str_table + (*head)->ptr->n_un.n_strx, str_table + nlist->n_un.n_strx) > 0)
	{
		ft_printf("	check head: %s\n",  str_table + (*head)->ptr->n_un.n_strx);
		*head = new;
		(*head)->next = tmp;
	}
	else
	{
		while (tmp->next && (ft_strcmp(str_table + tmp->next->ptr->n_un.n_strx, str_table + nlist->n_un.n_strx) < 0))
		{
			tmp = tmp->next;
		}
		new->next = tmp->next;
		tmp->next = new;
	}
	ft_printf("LIST:\n");
	print_list(*head, str_table);
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