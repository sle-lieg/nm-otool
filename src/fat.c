/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fat.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sle-lieg <sle-lieg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/25 13:17:21 by sle-lieg          #+#    #+#             */
/*   Updated: 2018/09/26 16:48:30 by sle-lieg         ###   ########.fr       */
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

// void	parse_FAT(void *file_mmap)
// {
// 	t_fat_arch *arch;
// 	uint32_t	nb_arch;
// 	uint32_t	offset;
// 	size_t i;

// 	i = 0;
// 	nb_arch = get(&((t_fat_header*)file_mmap)->nfat_arch, sizeof(uint32_t));
// 	while (i < nb_arch)
// 	{
// 		arch = &((t_fat_arch *)((char*)file_mmap + sizeof(t_fat_header)))[i++];
// 		offset = get(&arch->offset, sizeof(uint32_t));
// 		if (arch->cputype == CPU_TYPE_X86_64)
// 		{
// 			nm((char*)file_mmap + offset);
// 			return ;
// 		}
// 	}
// 	i = 0;
// 	while (i < nb_arch)
// 	{
// 		g_little_endian = FALSE;
// 		arch = &((t_fat_arch *)((char*)file_mmap + sizeof(t_fat_header)))[i++];
// 		offset = get(&arch->offset, sizeof(uint32_t));
// 		print_archi(arch->cputype);
// 		nm((char*)file_mmap + offset);
// 	}
// }

void	parse_FAT(void *file_mmap)
{
	t_fat_arch *arch;
	uint32_t	nb_arch;
	uint32_t	offset;
	size_t i;

	nb_arch = get(&((t_fat_header*)file_mmap)->nfat_arch, sizeof(uint32_t));
	if ((i = get_x86_64_arch_index()) >= 0)
	{
		arch = &((t_fat_arch *)((char*)file_mmap + sizeof(t_fat_header)))[i];
		offset = get(&arch->offset, sizeof(uint32_t));
		nm((char*)file_mmap + offset);
	}
	else
	{
		while (++i < nb_arch)
		{
			g_little_endian = FALSE;
			arch = &((t_fat_arch *)((char*)file_mmap + sizeof(t_fat_header)))[i];
			offset = get(&arch->offset, sizeof(uint32_t));
			print_archi(arch->cputype);
			nm((char*)file_mmap + offset);
		}		
	}
}