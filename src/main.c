/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sle-lieg <sle-lieg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/13 14:24:50 by sle-lieg          #+#    #+#             */
/*   Updated: 2018/09/26 16:10:14 by sle-lieg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

int	g_flags;
int	g_little_endian;
char	*g_filename;

void	nm(void *file_mmap)
{
	uint32_t magic_number;

	g_little_endian = FALSE;
	magic_number = *(int *)file_mmap;
	if (!ft_strncmp((char *)file_mmap, "!<arch>\n", 8))
		parse_archive(file_mmap);
	else if (magic_number == FAT_CIGAM || magic_number == FAT_CIGAM_64)
		parse_FAT(file_mmap);
	else
	{
		if (!(g_flags & O_FIL) && (g_flags & O_MULT))
			ft_printf("%s:\n", g_filename);
		if (magic_number == MH_MAGIC_64 || magic_number == MH_CIGAM_64)
		{
			g_little_endian = (magic_number == MH_MAGIC_64 ? TRUE : FALSE);
			parse_64(file_mmap);
		}
		else if (magic_number == MH_MAGIC || magic_number == MH_CIGAM)
		{
			g_little_endian = (magic_number == MH_MAGIC ? TRUE : FALSE);
			parse_32(file_mmap);
		}
	}
	// ft_printf("magic_number = %x\n", magic_number);
}

// TODO: add tab to funtion ptr for parsers
// void	nm(void *file_mmap)
// {
// 	uint32_t magic_number;

// 	g_little_endian = FALSE;
// 	magic_number = *(int *)file_mmap;
// 	// ft_printf("magic_number = %x\n", magic_number);
// 	// if (!ft_strncmp((char *)file_mmap, "!<arch>\n", 8))
// 	// 	parse_archive(file_mmap);
// 	if (magic_number == MH_MAGIC_64 || magic_number == MH_CIGAM_64)
// 	{
// 		// if (!(g_flags & O_FIL) && (g_flags & O_MULT))
// 		// 	ft_printf("%s:\n", g_filename);
// 		g_little_endian = (magic_number == MH_MAGIC_64 ? TRUE : FALSE);
// 		parse_64(file_mmap);
// 	}
// 	else if (magic_number == MH_MAGIC || magic_number == MH_CIGAM)
// 	{
// 		// if (!(g_flags & O_FIL) && (g_flags & O_MULT))
// 		// 	ft_printf("%s:\n", g_filename);
// 		g_little_endian = (magic_number == MH_MAGIC ? TRUE : FALSE);
// 		parse_32(file_mmap);
// 	}
// 	else if (magic_number == FAT_CIGAM || magic_number == FAT_CIGAM_64)
// 		parse_FAT(file_mmap);
// }

static int	handle_error(int code_error, char *file)
{
	char bin_path[255];

	getcwd(bin_path, 255);
	if (code_error == ERR_FILE_NAME)
	{
		if (errno == EACCES)
			ft_printf("%s/nm: %s: Permission denied.\n", bin_path, file);
		else
			ft_printf("%s/nm: %s: No such file or directory.\n", bin_path, file);
	}
	else if (code_error == ERR_FSTAT)
		ft_printf("%s/nm: %s: An error occured with fstat.\n", bin_path);
	else if (code_error == ERR_IS_DIR)
		ft_printf("%s/nm: %s: Is a directory.\n", bin_path, file);
	else if (code_error == ERR_MMAP)
		ft_printf("%s/nm: %s: MMAP error.\n", bin_path, file);
	return (code_error);
}

static int	handle_file(char *file_name)
{
	struct stat	buf;
	int			fd;
	void			*ptr;

	g_filename = file_name;
	if ((fd = open(file_name, O_RDONLY)) < 0)
		return (handle_error(ERR_FILE_NAME, file_name));
	if (fstat(fd, &buf) < 0)
		return (handle_error(ERR_FSTAT, file_name));
	if ((buf.st_mode & S_IFMT) == S_IFDIR)
		return (handle_error(ERR_IS_DIR, file_name));
	if ((ptr = mmap(NULL, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) \
		== MAP_FAILED)
		return (handle_error(ERR_MMAP, file_name));
	nm(ptr);
	munmap(ptr, buf.st_size);
	close(fd);
	return (0);
}

static void	get_option(char *option)
{
	static char	*match_flag = OPTIONS;
	char			*tmp;
	int			i;

	i = 0;
	tmp = option;
	while (*tmp == '-')
		tmp++;
	while (*tmp)
	{
		if ((i = ft_strichr(match_flag, *tmp++)) >= 0)
			g_flags |= (1 << i);
		else
		{
			ft_printf("nm: Unknown command line argument '%s'.", option);
			exit(EXIT_FAILURE);
		}
	}
}

int	main(int ac, char **av)
{
	int			i;

	i = 0;
	while (++i < ac && *av[i] == '-' && (ft_strcmp(av[i], "--") != 0))
		get_option(av[i]);
	if (i == ac)
		handle_file("a.out");
	else
	{
		if (!ft_strcmp(av[i], "--"))
			++i;
		if (i < ac - 1)
			g_flags |= o_MULT;
		while (i < ac)
			handle_file(av[i++]);
	}
	return (0);
}
