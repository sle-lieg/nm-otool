/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   macho_b32.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sle-lieg <sle-lieg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/20 07:04:06 by sle-lieg          #+#    #+#             */
/*   Updated: 2018/09/20 11:30:25 by sle-lieg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

/*
** @note		handle mach-o big endian 32 bits binary
** @param	*ptr: start of the file mapping
*/
void	parse_b32(char *ptr)
{
	ft_printf("%x: Big endian 32 bits\n", *(uint32_t*)ptr);
}