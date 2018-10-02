/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   archive.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sle-lieg <sle-lieg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/27 16:45:34 by sle-lieg          #+#    #+#             */
/*   Updated: 2018/10/02 14:45:19 by sle-lieg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

static t_static_obj	*create_new_static_object(void *f_start, struct ranlib *obj)
{
	t_static_obj *new;

	check_limit((void*)obj + sizeof(struct ranlib));
	check_limit((void*)f_start + obj->ran_off + sizeof(t_ar_hdr));
	if (!(new = (t_static_obj*)malloc(sizeof(*new))))
		exit(EXIT_FAILURE);
	new->offset = obj->ran_off;
	new->name_size = ft_atoi(f_start + obj->ran_off + 3);
	new->object = f_start + obj->ran_off + sizeof(t_ar_hdr) + new->name_size;
	new->next = NULL;
	return (new);
}

static void			display_lib_static(t_static_obj *list)
{
	g_flags |= O_LIB_A;
	while (list)
	{
		ft_printf("\n%s(%s):\n", g_filename, \
		(char*)list->object - list->name_size);
		nm(list->object);
		list = list->next;
	}
}

static int			is_in_list(uint32_t offset, t_static_obj *list)
{
	while (list)
	{
		if (offset == list->offset)
			return (TRUE);
		list = list->next;
	}
	return (FALSE);
}

static void			get_sym(void *f_start, uint32_t size, struct ranlib *obj)
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

void				parse_archive(void *f_start)
{
	struct ranlib	*ranlib;
	t_ar_hdr		*header;
	uint32_t		size;

	check_limit(f_start + sizeof(struct ar_hdr));
	header = f_start + SARMAG;
	size = ft_atoi(header->ar_name + 3);
	ranlib = (void*)header + sizeof(struct ar_hdr) + size;
	get_sym(f_start, *(uint32_t*)ranlib, (void*)ranlib + 4);
}
