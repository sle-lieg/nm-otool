/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sle-lieg <sle-lieg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/13 14:57:46 by sle-lieg          #+#    #+#             */
/*   Updated: 2018/09/14 16:00:21 by sle-lieg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NM_H
# define NM_H

# include <sys/mman.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <mach-o/loader.h>
# include <mach-o/nlist.h>
# include <mach-o/fat.h>
# include <stdlib.h>
# include <unistd.h>

# include "ft_printf.h"

#include <assert.h>

#define O_REV 0x1 // option -r

extern int flags;

enum			e_errors {
	ERR_FILE_NAME = -4,
	ERR_FSTAT,
	ERR_IS_DIR,
	ERR_MMAP
};

typedef struct	s_sym_list_64
{
	struct nlist_64	*ptr;
	struct s_sym_list_64	*next;
}				t_sym_list;

/*
**	mach_0_64.c
*/
void			handle_64(char *ptr);
void			read_sym_table(struct nlist_64 *ptr, char *str_table, int nb_sym);

/*
**	list_64_tools.c
*/
t_sym_list	*new_nlist(struct nlist_64 *nlist);
void			insert_nlist(t_sym_list **head, struct nlist_64 *nlist, char *str_table);
void			print_list(t_sym_list *tmp, char *str_table);

#endif
