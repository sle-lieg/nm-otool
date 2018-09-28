/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   archive.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sle-lieg <sle-lieg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/27 16:45:34 by sle-lieg          #+#    #+#             */
/*   Updated: 2018/09/28 20:33:13 by sle-lieg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

void display_header(struct ar_hdr *header)
{
	ft_printf("ar_name: %s\n", header->ar_name);
	ft_printf("ar_date: %s\n", header->ar_date);
	ft_printf("ar_uid: %s\n", header->ar_uid);
	ft_printf("ar_gid: %s\n", header->ar_gid);
	ft_printf("ar_mode: %s\n", header->ar_mode);
	ft_printf("ar_size: %s\n", header->ar_size);
	ft_printf("%s\n", header + 1);
}

void	get_symboles(uint32_t size, struct ranlib *obj)
{
	ft_printf("size = %u\n", size);
	while (size)
	{
		ft_printf("index str_table: %x\n", obj->ran_un);
		ft_printf("index obj: %x\n", obj->ran_off);
		size -= 4;
		obj++;
	}
}

void	parse_archive(void *file_mmap)
{
	struct ar_hdr *header;
	struct ranlib *ranlib;
	uint32_t size;

	header = file_mmap + SARMAG;
	size = ft_atoi(header->ar_name + 3);
	ranlib = (void*)header + sizeof(struct ar_hdr) + size;
	get_symboles(*(uint32_t*)ranlib, (void*)ranlib + 4);
	// ft_printf("index str_table: %x\n", ranlib->ran_un);
	// ft_printf("index object: %x\n", ranlib->ran_off);
	// ft_printf("size: %d\n", size);
	display_header(header);
}