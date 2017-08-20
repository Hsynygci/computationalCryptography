#include<stdio.h>
#include<stdlib.h>
#include<math.h>

typedef unsigned long ui;

typedef struct {
	ui *n;
	int sign;
	int len;    //The number of used digits.
	int size;   //The number of allocated digits.
} MDZ_t[1], *MDZ;

#define LOW(a) ((a << 32) >> 32)
#define HIGH(a) (a >> 32)

void static mul2add2(ui *zH, ui *zL, ui a, ui b, ui c, ui d) {
	ui aH, aL, bH, bL, cH, cL, dH, dL;
	ui aHbH, aHbL, aLbH, aLbL, zHH, zHL, zLH, zLL;
	ui aLbLH, aLbLL, aHbLH, aHbLL, aLbHH, aLbHL, aHbHH, aHbHL;

	aH = HIGH(a);
	aL = LOW(a);
	bH = HIGH(b);
	bL = LOW(b);
	cH = HIGH(c);
	cL = LOW(c);
	dH = HIGH(d);
	dL = LOW(d);

	aLbL = aL * bL;
	aLbH = aL * bH;
	aHbL = aH * bL;
	aHbH = aH * bH;

	aLbL = aLbL + cL;
	aLbL = aLbL + dL;
	aLbLH = HIGH(aLbL);
	aLbLL = LOW(aLbL);

	aHbL = aHbL + cH;
	aHbL = aHbL + dH;
	aHbLH = HIGH(aHbL);
	aHbLL = LOW(aHbL);

	aLbH = aLbH + aHbLL;
	aLbH = aLbH + aLbLH;
	aLbHH = HIGH(aLbH);
	aLbHL = LOW(aLbH);

	aHbH = aHbH + aHbLH;
	aHbH = aHbH + aLbHH;
	aHbHH = HIGH(aHbH);
	aHbHL = LOW(aHbH);

	zLL = aLbLL;
	zLH = aLbHL;
	zHL = aHbHL;
	zHH = aHbHH;
	*zL = (zLH << 32) + zLL;
	*zH = (zHH << 32) + zHL;
}

void mdz_init(MDZ z, int l) {
	z->n = (unsigned long *) malloc(sizeof(unsigned long) * l);
	z->len = l;
}

void mdz_rand(MDZ z) {
	int i;
	for (i = 0; i < z->len; i++) {
		z->n[i] = ((unsigned long) random()) * ((unsigned long) random());
	}
	z->sign = 0; //Positive.
}

void mdz_print(char *s, MDZ z) {
	int i;
	printf("%s :=", s);
	for (i = 0; i < z->len; i++) {
		printf(" + %19lu * (2^64)^%d", z->n[i], i);
		fflush(stdout);
	}
	printf(";\n");
	fflush(stdout);
}

int mdz_add(MDZ c, MDZ a, MDZ b) {
	int i, cr, cl = 7;

	if (a->len < b->len) {
		MDZ t;
		t = a;
		a = b;
		b = t;
	}
	cr = 0;
	for (i = 0; i < b->len; i++) {
		c->n[i] = a->n[i] + b->n[i] + cr;
		if (a->n[i] > c->n[i]) {
			cr = 1;
		} else if (a->n[i] < c->n[i]) {
			cr = 0;
		}
	}
	for (i = b->len; i < a->len; i++) {
		c->n[i] = a->n[i] + cr;
		if (a->n[i] > c->n[i]) {
			cr = 1;
		} else if (a->n[i] < c->n[i]) {
			cr = 0;
		}
	}
	if (cr == 0) {
		c->len = a->len;
	} else {
		c->n[a->len] = cr;
		c->len = a->len + 1;
	}
	return cl;
}

int mdz_sub(MDZ c, MDZ a, MDZ b) {
	int i, cl = 3;

	if (a->len < b->len) {
		MDZ t;
		t = a;
		a = b;
		b = t;
	}
	for (i = 0; i < b->len; i++) {
		if (a->n[i] > b->n[i]) {
			c->n[i] = a->n[i] - b->n[i];
		} else {
			a->n[i + 1]--;
			c->n[i] = 18446744073709551615UL - b->n[i] + 1;
			c->n[i] = c->n[i] + a->n[i];
		}
	}
	for (i = b->len; i < a->len; i++) {
		c->n[i] = a->n[i];
	}
	return cl;
}

void mdz_mul(MDZ c, MDZ a, MDZ b) {
	//Implement with a test scenario.
	ui zH, zL;

	int i;
	int j;
	for (i = 0; i < a->len; i++) {
		ui d = 0;
		for (j = 0; j < b->len; j++) {
			mul2add2(&zH, &zL, a->n[i], b->n[j], c->n[j + i], d);
			c->n[i + j] = zL;
			d = zH;
		}
		c->n[i + j] = d;
	}

}

