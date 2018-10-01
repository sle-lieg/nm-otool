/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structures.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sle-lieg <sle-lieg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/01 19:14:48 by sle-lieg          #+#    #+#             */
/*   Updated: 2018/10/01 19:24:09 by sle-lieg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTURES_H
# define STRUCTURES_H

# include <stdlib.h>

typedef struct fat_header			t_fat_header;
typedef struct fat_arch				t_fat_arch;
typedef struct fat_arch_64			t_fat_arch_64;
typedef struct load_command			t_load_cmd;
typedef struct mach_header			t_mach_h;
typedef struct mach_header_64		t_mach_h_64;
typedef struct nlist				t_nlist;
typedef struct nlist_64				t_nlist_64;
typedef struct section				t_section;
typedef struct section_64			t_section_64;
typedef struct segment_command		t_segment;
typedef struct segment_command_64	t_segment_64;
typedef struct symtab_command		t_symtab_cmd;
typedef struct ar_hdr				t_ar_hdr;

enum	e_errors {
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
	char				*name;
	t_nlist				*symbole;
	struct s_nlist_list	*next;
}				t_nlist_list;

typedef struct	s_nlist64_list
{
	char					*name;
	t_nlist_64				*symbole;
	struct s_nlist64_list	*next;
}				t_nlist64_list;

/*
** @note	Linked list of ptr to struct section (t_section[_64] *)
*/
typedef struct	s_section_list
{
	t_section				*sect;
	struct s_section_list	*next;
}				t_section_list;

typedef struct	s_section64_list
{
	t_section_64			*sect;
	struct s_section64_list	*next;
}				t_section64_list;

/*
** @note	Linked list of ptr to struct segment_command (t_segment[_64] *) and
**			a ptr to the linked list of the sections of the segment
*/
typedef struct	s_segment_list
{
	t_segment				*seg;
	t_section_list			*sections;
	struct s_segment_list	*next;
}				t_segment_list;

typedef struct	s_segment64_list
{
	t_segment_64			*seg;
	t_section64_list		*sections;
	struct s_segment64_list *next;
}				t_segment64_list;

typedef struct	s_static_obj
{
	uint32_t			offset;
	uint32_t			name_size;
	void				*object;
	struct s_static_obj	*next;
}				t_static_obj;

/*
** @note	Contains different pointers to different part of a mach-o
**			file mapped in memory
*/
typedef struct	s_file
{
	t_mach_h		*header;
	t_nlist_list	*symboles;
	t_segment_list	*segments;
	t_load_cmd		*ld_cmd;
	t_symtab_cmd	*symtab_cmd;
	char			*str_table;
}				t_file;

typedef struct	s_file_64
{
	t_mach_h_64			*header;
	t_nlist64_list		*symboles;
	t_segment64_list	*segments;
	t_load_cmd			*ld_cmd;
	t_symtab_cmd		*symtab_cmd;
	char				*str_table;
}				t_file_64;

#endif
