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

enum e_errors {
	ERR_FILE_NAME = -4,
	ERR_FSTAT,
	ERR_IS_DIR,
	ERR_MMAP
};

#endif
