/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fat.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sle-lieg <sle-lieg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/25 13:17:21 by sle-lieg          #+#    #+#             */
/*   Updated: 2018/09/27 18:30:53 by sle-lieg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

uint64_t get(void *data, size_t size)
{
	uint64_t ret;
	uint8_t	*ptr;

	ret = 0;
	if (g_little_endian)
	{
		ptr = (uint8_t *)data + size;
		while (size--)
			ret = (ret << 8) + *--ptr;
	}
	else
	{
		ptr = (uint8_t *)data;
		while (size--)
			ret = (ret << 8) + *ptr++;
	}
	return (ret);
}

static uint32_t get_x86_64_offset(t_fat_arch *arch, uint32_t nb_arch)
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

void	get_arch_name(cpu_type_t cputype)
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

void	parse_FAT(void *file_mmap)
{
	t_fat_arch *arch;
	uint32_t	nb_arch;
	uint32_t	offset;
	size_t i;

	i = 0;
	nb_arch = get(&((t_fat_header*)file_mmap)->nfat_arch, sizeof(uint32_t));
	if ((offset = get_x86_64_offset((t_fat_arch *)((char*)file_mmap + sizeof(t_fat_header)), nb_arch)))
		nm((char*)file_mmap + offset);
	else
	{
		while (i < nb_arch)
		{
			g_little_endian = FALSE;
			arch = &((t_fat_arch *)((char*)file_mmap + sizeof(t_fat_header)))[i++];
			get_arch_name(get(&arch->cputype, sizeof(arch->cputype)));
			offset = get(&arch->offset, sizeof(uint32_t));
			nm((char*)file_mmap + offset);
		}		
	}
}
