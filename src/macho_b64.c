/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   macho_b64.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sle-lieg <sle-lieg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/20 07:03:17 by sle-lieg          #+#    #+#             */
/*   Updated: 2018/09/20 07:08:20 by sle-lieg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

/*
** @note		handle mach-o big endian 64 bits binary
** @param	*ptr: start of the file mapping
*/
void	parse_b64(char *ptr)
{
	ft_printf("%x: Big endian 64 bits\n", *(uint32_t*)ptr);
}