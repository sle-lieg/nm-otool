#include <stdio.h>

int main()
{
	char	tab[4];
	int	nb = 123456789;
	int	*nb2;

	*nb2 = 123456789;
	*(int *)tab = nb;

	// 75BCD15

	printf("nb2[0]: &%p = %x\n", nb2, (char*)nb2[0]);
	printf("nb2[1]: &%p = %x\n", ((char*)nb2+1), *((char*)nb2)+1);
	printf("nb2[2]: &%p = %x\n", ((char*)nb2+2), *((char*)nb2)+2);
	printf("nb2[3]: &%p = %x\n", ((char*)nb2+3), *((char*)nb2)+3);

	// printf("tab[0]: &%p = %x\n", tab, tab[0]);
	// printf("tab[1]: &%p = %x\n", &tab[1], tab[1]);
	// printf("tab[2]: &%p = %x\n", &tab[2], tab[2]);
	// printf("tab[3]: &%p = %x\n", &tab[3], tab[3]);
	return (0);
}
