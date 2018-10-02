/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sle-lieg <sle-lieg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/13 14:57:46 by sle-lieg          #+#    #+#             */
/*   Updated: 2018/10/02 10:46:01 by sle-lieg         ###   ########.fr       */
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
# include <mach-o/stab.h>
# include <mach-o/fat.h>
# include <mach-o/ranlib.h>
# include <ar.h>
# include "ft_printf.h"
# include "structures.h"

# define OPTIONS "gpruU"
# define O_GLO 0x1
# define O_UNS 0x2
# define O_REV 0x4
# define O_UND 0x8
# define O_DEF 0x10

# define O_LIB_A 0x20
# define O_MULT 0x80000000

# define X86_64 "x86_64"
# define I386 "i386"
# define X86 "x86"
# define POWERPC "ppc"
# define POWERPC64 "ppc64"
# define ARM "arm"
# define ARM64 "arm64"
# define HPPA "hppa"
# define I860 "i860"
# define SPARC "sparc"
# define VAX "vax"

extern int	g_flags;
extern int	g_little_endian;
extern char	*g_filename;
extern char	*g_file_end;
extern char *g_arch_name;

/*
**	================== COMMON SOURCES ==================
**	MAIN.C
*/
int				main(int ac, char **av);
void			nm(void *file_mmap);

/*
**	ARCHIVE.C
*/
void			parse_archive(void *f_start);

/*
**	FAT.C
*/
void			parse_fat(void *file_mmap);
void			get_arch_name(cpu_type_t cputype);

/*
**	FAT_64.C
*/
void			parse_fat_64(void *file_mmap);

/*
**	TOOLS.C
*/
void			insert_obj(t_static_obj *list, t_static_obj *new);
uint64_t		get(void *data, size_t size);
void			set_endianness(uint32_t magic);
void			check_limit(void *addr);
void			display_title(void);

/*
**	==================== NM-OTOOL SOURCES ====================
**	MACHO_32.c
*/
void			parse_32(char *ptr);

/*
**	MACHO_64.c
*/
void			parse_64(char *ptr);
void			print_infos(void);

/*
**	PRINT_SYMBOLES_32.c
*/
void			print_list(t_file *file, t_nlist_list *elem);

/*
**	PRINT_SYMBOLES_64.c
*/
void			print_list_64(t_file_64 *file, t_nlist64_list *elem);

/*
**	READ_SYMBOLES_32.c
*/
void			read_sym_table(t_file *file);

/*
**	READ_SYMBOLES_64.c
*/
void			read_sym_table_64(t_file_64 *file);
t_nlist64_list	*create_new_nlist_64(t_nlist_64 *nlist, char *str_table);

#endif
