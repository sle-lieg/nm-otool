/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_64_tools.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sle-lieg <sle-lieg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/13 14:28:43 by sle-lieg          #+#    #+#             */
/*   Updated: 2018/09/19 17:18:04 by sle-lieg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

t_nlist_list	*new_nlist(t_nlist64 *nlist)
{
	t_nlist_list *new_elem;

	if (!(new_elem = (t_nlist_list*)malloc(sizeof(*new_elem))))
		exit(EXIT_FAILURE);
	new_elem->ptr = nlist;
	new_elem->next = NULL;
	return (new_elem);
}

void	insert_nlist(t_file_map *file, t_nlist64 *nlist)
{
	t_nlist_list	*tmp;
	t_nlist_list	*new;

	tmp = file->symboles;
	new = new_nlist(nlist);

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
	struct load_command	*tmp;
	struct section_64 *target;
	int tot_sects;
	int nb_cmds;

	tmp = file->ld_cmd;
	tot_sects = 0;
	nb_cmds = file->header->ncmds;
	while (nb_cmds--)
	{
		if (tmp->cmd == LC_SEGMENT_64)
		{
			if (tot_sects + ((struct segment_command_64 *)tmp)->nsects >= ptr->n_sect)
			{
				// ft_printf("\nLD_CMD:		%p type= %s\n", tmp, ((struct segment_command_64 *)tmp)->segname);
				// target = (struct section_64 *)(((struct segment_command_64 *)tmp)+1);
				// ft_printf("SECTION_64:	%p\n", target);
				target = &((struct section_64 *)((struct segment_command_64 *)tmp+1))[ptr->n_sect - tot_sects - 1];
				// ft_printf("TARGET: SECTNAME=%s	SEGNAME=%s\n", target->sectname, target->segname);
				// if (!ft_strncmp(target->sectname, SECT_TEXT, ft_strlen(SECT_TEXT)))
				if (!ft_strcmp(target->sectname, SECT_TEXT))
					return 'T';
				if (!ft_strcmp(target->sectname, SECT_DATA))
					return 'D';
				if (!ft_strcmp(target->sectname, SECT_BSS))
					return 'B';
				if (!ft_strcmp(target->sectname, SECT_COMMON))
					return 'C';
				return 'S';
			}
			tot_sects += ((struct segment_command_64 *)tmp)->nsects;
		}
		tmp = (void*)((char *)tmp + tmp->cmdsize);
	}
	return (' ');
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
		type_c |= 0x20;
	ft_printf("%c ", type_c);
}

void	print_list(t_file_map *file)
{
	t_nlist_64_list *tmp;

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