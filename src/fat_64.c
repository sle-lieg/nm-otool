/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fat_64.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sle-lieg <sle-lieg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/27 17:24:26 by sle-lieg          #+#    #+#             */
/*   Updated: 2018/09/27 17:48:38 by sle-lieg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

static uint64_t get_x86_64_offset(t_fat_arch_64 *arch, uint32_t nb_arch)
{
	uint32_t offset;

	while (nb_arch--)
	{
		if (get(&arch->cputype, sizeof(arch->cputype)) == CPU_TYPE_X86_64)
		{
			offset = get(&arch->offset, sizeof(arch->offset));
			return (offset);
		}
		arch++;
	}
	return (0);
}

void	parse_FAT_64(void *file_mmap)
{
	t_fat_arch_64 *arch;
	uint32_t	nb_arch;
	uint64_t	offset;
	size_t i;

	i = 0;
	nb_arch = get(&((t_fat_header*)file_mmap)->nfat_arch, sizeof(uint32_t));
	if ((offset = get_x86_64_offset((t_fat_arch_64 *)((char*)file_mmap + sizeof(t_fat_header)), nb_arch)))
		nm((char*)file_mmap + offset);
	else
	{
		while (i < nb_arch)
		{
			g_little_endian = FALSE;
			arch = &((t_fat_arch_64 *)((char*)file_mmap + sizeof(t_fat_header)))[i++];
			offset = get(&arch->offset, sizeof(uint32_t));
			nm((char*)file_mmap + offset);
		}
	}
}
