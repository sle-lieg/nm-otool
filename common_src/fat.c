/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fat.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sle-lieg <sle-lieg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/25 13:17:21 by sle-lieg          #+#    #+#             */
/*   Updated: 2018/10/01 18:41:41 by sle-lieg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

void			get_arch_name(cpu_type_t cputype)
{
	if (cputype == CPU_TYPE_X86_64)
		g_arch_name = X86_64;
	else if (cputype == CPU_TYPE_I386)
		g_arch_name = I386;
	else if (cputype == CPU_TYPE_X86)
		g_arch_name = X86;
	else if (cputype == CPU_TYPE_POWERPC)
		g_arch_name = POWERPC;
	else if (cputype == CPU_TYPE_POWERPC64)
		g_arch_name = POWERPC64;
	else if (cputype == CPU_TYPE_ARM)
		g_arch_name = ARM;
	else if (cputype == CPU_TYPE_ARM64)
		g_arch_name = ARM64;
	else if (cputype == CPU_TYPE_HPPA)
		g_arch_name = HPPA;
	else if (cputype == CPU_TYPE_I860)
		g_arch_name = I860;
	else if (cputype == CPU_TYPE_SPARC)
		g_arch_name = SPARC;
	else if (cputype == CPU_TYPE_VAX)
		g_arch_name = VAX;
}

static uint32_t	get_x86_64_offset(t_fat_arch *arch, uint32_t nb_arch)
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

void			parse_fat(void *file_mmap)
{
	t_fat_arch	*arch;
	uint32_t	nb_arch;
	uint32_t	offset;
	size_t		i;

	i = 0;
	check_limit((char*)file_mmap + sizeof(t_fat_header));
	nb_arch = get(&((t_fat_header*)file_mmap)->nfat_arch, sizeof(uint32_t));
	arch = (void*)((char*)file_mmap + sizeof(t_fat_header));
	check_limit((char*)arch + sizeof(t_fat_arch) * nb_arch);
	if ((offset = get_x86_64_offset(arch, nb_arch)))
		nm((char*)file_mmap + offset);
	else
	{
		while (i < nb_arch)
		{
			g_little_endian = FALSE;
			arch = &((t_fat_arch *)((char*)file_mmap \
			+ sizeof(t_fat_header)))[i++];
			if (nb_arch > 1)
				get_arch_name(get(&arch->cputype, sizeof(arch->cputype)));
			offset = get(&arch->offset, sizeof(uint32_t));
			nm((char*)file_mmap + offset);
		}
	}
}
