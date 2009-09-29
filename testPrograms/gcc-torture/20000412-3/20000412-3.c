#include <conprint.h>

typedef struct {
	char y;
	char x[32];
} X;

int z (void)
{
	X xxx;
	xxx.x[0] =
		xxx.x[31] = '0';
	xxx.y = 0xf;
	return f (xxx, xxx);
}

int _main (void)
{
	int val;

	val = z ();
	/*printf("val: 0x%x.\n", val);
	maWait(0, 5000);*/
	if (val != 0x60)
		abort ();
	exit (0);
}

int f(X x, X y)
{
	/*InitConsole();
	printf("x.y: 0x%x. y.y: 0x%x.\n", x.y, y.y);
	printf("x.x[0]: 0x%x. y.x[0]: 0x%x.\n", x.x[0], y.x[0]);*/
	if (x.y != y.y) {
		return 'F';
	}

	return x.x[0] + y.x[0];
}
