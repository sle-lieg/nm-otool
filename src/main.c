#include "nm.h"

void	handle_64(char *ptr)
{
	struct mach_header_64	*header;
	// struct load_command		*ldc;
	struct segment_command_64 *ldc;
	// uint32_t						ncmds;
	uint32_t						i;

	header = (struct mach_header_64 *)ptr;

	ldc = (struct segment_command_64 *)(header + 1);
	i = 0;
	ft_printf("nb: %u\n", header->ncmds);
	while (i++ < header->ncmds)
	{
		ft_printf("ldc[%s]: type=%u size=%u\n", ldc->segname, ldc->cmd, ldc->cmdsize);
		ldc = (struct segment_command_64 *)((char*)ldc + ldc->cmdsize);
	}
	// ldc = (struct load_command *)(header + 1);
	// i = 0;
	// ft_printf("nb: %u\n", header->ncmds);
	// while (i++ < header->ncmds)
	// {
	// 	ft_printf("ldc[%p]: type=%u size=%u\n", ldc, ldc->cmd, ldc->cmdsize);
	// 	ldc = (struct load_command *)((char*)ldc + ldc->cmdsize);
	// }
}

void	nm(void *file_map)
{
	uint32_t magic_number;

	magic_number = *(int *)file_map;
	if (magic_number == MH_MAGIC_64)
		handle_64(file_map);
	// else if (magic_number == MH_CIGAM_64)
	// 	eldnah_64(file_map);
	// else if (magic_number == MH_MAGIC)
	// 	handle_32(file_map);
	// else if (magic_number == MH_CIGAM)
	// 	eldnah_32(file_map);
	// else if (magic_number == FAT_MAGIC)
	// 	ft_printf("fat binary detected\n");
	// else if (magic_number == FAT_CIGAM)
	// 	ft_printf("fat binary detected\n");
	ft_printf("magic_number = %x\n\n", magic_number);
}

int	handle_error(int code_error, char *file)
{
	char bin_path[255];

	getcwd(bin_path, 255);
	if (code_error == ERR_FILE_NAME)
		ft_printf("%s/nm: %s: No such file or directory.\n", bin_path, file);
	else if (code_error == ERR_FSTAT)
		ft_printf("%s/nm: %s: An error occured with fstat.\n", bin_path);
	else if (code_error == ERR_IS_DIR)
		ft_printf("%s/nm: %s: Is a directory.\n", bin_path, file);
	else if (code_error == ERR_MMAP)
		ft_printf("%s/nm: %s: An error occured while mapping the file.\n", bin_path, file);
	return (code_error);
}

int	handle_file(char *file_name)
{
	struct stat	buf;
	int			fd;
	void			*ptr;

	if ((fd = open(file_name, O_RDONLY)) < 0)
		return (handle_error(ERR_FILE_NAME, file_name));
	if (fstat(fd, &buf) < 0)
		return (handle_error(ERR_FSTAT, file_name));
	if ((buf.st_mode & S_IFMT) == S_IFDIR)
		return (handle_error(ERR_IS_DIR, file_name));
	if ((ptr = mmap(NULL, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
		return (handle_error(ERR_MMAP, file_name));
	nm(ptr);
	munmap(ptr, buf.st_size);
	close(fd);
	return (0);
}

int	main(int ac, char **av)
{
	int i;

	i = 1;
	if (ac == 1)
		handle_file("a.out");
	else
	{
		while (i < ac)
			handle_file(av[i++]);
	}
	return (0);
}
