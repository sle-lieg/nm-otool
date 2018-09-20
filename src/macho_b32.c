/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   macho_b32.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sle-lieg <sle-lieg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/20 07:04:06 by sle-lieg          #+#    #+#             */
/*   Updated: 2018/09/20 07:08:37 by sle-lieg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

/*
** @note		handle mach-o big endian 32 bits binary
** @param	*ptr: start of the file mapping
*/
void	parse_l64(char *ptr)
{
	ft_printf("%x: Little endian 64 bits\n", *(uint32_t*)ptr);
}