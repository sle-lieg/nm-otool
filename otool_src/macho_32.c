/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   macho_32.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sle-lieg <sle-lieg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/01 10:39:21 by sle-lieg          #+#    #+#             */
/*   Updated: 2018/10/01 19:11:14 by sle-lieg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

static void	print_section(t_section *sect, char *ptr)
{
	uint8_t		*octet;
	uint32_t	addr;
	uint32_t	size;

	octet = (uint8_t *)ptr + get(&sect->offset, sizeof(sect->offset));
	check_limit(octet);
	addr = get(&sect->addr, sizeof(sect->addr));
	size = 0;
	ft_printf("%08x	", addr);
	addr += 16;
	while (size++ < get(&sect->size, sizeof(sect->size)))
	{
		ft_printf("%02x ", *octet);
		if (size % 16 == 0 && size < sect->size)
		{
			ft_printf("\n%08x	", addr);
			addr += 16;
		}
		++octet;
	}
}

static void	get_section_text(t_segment *seg_text, char *ptr)
{
	t_section	*sect;
	uint32_t	i;

	if (g_flags & O_LIB_A)
		ft_printf("Contents of (__TEXT,__text) section\n");
	else
	{
		ft_printf("%s", g_filename);
		if (g_arch_name)
			ft_printf("(%s):\nContents of (__TEXT,__text) section\n", \
				g_filename);
		else
			ft_printf(":\nContents of (__TEXT,__text) section\n");
	}
	sect = (t_section*)(seg_text + 1);
	i = 0;
	while (i < get(&seg_text->nsects, sizeof(seg_text->nsects)))
	{
		if (!ft_strcmp(sect[i].sectname, SECT_TEXT))
			print_section(&sect[i], ptr);
		++i;
	}
}

void		parse_32(char *ptr)
{
	t_mach_h	*header;
	t_load_cmd	*ldc;
	uint32_t	nb_cmds;
	uint32_t	i;

	header = (t_mach_h *)ptr;
	ldc = (t_load_cmd *)(header + 1);
	nb_cmds = get(&header->ncmds, sizeof(header->ncmds));
	i = 0;
	while (i++ < nb_cmds)
	{
		if (get(&ldc->cmd, sizeof(ldc->cmd)) == LC_SEGMENT)
		{
			if (!*((t_segment*)ldc)->segname \
				|| !ft_strcmp(((t_segment*)ldc)->segname, SEG_TEXT))
			{
				get_section_text((t_segment*)ldc, ptr);
				return ;
			}
		}
		ldc = (t_load_cmd *)((char*)ldc \
			+ get(&ldc->cmdsize, sizeof(ldc->cmdsize)));
	}
}
