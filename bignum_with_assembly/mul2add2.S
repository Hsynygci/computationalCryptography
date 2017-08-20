.text

.globl mul2add2

mul2add2:
	movq %rdx, %rax
	mulq %rcx
	addq %r8, %rax       ;//rax=rax+r8
	adcq $0, %rdx        ;//rdx=rdx+carry
	addq %r9, %rax       ;//rax=rax+r9
	adcq $0, %rdx        ;//rdx=rdx+carry
	movq %rax, (%rsi)    ;//*zL=rax
	movq %rdx, (%rdi)    ;//*zH=rdx
	ret



//void static mul2add2(ui *zH, ui *zL, ui a, ui b, ui c, ui d){
//	ui aH, aL, bH, bL, cH, cL, dH, dL;
//	ui aHbH, aHbL, aLbH, aLbL, zHH, zHL, zLH, zLL;
//	ui aLbLH, aLbLL, aHbLH, aHbLL, aLbHH, aLbHL, aHbHH, aHbHL;
//
//	aH = HIGH(a); aL = LOW(a);
//	bH = HIGH(b); bL = LOW(b);
//	cH = HIGH(c); cL = LOW(c);
//	dH = HIGH(d); dL = LOW(d);
//
//	aLbL = aL*bL; aLbH = aL*bH; aHbL = aH*bL; aHbH = aH*bH;
//
//	aLbL = aLbL + cL; aLbL = aLbL + dL;
//	aLbLH = HIGH(aLbL); aLbLL = LOW(aLbL);
//
//	aHbL = aHbL + cH; aHbL = aHbL + dH;
//	aHbLH = HIGH(aHbL); aHbLL = LOW(aHbL);
//
//	aLbH = aLbH + aHbLL; aLbH = aLbH + aLbLH;
//	aLbHH = HIGH(aLbH); aLbHL = LOW(aLbH);
//
//	aHbH = aHbH + aHbLH; aHbH = aHbH + aLbHH;
//	aHbHH = HIGH(aHbH); aHbHL = LOW(aHbH);
//
//	zLL = aLbLL; zLH = aLbHL; zHL = aHbHL; zHH = aHbHH;
//	*zL = (zLH << 32) + zLL; *zH = (zHH << 32) + zHL;
//}

