/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   macho_64.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sle-lieg <sle-lieg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/01 10:39:24 by sle-lieg          #+#    #+#             */
/*   Updated: 2018/10/02 16:38:39 by sle-lieg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

void		print_infos(void)
{
	if (g_flags & O_LIB_A)
		ft_printf("Contents of (__TEXT,__text) section\n");
	else
	{
		ft_printf("\n%s", g_filename);
		if (g_arch_name)
			ft_printf("(architecture %s):\nContents of (__TEXT,__text) section\
\n", g_arch_name);
		else
			ft_printf(":\nContents of (__TEXT,__text) section\n");
	}
}

static void	print_section(t_section_64 *sect, char *ptr)
{
	uint8_t		*octet;
	uint64_t	addr;
	uint64_t	size;

	octet = (uint8_t *)ptr + get(&sect->offset, sizeof(sect->offset));
	check_limit(octet + get(&sect->size, sizeof(sect->size)));
	addr = get(&sect->addr, sizeof(sect->addr));
	size = 0;
	print_infos();
	ft_printf("%016lx	", addr);
	addr += 16;
	while (size++ < get(&sect->size, sizeof(sect->size)))
	{
		ft_printf("%02x ", *octet);
		if (size % 16 == 0 && size < sect->size)
		{
			ft_printf("\n%016lx	", addr);
			addr += 16;
		}
		++octet;
	}
}

static void	get_section_text(t_segment_64 *seg_text, char *ptr)
{
	t_section_64	*sect;
	uint32_t		i;

	sect = (t_section_64*)(seg_text + 1);
	i = 0;
	while (i < get(&seg_text->nsects, sizeof(seg_text->nsects)))
	{
		if (!ft_strcmp(sect[i].sectname, SECT_TEXT))
			print_section(&sect[i], ptr);
		++i;
	}
}

void		parse_64(char *ptr)
{
	t_segment_64	*seg;
	t_load_cmd		*ldc;
	uint32_t		nb_cmds;
	uint32_t		i;

	ldc = (t_load_cmd *)(ptr + sizeof(t_mach_h_64));
	nb_cmds = get(&((t_mach_h_64*)ptr)->ncmds, sizeof(uint32_t));
	i = 0;
	while (i++ < nb_cmds)
	{
		if (get(&ldc->cmd, sizeof(ldc->cmd)) == LC_SYMTAB)
			check_limit(ptr + get(&((t_symtab_cmd*)ldc)->stroff, \
	sizeof(uint32_t)) + get(&((t_symtab_cmd*)ldc)->strsize, sizeof(uint32_t)));
		if (get(&ldc->cmd, sizeof(ldc->cmd)) == LC_SEGMENT_64)
		{
			if (!*((t_segment_64*)ldc)->segname \
			|| !ft_strcmp(((t_segment_64*)ldc)->segname, SEG_TEXT))
				seg = (t_segment_64*)ldc;
		}
		ldc = (t_load_cmd *)((char*)ldc \
		+ get(&ldc->cmdsize, sizeof(ldc->cmdsize)));
	}
	get_section_text(seg, ptr);
}
