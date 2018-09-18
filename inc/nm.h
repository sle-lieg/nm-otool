/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sle-lieg <sle-lieg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/13 14:57:46 by sle-lieg          #+#    #+#             */
/*   Updated: 2018/09/18 18:43:22 by sle-lieg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NM_H
# define NM_H

# include <sys/mman.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <errno.h>
# include <mach-o/loader.h>
# include <mach-o/nlist.h>
# include <mach-o/fat.h>
# include <stdlib.h>
# include <unistd.h>

# include "ft_printf.h"

#include <assert.h>

#define OPTIONS "rlap"
#define O_REV 0x1 // option -r

extern int flags;

enum			e_errors {
	ERR_FILE_NAME = -5,
	ERR_FSTAT,
	ERR_IS_DIR,
	ERR_MMAP,
	ERR_OPTION
};

/*
** @note	Linked list containing pointers to the nlist_64
**			data structures of the file symbols.
*/
typedef struct	s_nlist_64_list
{
	struct nlist_64	*ptr;
	struct s_nlist_64_list	*next;
}				t__nlist_64_list;

/*
** @note	Linked list containing pointers to the LC_SEGMENT_64
**			load_command type data structure. (__TEXT, __DATA, ...)
*/
typedef struct	s_segm_cmd_64_list
{
	struct segment_command_64 *ptr;
	struct s_segm_cmd_64_list *next;
}				t_seg_cmd_64_list;

/*
** @note	Contains different pointers to different part of a mach-o
**			file mapped in memory
*/
typedef struct	s_file_map
{
	// struct mach_header		*header;				// header of the file
	struct mach_header_64	*header;				// header of the file
	struct load_command		*ld_cmd;				// 1st load_command
	struct symtab_command	*sym_cmd;			// LC_SYMTAB load_command structure
															// hold symbole_table informations
	t__nlist_64_list			*symboles;			// 1st symbole entry
	t_seg_cmd_64_list			*segments;			// 1st LC_SEGMENT_64

	char							*str_table;		// start of the symboles


}				t_file_map;

/*
**	mach_0_64.c
*/
void			handle_64(char *ptr);
void			read_sym_table(t_file_map *file);

/*
**	list_64_tools.c
*/
t__nlist_64_list	*new_nlist64(struct nlist_64 *nlist);
void			insert_nlist64(t_file_map *file, struct nlist_64 *nlist);
void			print_list(t_file_map *file);

#endif
