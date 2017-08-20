typedef unsigned long ui;

typedef struct{
	ui *n;
	int sign;
	int len;    //The number of used digits.
	int size;   //The number of allocated digits.
} MDZ_t[1], *MDZ;

#define LOW(a) ((a << 32) >> 32)
#define HIGH(a) (a >> 32)

void mul2add2(ui *zH, ui *zL, ui a, ui b, ui c, ui d);
