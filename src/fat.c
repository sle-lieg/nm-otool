/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fat.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sle-lieg <sle-lieg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/25 13:17:21 by sle-lieg          #+#    #+#             */
/*   Updated: 2018/09/25 18:05:07 by sle-lieg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

// uint64_t get(void *data, size_t size)
// {
// 	uint64_t ret;
// 	uint8_t	*ptr;

// 	ret = 0;
// 	ptr = (uint8_t *)data;
// 	while (size--)
// 		ret = (ret << 8) + *ptr++;
// 	return (ret);
// }
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

void	parse_FAT(void *file_mmap)
{
	t_fat_arch *arch;
	uint32_t	nb_arch;
	uint32_t	offset;
	size_t i;

	i = 0;
	nb_arch = get(&((struct fat_header*)file_mmap)->nfat_arch, sizeof(uint32_t));
	while (i < nb_arch)
	{
		arch = &((struct fat_arch *)((char*)file_mmap + sizeof(struct fat_header)))[i++];
		offset = get(&arch->offset, sizeof(uint32_t));
		nm((char*)file_mmap + offset);
	}
}
