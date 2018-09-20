/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sle-lieg <sle-lieg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/13 14:57:46 by sle-lieg          #+#    #+#             */
/*   Updated: 2018/09/20 07:48:28 by sle-lieg         ###   ########.fr       */
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

#define OPTIONS "anoPruU"
#define L_ENDIAN	0x1
#define O_REV 0x1 // option -r

extern int flags;

typedef struct fat_arch t_fat_arch;
typedef struct load_command t_load_cmd;
typedef struct mach_header t_mach_h;
typedef struct mach_header_64 t_mach_h_64;
typedef struct nlist t_nlist;
typedef struct nlist_64 t_nlist_64;
typedef struct section t_section;
typedef struct section_64 t_section_64;
typedef struct segment_command t_segment;
typedef struct segment_command_64 t_segment_64;
typedef struct symtab_command t_symtab_cmd;

enum			e_errors {
	ERR_FILE_NAME = -5,
	ERR_FSTAT,
	ERR_IS_DIR,
	ERR_MMAP,
	ERR_OPTION
};

/*
** @note	Linked list of ptr to struct nlist (t_nlist[_64] *)
*/
typedef struct	s_nlist_list
{
	t_nlist					*ptr;
	struct s_nlist_list	*next;
}				t_nlist_list;

typedef struct	s_nlist64_list
{
	t_nlist_64					*ptr;
	struct s_nlist64_list	*next;
}				t_nlist64_list;

/*
** @note	Linked list of ptr to struct section (t_section[_64] *)
*/
typedef struct	s_section_list
{
	t_section					*ptr;
	struct s_section_list	*next;
}				t_section_list;

typedef struct	s_section64_list
{
	t_section_64					*ptr;
	struct s_section64_list *next;
}				t_section64_list;

/*
** @note	Linked list of ptr to struct segment_command (t_segment[_64] *) and
**			a ptr to the linked list of the sections of the segment
*/
typedef struct	s_segment_list
{
	t_segment					*ptr;
	t_section_list				sections;
	struct s_segment_list	*next;
}				t_segment_list;

typedef struct	s_segment64_list
{
	t_segment_64				*ptr;
	t_section64_list			*sections;
	struct s_segment64_list *next;
}				t_segment64_list;

/*
** @note	Contains different pointers to different part of a mach-o
**			file mapped in memory
*/
typedef struct	s_file
{
	t_mach_h			*header;				// header of the file
	t_nlist_list	*symboles;			// 1st symbole entry
	t_segment_list	*segments;			// 1st LC_SEGMENT_64
	t_load_cmd		*ld_cmd;				// 1st load_command
	t_sym_cmd		*symtab_cmd;			// LC_SYMTAB load_command structure
												// hold symbole_table informations
	char	*str_table;		// start of the symboles
}				t_file;

typedef struct	s_file_64
{
	t_mach_h_64			*header;				// header of the file
	t_nlist64_list		*symboles;			// 1st symbole entry
	t_segment64_list	*segments;			// 1st LC_SEGMENT_64
	t_load_cmd			*ld_cmd;				// 1st load_command
	t_sym_cmd			*symtab_cmd;			// LC_SYMTAB load_command structure
												// hold symbole_table informations
}				t_file_64;



uint64_t bendtolend(void *data, size_t size);

/*
**	macho_l64.c
*/
void			parse_l64(char *ptr);
void			read_sym_table(t_file_map *file);

/*
**	macho_b64.c
*/
void			parse_b64(char *ptr);

/*
**	macho_l32.c
*/
void			parse_l32(char *ptr);

/*
**	macho_b32.c
*/
void			parse_b32(char *ptr);

/*
**	list_64_tools.c
*/
t_nlist_list	*new_nlist(t_nlist64 *nlist);
void			insert_nlist(t_file_map *file, struct nlist_64 *nlist);
void			print_list(t_file_map *file);

#endif