void mdz_karatsuba(MDZ c, MDZ a, MDZ b) {

	MDZ_t AH, AL, BH, BL, CH, CL, DH, DL, T, z, x1, x2, x3;
	mdz_init(AH, a->len / 2);
	mdz_init(AL, a->len / 2);
	mdz_init(BH, a->len / 2);
	mdz_init(BL, a->len / 2);
	mdz_init(CH, a->len);
	mdz_init(CL, a->len);
	mdz_init(DH, AL->len + 1);
	mdz_init(DL, BL->len + 1);
	mdz_init(T, DL->len * 2);
	mdz_init(z, T->len);
	mdz_init(x1, a->len);
	mdz_init(x3, T->len);
	mdz_init(x2, a->len * 3);

	int i;
	for (i = 0; i < a->len / 2; i++) {
		AL->n[i] = a->n[i];
		AH->n[i] = a->n[i + (a->len / 2)];
	}

	int j;
	for (j = 0; j < b->len / 2; j++) {
		BL->n[j] = b->n[j];
		BH->n[j] = b->n[j + (b->len / 2)];
	}

//	mdz_print("AH", AH);
//	mdz_print("AL", AL);
//	mdz_print("BH", BH);
//	mdz_print("BL", BL);
	mdz_mul(CH, BH, AH);
//	mdz_print("CH", CH);
//	printf("CH eq AH* BH;\n\n");
//	mdz_print("AL",AL);
//	mdz_print("BL",BL);
	mdz_mul(CL, AL, BL);
//	mdz_print("CL", CL);
//	printf("CL eq AL* BL;\n\n");
//	mdz_print("AH",AH);
//	mdz_print("AL",AL);
	mdz_add(DH, AH, AL);
//	mdz_print("DH", DH);
//	printf("DH eq AL + AH;\n\n");
//	mdz_print("BL", BL);
//	mdz_print("BH", BH);
	mdz_add(DL, BH, BL);
//	mdz_print("DL", DL);
//	printf("DL eq BL + BH;\n\n");
//	mdz_print("DH", DH);
//	mdz_print("DL", DL);
	mdz_mul(T, DH, DL);
//	mdz_print("T", T);
//	printf("T eq DH * DL;\n\n");
//	mdz_print("T", T);
//	mdz_print("CH", CH);
	mdz_sub(z, T, CH);
//	mdz_print("z", z);
//	printf("z eq T - CH;\n\n");
//	mdz_print("z", z);
//	mdz_print("CL", CL);
	mdz_sub(x3, z, CL);
//	mdz_print("x3", x3);
//	printf("x3 eq z - CL;\n\n");
//	mdz_print("CH", CH);

	MDZ_t x5;
	mdz_init(x5, CH->len + a->len);
	int k;
	for (k = 0; k < x5->len; k++) {
		if (k < a->len) {
			x5->n[k] = 0;
		} else {
			x5->n[k] = CH->n[k - a->len];
		}
	}
//	mdz_print("x5", x5);
//	mdz_print("x3", x3);
	MDZ_t x6;
	mdz_init(x6, z->len + (a->len / 2));
	int y;
	for (y = 0; y < x6->len; y++) {
		if (y < a->len / 2) {
			x6->n[y] = 0;
		} else {
			x6->n[y] = x3->n[y - (a->len / 2)];
		}
	}

//	mdz_print("x6", x6);
//	mdz_print("CH", CH);
//	mdz_print("CL", CL);
	MDZ_t x7;
	mdz_init(x7, x5->len + 1);
	mdz_add(x7, x6, x5);
//	mdz_print("x2", x2);
//	printf("x2 eq CH + CL;\n\n");
//	mdz_print("x3", x3);
//	mdz_print("x2", x2);
	mdz_add(c, x7, CL);
//	mdz_print("c", c);
//	printf("c eq x2 + x3;\n\n");
}

int main() {

	//Test for mul2add2.
//	int i;
//	for (i = 0; i < 1000; i++) {
//		ui a = (ui)random()*(ui)random();
//		ui b = (ui)random()*(ui)random();
//		ui c = (ui)random()*(ui)random();
//		ui d = (ui)random()*(ui)random();
//		ui zH, zL;
//
//		mul2add2(&zH, &zL, a, b, c, d);
//
//		printf("a := %lu;\n", a);
//		printf("b := %lu;\n", b);
//		printf("c := %lu;\n", c);
//		printf("d := %lu;\n\n", d);
//		printf("z := a*b + c + d;\n\n");
//		printf("z eq (%lu + (2^64)*%lu);\n\n", zL, zH);
//	}

	MDZ_t a, b, c;

	srand(1224);

	mdz_init(a, 2);
	mdz_init(b, 2);
	mdz_init(c, 4);

	mdz_rand(a);
	mdz_rand(b);

	mdz_print("a", a);
	mdz_print("b", b);

//	mdz_add(c, a, b);
//	mdz_sub(c, a, b);
	mdz_mul(c, a, b);
//	mdz_karatsuba(c, a, b);
	mdz_print("c", c);
//	printf("c eq a+b;\n\n");
//	printf("c eq a-b;\n\n");
	printf("c eq a*b;\n\n");
	//mdz_free(c);
	//mdz_free(b);
	//mdz_free(a);

	return 1;
}
