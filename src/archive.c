/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   archive.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sle-lieg <sle-lieg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/27 16:45:34 by sle-lieg          #+#    #+#             */
/*   Updated: 2018/09/30 15:54:27 by sle-lieg         ###   ########.fr       */
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


void	insert_obj(t_static_obj *list, t_static_obj *new)
{
	while (list->next)
	{
		if (new->offset == list->offset)
		{
			free(new);
			return ;
		}
		if (new->offset < list->next->offset)
		{
			new->next = list->next;
			list->next = new;
			return ;
		}
		list = list->next;
	}
	if (new->offset == list->offset)
		free(new);
	else
		list->next = new;
}

t_static_obj *create_new_static_object(void *f_start, struct ranlib *obj)
{
	t_static_obj	*new;

	check_limit((void*)obj + sizeof(struct ranlib));
	check_limit((void*)f_start + obj->ran_off + sizeof(struct ar_hdr));
	if(!(new = (t_static_obj*)malloc(sizeof(*new))))
		exit(EXIT_FAILURE);
	new->offset = obj->ran_off;
	new->name_size = ft_atoi(f_start + obj->ran_off + 3);
	new->object = f_start + obj->ran_off + sizeof(struct ar_hdr) + new->name_size;
	new->next = NULL;
	return (new);
}

void	display_lib_static(t_static_obj *list)
{
	g_flags |= O_LIB_A;
	while (list)
	{
		// ft_printf("obj: %x\n", *(int*)list->object);
		ft_printf("\n%s(%s):\n", g_filename, (char*)list->object - list->name_size);
		nm(list->object);
		list = list->next;
	}
}

int	is_in_list(uint32_t offset, t_static_obj *list)
{
	while (list)
	{
		if (offset == list->offset)
			return (TRUE);
		list = list->next;
	}
	return (FALSE);
}

void	get_symboles(void	*f_start, uint32_t size, struct ranlib *obj)
{
	t_static_obj *list;
	t_static_obj *new;

	new = create_new_static_object(f_start, obj);
	list = new;
	size -= sizeof(struct ranlib);
	while (size)
	{
		++obj;
		size -= sizeof(struct ranlib);
		if (is_in_list(obj->ran_off, list))
			continue ;
		new = create_new_static_object(f_start, obj);
		if (new->offset < list->offset)
		{
			new->next = list;
			list = new;
		}
		else
			insert_obj(list, new);
	}
	display_lib_static(list);
}

void	parse_archive(void *f_start)
{
	struct ar_hdr *header;
	struct ranlib *ranlib;
	uint32_t size;

	check_limit(f_start + sizeof(struct ar_hdr));
	header = f_start + SARMAG;
	size = ft_atoi(header->ar_name + 3);
	ranlib = (void*)header + sizeof(struct ar_hdr) + size;
	get_symboles(f_start, *(uint32_t*)ranlib, (void*)ranlib + 4);
}