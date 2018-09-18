/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mach_o_64.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sle-lieg <sle-lieg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/13 14:24:45 by sle-lieg          #+#    #+#             */
/*   Updated: 2018/09/18 14:58:48 by sle-lieg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

/**
 * @brief	read all symbols in the symbol_table and create a t_sym_list *
 * @param  *file:	ptr to the file structure
 */
void	read_sym_table(t_file_map *file) // add strsize, the size of string table to check if the file is valid
{
	struct nlist_64 *ptr;
	int nb_sym;

	nb_sym = file->sym_cmd->nsyms;
	ptr = (struct nlist_64 *)((char*)file->header + file->sym_cmd->symoff);
	file->symboles = new_nlist64(ptr);
	while (--nb_sym)
	{
		++ptr;
		insert_nlist64(file, ptr);
	}
	print_list(file);
}

/*
** @note		handle mach-o little endian 64 bits binary
** @param	*ptr: start of the file mapping
*/
void	handle_64(char *ptr)
{
	t_file_map					file;
	struct load_command		*ldc;
	uint32_t						i;

	file.header = (struct mach_header_64 *)ptr;
	file.ld_cmd = (struct load_command *)(file.header + 1);
	ldc = file.ld_cmd;
	i = 0;
	while (i++ < file.header->ncmds)
	{
		// ft_printf("ldc[%p]: type=%u size=%u\n", ldc, ldc->cmd, ldc->cmdsize);
		// if (ldc->cmd == LC_SEGMENT_64)
		// 	add_segment_cmd_64(file, ldc);
		if (ldc->cmd == LC_SYMTAB)
			file.sym_cmd = (struct symtab_command *)ldc;
		ldc = (struct load_command *)((char*)ldc + ldc->cmdsize);
	}
	file.str_table = ptr + file.sym_cmd->stroff;
	read_sym_table(&file);
	// read_sym_table((struct nlist_64 *)(ptr + scmd->symoff), ptr + scmd->stroff, scmd->nsyms);
}