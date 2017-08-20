#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include"mylib.h"

void mdz_init(MDZ z, int l){
	z->n = (unsigned long *)malloc(sizeof(unsigned long)*l);
	z->len = l;
}

void mdz_rand(MDZ z){
	int i;
	for (i = 0; i < z->len; i++) {
		z->n[i] = ((unsigned long)random()) * ((unsigned long)random());
	}
	z->sign = 0; //Positive.
}

void mdz_print(char *s, MDZ z){
	int i;
	printf("%s :=", s);
	for(i=0; i<z->len; i++){
		printf(" + %19lu * (2^64)^%d", z->n[i], i);
		fflush(stdout);
	}
	printf(";\n");
	fflush(stdout);
}

//int mdz_add(MDZ c, MDZ a, MDZ b){
//	int i, cr, cl = 7;
//
//	if(a->len < b->len){
//		MDZ t;
//		t = a;
//		a = b;
//		b = t;
//	}
//	cr = 0;
//	for (i = 0; i < b->len; i++) {
//		c->n[i] = a->n[i] + b->n[i] + cr;
//		if(a->n[i] > c->n[i]){
//			cr = 1;
//		}
//		else if(a->n[i] < c->n[i]){
//			cr = 0;
//		}
//	}
//	for (i = b->len; i < a->len; i++) {
//		c->n[i] = a->n[i] + cr;
//		if(a->n[i] > c->n[i]){
//			cr = 1;
//		}
//		else if(a->n[i] < c->n[i]){
//			cr = 0;
//		}
//	}
//	if(cr == 0){
//		c->len = a->len;
//	}
//	else{
//		c->n[a->len] = cr;
//		c->len = a->len + 1;
//	}
//	return cl;
//}

int main(){

//	//Test for mul2add2.
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

	mdz_init(a, 4);
	mdz_init(b, 7);
	mdz_init(c, 8);

	mdz_rand(a);
	mdz_rand(b);

	mdz_print("a", a);
	mdz_print("b", b);

	mdz_add(c, a, b);

	mdz_print("c", c);

	printf("c eq a+b;\n\n");

	//mdz_free(c);
	//mdz_free(b);
	//mdz_free(a);


	return 1;
}

