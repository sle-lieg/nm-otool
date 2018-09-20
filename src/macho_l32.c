/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   macho_l32.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sle-lieg <sle-lieg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/20 07:03:41 by sle-lieg          #+#    #+#             */
/*   Updated: 2018/09/20 07:08:03 by sle-lieg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

/*
** @note		handle mach-o little endian 32 bits binary
** @param	*ptr: start of the file mapping
*/
void	parse_l32(char *ptr)
{
	ft_printf("%x: Little endian 32 bits\n", *(uint32_t*)ptr);
}