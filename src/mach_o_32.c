/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mach_o_32.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sle-lieg <sle-lieg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/18 14:59:05 by sle-lieg          #+#    #+#             */
/*   Updated: 2018/09/18 15:03:19 by sle-lieg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

void	handle_32(char *ptr)
{
	t_file_map					file;
	struct load_command		*ldc;
	uint32_t						i;

	file.header = (struct mach_header *)ptr;
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
}