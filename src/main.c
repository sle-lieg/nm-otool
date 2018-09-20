/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sle-lieg <sle-lieg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/13 14:24:50 by sle-lieg          #+#    #+#             */
/*   Updated: 2018/09/20 19:17:37 by sle-lieg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

int	g_flags;
char	*g_filename;

uint64_t bendtolend(void *data, size_t size)
{
	uint64_t ret;
	uint8_t	*ptr;

	ret = 0;
	ptr = (uint8_t *)data;
	while (size--)
		ret = (ret << 8) + *ptr++;
	return (ret);
}

// TODO: add tab to funtion ptr for parsers
void	nm(void *file_mmap)
{
	t_fat_arch *arch;
	uint32_t magic_number;
	uint32_t	nb_arch;
	uint32_t	offset;
	size_t i;

	magic_number = *(int *)file_mmap;
	i = 0;
	if (magic_number == MH_MAGIC_64)
		parse_l64(file_mmap);
	else if (magic_number == MH_CIGAM_64)
		parse_b64(file_mmap);
	else if (magic_number == MH_MAGIC)
		parse_l32(file_mmap);
	else if (magic_number == MH_CIGAM)
		parse_b32(file_mmap);
	else if (magic_number == FAT_CIGAM || magic_number == FAT_CIGAM_64)
	{
		nb_arch = bendtolend(&((struct fat_header*)file_mmap)->nfat_arch, sizeof(uint32_t));
		while (i < nb_arch)
		{
			arch = &((struct fat_arch *)((char*)file_mmap + sizeof(struct fat_header)))[i++];
			offset = bendtolend(&arch->offset, sizeof(uint32_t));
			nm((char*)file_mmap + offset);
		}
	}
	// ft_printf("magic_number = %x\n\n", magic_number);
}

int	handle_error(int code_error, char *file)
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

int	handle_file(char *file_name)
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

int	get_option(char *option)
{
	static char		*match_flag = OPTIONS;
	int				i;

	i = 0;
	while (*option == '-')
		option++;
	while (*option)
	{
		if ((i = ft_strichr(match_flag, *option++)) >= 0)
			g_flags |= (1 << i);
		else
			return (ERR_OPTION);
	}
	return (0);
}

static int	is_option(char *arg)
{
	return (*arg == '-' && (ft_strcmp(arg, "--") != 0));
}

int	main(int ac, char **av)
{
	int			i;

	i = 0;
	while (++i < ac && is_option(av[i]))
	{
		if (get_option(av[i]) == ERR_OPTION)
		{
			ft_printf("nm: Unknown command line argument '%s'.", av[i]);
			exit(EXIT_FAILURE);
		}
	}
	if (i == ac)
		handle_file("a.out");
	else
	{
		if (!ft_strcmp(av[i], "--"))
			++i;
		while (i < ac)
			handle_file(av[i++]);
	}
	return (0);
}
