/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mach_o_64.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sle-lieg <sle-lieg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/13 14:24:45 by sle-lieg          #+#    #+#             */
/*   Updated: 2018/09/14 12:03:40 by sle-lieg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

/**
 * @brief	read all symbols in the symbol_table and add create a t_sym_list *
 * @param  *ptr:			ptr to the symbol table
 * @param  *str_table:	ptr to the string table
 * @param  nb_sym:		number of symbols
 */
void	read_sym_table(struct nlist_64 *ptr, char *str_table, int nb_sym) // add strsize, the size of string table to check if the file is valid
{
	t_sym_list	*head;

	head = new_nlist(ptr);
	while (--nb_sym)
	{
		++ptr;
		insert_nlist(&head, ptr, str_table);
	}
	print_list(head, str_table);
}

/*
** @note		handle mach-o little endian 64 bits binary
** @param	*ptr: start of the file mapping
*/
void	handle_64(char *ptr)
{
	struct mach_header_64	*header;
	struct load_command		*ldc;
	struct symtab_command	*scmd;
	uint32_t						i;

	header = (struct mach_header_64 *)ptr;
	ldc = (struct load_command *)(header + 1);
	scmd = NULL;
	i = 0;

	// ft_printf("nb: %u\n", header->ncmds);
	while (i++ < header->ncmds)
	{
		// ft_printf("ldc[%p]: type=%u size=%u\n", ldc, ldc->cmd, ldc->cmdsize);
		if (ldc->cmd == LC_SYMTAB)
		{
			scmd = (struct symtab_command *)ldc;
			// ft_printf("FOUND %d %d %d\n", scmd->symoff, scmd->stroff, scmd->nsyms);
			read_sym_table((struct nlist_64 *)(ptr + scmd->symoff), ptr + scmd->stroff, scmd->nsyms);
			break ;
		}
		ldc = (struct load_command *)((char*)ldc + ldc->cmdsize);
	}
}
