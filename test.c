#include <unistd.h> 

int foo()
{
	write(1, "ABC\n", 4);
	return (0);
}
