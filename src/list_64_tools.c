/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_64_tools.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sle-lieg <sle-lieg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/13 14:28:43 by sle-lieg          #+#    #+#             */
/*   Updated: 2018/09/20 20:16:26 by sle-lieg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

char *get_stab_type(uint8_t type)
{
	static char *stab_types[255];

	stab_types[N_GSYM] = "GSYM";
	stab_types[N_FNAME] = "FNAME";
	stab_types[N_FUN] = "FUN";
	stab_types[N_STSYM] = "STSYM";
	stab_types[N_LCSYM] = "LCSYM";
	stab_types[N_BNSYM] = "BNSYM";
	stab_types[N_AST] = "AST";
	stab_types[N_OPT] = "OPT";
	stab_types[N_RSYM] = "RSYM";
	stab_types[N_SLINE] = "SLINE";
	stab_types[N_ENSYM] = "ENSYM";
	stab_types[N_SSYM] = "SSYM";
	stab_types[N_SO] = "SO";
	stab_types[N_OSO] = "OSO";
	stab_types[N_LSYM] = "LSYM";
	stab_types[N_BINCL] = "BINCL";
	stab_types[N_SOL] = "SOL";
	stab_types[N_PARAMS] = "PARAMS";
	stab_types[N_VERSION] = "VERSION";
	stab_types[N_OLEVEL] = "OLEVEL";
	stab_types[N_PSYM] = "PSYM";
	stab_types[N_EINCL] = "EINCL";
	stab_types[N_ENTRY] = "ENTRY";
	stab_types[N_LBRAC] = "LBRAC";
	stab_types[N_EXCL] = "EXCL";
	stab_types[N_RBRAC] = "RBRAC";
	stab_types[N_BCOMM] = "BCOMM";
	stab_types[N_ECOMM] = "ECOMM";
	stab_types[N_ECOML] = "ECOML";
	stab_types[N_LENG] = "LENG";

	return (stab_types[type]);
}