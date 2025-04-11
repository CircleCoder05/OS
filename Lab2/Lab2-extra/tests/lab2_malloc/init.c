#include <malloc.h>
#include <pmap.h>

u_int address[1000];
u_int end[1000];
int pos = 0;

void out_of_range()
{
	printk("Invalid address: out of range\n");
}

void overlap()
{
	printk("Invalid address: address overlap\n");
}

void should_null()
{
	printk("Invalid alloc: address should be NULL\n");
}

void not_null()
{
	printk("Invalid alloc: address should not be NULL\n");
}

void not_aligned()
{
	printk("Invalid address: not aligned to 8\n");
}

char check_null(void *p)
{
	if (p != NULL)
	{
		should_null();
		return 0;
	}
	return 1;
}

char check(u_int a, u_int b)
{

	if (a == 0)
	{
		not_null();
		return 0;
	}

	if (a < (u_int)HEAP_BEGIN || b > (u_int)(HEAP_BEGIN + HEAP_SIZE))
	{
		out_of_range();
		return 0;
	}

	if ((a & 7) != 0)
	{
		not_aligned();
		return 0;
	}

	for (int i = 0; i < pos; i++)
	{
		if ((a >= address[i] && a <= end[i]) || (b >= address[i] && b <= end[i]))
		{
			overlap();
			return 0;
		}
	}

	address[pos] = a;
	end[pos++] = b;

	return 1;
}

void rem(u_int a, u_int b)
{
	for (int i = 0; i < pos; i++)
	{
		if (address[i] == a && end[i] == b)
		{
			address[i] = 0;
			end[i] = 0;
		}
	}
}

void malloc_test()
{
	void *p1 = malloc(8);
	void *p2 = malloc(80);
	void *p3 = malloc(32);
	printk("p1: 0x%x  p2: 0x%x  p3: 0x%x\n", p1, p2, p3);
	// free(p1);
	free(p2);
	void *p4 = malloc(9);
	void *p5 = malloc(46);
	printk("p4: 0x%x  p5: 0x%x  p3: 0x%x\n", p4, p5, p3);

	printk("malloc_test() is done\n");
}

void mips_init(u_int argc, char **argv, char **penv, u_int ram_low_size)
{
	printk("init.c:\tmips_init() is called\n");

	mips_detect_memory(ram_low_size);
	mips_vm_init();
	page_init();
	malloc_init();

	malloc_test();
	halt();
}
