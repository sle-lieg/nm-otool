/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   macho_32.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sle-lieg <sle-lieg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/01 10:39:21 by sle-lieg          #+#    #+#             */
/*   Updated: 2018/10/02 11:43:17 by sle-lieg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

static void	print_section(t_section *sect, char *ptr)
{
	uint8_t		*octet;
	uint32_t	addr;
	uint32_t	size;

	octet = (uint8_t *)ptr + get(&sect->offset, sizeof(sect->offset));
	check_limit(octet + get(&sect->size, sizeof(sect->size)));
	addr = get(&sect->addr, sizeof(sect->addr));
	size = 0;
	print_infos();
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
	t_segment	*seg;
	t_load_cmd	*ldc;
	uint32_t	nb_cmds;
	uint32_t	i;

	ldc = (t_load_cmd *)(ptr + sizeof(t_mach_h));
	nb_cmds = get(&((t_mach_h*)ptr)->ncmds, sizeof(uint32_t));
	i = 0;
	while (i++ < nb_cmds)
	{
		if (get(&ldc->cmd, sizeof(ldc->cmd)) == LC_SYMTAB)
			check_limit(ptr + get(&((t_symtab_cmd*)ldc)->stroff, \
	sizeof(uint32_t)) + get(&((t_symtab_cmd*)ldc)->strsize, sizeof(uint32_t)));
		if (get(&ldc->cmd, sizeof(ldc->cmd)) == LC_SEGMENT)
		{
			if (!*((t_segment*)ldc)->segname \
			|| !ft_strcmp(((t_segment*)ldc)->segname, SEG_TEXT))
				seg = (t_segment*)ldc;
		}
		ldc = (t_load_cmd *)((char*)ldc \
		+ get(&ldc->cmdsize, sizeof(ldc->cmdsize)));
	}
	get_section_text(seg, ptr);
}
