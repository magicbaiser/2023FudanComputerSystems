/* 
 * CS:APP Data Lab 
 * 
 * <Please put your name and userid here>
 * 
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.  
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:
 
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
 
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>
    
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.

 
  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting if the shift amount
     is less than 0 or greater than 31.


EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require you to implement floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants. You can use any arithmetic,
logical, or comparison operations on int or unsigned data.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legality of your solutions.
  2. Each function has a maximum number of operations (integer, logical,
     or comparison) that you are allowed to use for your implementation
     of the function.  The max operator count is checked by dlc.
     Note that assignment ('=') is not counted; you may use as many of
     these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. Use the BDD checker to formally verify your functions
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies 
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the BDD checker to formally verify that your solutions produce 
 *      the correct answers.
 */


#endif
#include "bits.h"

//P1
/* 
 * tconst - return a constant value 0xFFFFFFE0
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 2
 *   Rating: 1
 */
int tconst(void) {
	return ((~0) ^ 31) ;
}

//P2
/* 
 * bitNand - return ~(x&y) using only ~ and | 
 *   Example: bitNand(4, 5) = -5, bitNand(3, 11) = -4
 *   Legal ops: ~ |
 *   Max ops: 6
 *   Rating: 2
 */
int bitNand(int x, int y) {
  return (~x) | (~y);
}

//P3
/*
 * ezOverflow - determine if the addition of two signed positive numbers overflows,
 *      and return 1 if overflowing happens and 0 otherwise
 *   You can assume 0 <= x, 0 <= y
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 2
 */
int ezOverflow(int x,int y) {
	return !~((x + y) >> (31));
}

//P4
/* 
 * fastModulo - return x%(2^y)
 *   You can assume 0 <= x, 1 <= y <= 31
 *   Example: fastModulo(0x1234567,0xA) = 0x167
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Rating: 3
 */
int fastModulo(int x,int y) { 
	return ( (1 << y )+ ~0) & x;
}

//P5
/*
 * findDifference - return a mask that marks the different bits of y compared to x
 *   Example: findDifference(12, 10) = findDifference(10, 12) = 6
 *   Legal ops: ~ & |
 *   Max ops: 9
 *   Rating: 3
 */
int findDifference(int x,int y) {
	return ~(~(x | y)|(~(~x | ~y)));
}

//P6
/*
 * absVal - return the absolute value of x
 *   Examples: absVal(-10) = 10
 *			   absVal(5) = 5
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 4
 */
int absVal(int x) {
	int temp, mid;
	temp = x >> 31;
	mid = x + temp;
  	return mid ^ temp;
}

//P7
/*
 * secondLowBit - return a mask that marks the position of the second least significant 1 bit.
 *   Examples: secondLowBit(0x00000110) = 0x00000100
 *			       secondLowBit(0xFEDC1A80) = 0x00000200
 *             secondLowBit(0)  = 0
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 8
 *   Rating: 4
 */
int secondLowBit(int x) {
	int mid, temp, ttemp;
	mid = x + ~0;
	temp = mid & x;
	ttemp = ~(temp + ~0); 
	return temp & ttemp;
}

//P8
/*
 * byteSwap - swaps the nth byte and the mth byte
 *  Examples: byteSwap(0x12345678, 1, 3) = 0x56341278
 *            byteSwap(0xDEADBEEF, 0, 2) = 0xDEEFBEAD
 *  You may assume that 0 <= n <= 3, 0 <= m <= 3
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 20
 *  Rating: 5
 */
int byteSwap(int x, int n, int m) {
	int bitm, bitn, tempm, usem, midm, finalm, tempn, usen, midn, finaln, midx, finalx;
	bitm = m << 3;
	bitn = n << 3;
	tempm = x >> bitm;
	usem = tempm & 255;
	midm = usem << bitm;
	finalm = usem << bitn;
	tempn = x >> bitn;
	usen = tempn & 255;
	midn = usen << bitn;
	finaln = usen << bitm;
	midx = x + 2 + ~midn + ~midm;
	finalx = midx + finalm + finaln;
	return finalx;
}

//P9
/* 
 * byteCheck - Returns the number of bytes that are not equal to 0
 *   Examples: byteCheck(0xFE0A4100) = 3
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 5
 */
int byteCheck(int x) {
	int first, second, third, fourth, finalfirst, finalsecond, finalthird, finalfourth;
	first = x << 24;
	second = (x << 16) >> 24;
	third = (x << 8) >> 24;
	fourth = x >> 24;
  	finalfirst = !(!first);
  	finalsecond = !(!second);
  	finalthird = !(!third);
  	finalfourth = !(!fourth);
  	return finalfirst + finalsecond + finalthird + finalfourth;
}

//P10
/* 
 * fractions - return floor(x*7/16), for 0 <= x <= (1 << 28), x is an integer 
 *   Example: fractions(20) = 8
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 8
 *   Rating: 5
 */
int fractions(int x) {
	int temp, ttemp, tttemp;
	temp = x;
	ttemp = x + x;
	tttemp = ttemp + ttemp;
  	return (temp + ttemp + tttemp) >> 4;
}

//P11
/* 
 * biggerOrEqual - if x >= y  then return 1, else return 0 
 *   Example: biggerOrEqual(4,5) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 6
 */
int biggerOrEqual(int x, int y) {
	int rejudge, rrejudge, temp, ttemp, result, rresult, judge, midjudge, finaljudge;
	rejudge = x >> 31;
	rrejudge = y >> 31;
	temp = !rejudge;
	ttemp = !rrejudge;
	result = temp | !ttemp;
	rresult = temp & !ttemp;
	judge = x + ~y + 1;
	midjudge = judge >> 31;
	finaljudge = !midjudge;
	return (finaljudge & result) | rresult;
}


//P12
/*
 * hdOverflow - determine if the addition of two signed 32-bit integers overflows,
 *      and return 1 if overflowing happens and 0 otherwise
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 6
 */
int hdOverflow(int x,int y) {
	int sum, mids, amid, aamid, midx, bmid, bbmid, midy, cmid, ccmid;
	sum = x + y;
	mids = sum >> 31;
	amid = !mids;
	aamid = !amid;
	midx = x >> 31;
	bmid = !midx;
	bbmid = !bmid;
	midy = y >> 31;
	cmid = !midy;
	ccmid = !cmid;
	return !((bbmid ^ ccmid) | ((bbmid & ccmid & aamid )| (amid & bmid & cmid)));
}

//P13
/* 
 * overflowCalc - given binary representations of three 32-bit positive numbers and add them together, 
 *      return the binary representation of the part where bits are higher than 32.
 *   Examples: overflowCalc(0xffffffff,0xffffffff,0xffffffff) = 2
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 30
 *   Rating: 7
 */
int overflowCalc(int x, int y, int z) {
	int judge, clear, midx, first, midy, second, midz, third, add1, add2, add3, add, sum;
	judge = 255 << 8;
	clear = judge + 255;
	midx = x >> 16;
	first = midx & clear;
	midy = y >> 16;
	second = midy & clear;
	midz = z >> 16;
	third = midz & clear;
	add1 = x & clear;
	add2 = y & clear;
	add3 = z & clear;
	add = (add1 + add2 + add3 ) >> 16;
	sum = (add + first + second + third) >> 16; 
	return sum & 3 ;
	
}

//P14
/* 
 * logicalShift - shift x to the right by n, using a logical shift
 *   Can assume that 0 <= n <= 31
 *   Examples: logicalShift(0x87654321,4) = 0x08765432
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 8
 */
int logicalShift(int x, int n) {
	int high, mid, add, mix, midfinal, move, moven;
	high = x >> 31;
	mid = high & 1;
	add = mid << 31;
	mix = add + x;
	midfinal = mix >> n;
	move = 32 + ~n;
	moven = mid << move;
  	return moven + midfinal;
}

//P15
/* 
 * partialFill - given l, h, fill all even digits within the [l, h] interval with 1 (subscripts starting from 0)
 *   Can assume that 0 <= l <= 15, 16 <= h <= 31
 *   Examples: partialFill(13,16) = 0x00014000
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 8
 */
int partialFill(int l, int h) {
	int evenl, evenh, mid, temp, addl1, minush1, add1, add0, minus1, minus2, clear1, clear2, result1, result2;
	evenl = l + (l&1);
	evenh = h | 1;
	mid = 85 << 24;
	temp = ~evenh + 32;
	addl1 = evenl + 8;
	minush1 = 8 + temp;
	add1 = 85 << addl1;
	add0 = 85 << evenl;
	minus1 = mid >> temp;
	minus2 = mid >> minush1;
	clear1 = (255 << 8 )+ 255;
	clear2 = clear1 << 16;
	result1 = (add1 + add0) & clear1;
	result2 = (minus1 + minus2) & clear2;
	return result1 + result2;
}

//P16
/* 
 * float_abs - Return bit-level equivalent of expression |f| (absolute value of f) for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   When argument is NaN, return argument.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 20
 *   Rating: 3
 */
unsigned float_abs(unsigned uf) {
	int temp, exp, frac;
	unsigned result;
	temp = 2147483647;
	result = temp & uf;
	exp = uf & 0x7f800000;
	frac = uf & 0x007fffff;
	if(exp == 0x7f800000 && frac != 0 )
	{
		return uf;
	}
  	return result;
}

//P17
/* 
 * float_cmp - Return 1 if uf1 > uf2, and 0 otherwise.
 *   Both of the arguments are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   When ant of the arguments is NaN, return 0.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 40
 *   Rating: 5
 */
unsigned float_cmp(unsigned uf1, unsigned uf2) {
	unsigned sign1, sign2;
	int sign4, exp1, exp2, frac1, frac2;
	sign1 = uf1 & 0x80000000;
	sign2 = uf2 & 0x80000000;
	sign4 = uf2 & 0x7fffffff;
	exp1 = uf1 & 0x7f800000;
	exp2 = uf2 & 0x7f800000;
	frac1 = uf1 & 0x007fffff;
	frac2 = uf2 & 0x007fffff;
	if((exp1 == 0x7f800000 && frac1 != 0) ||(exp2 == 0x7f800000 && frac2 != 0))
	{
		return 0;
	}
	if(sign1 > sign2)
	{
		return 0;
	}
	if(sign1 < sign2 && sign4!= 0)
	{
		return 1;
	}
	if(sign1 == 0)
	{
		if(exp1 > exp2)
		{
			return 1;
		}
		if(exp1 == exp2)
		{
			return frac1 > frac2;
		}
		return 0;
	}
	if(sign1 != 0)
	{
		if(exp1 < exp2 )
		{
			return 1;
		}
		if(exp1 == exp2)
		{
			return frac1 < frac2;
		}
		return 0;
	}
	return 1;
}

//P18
/* 
 * float_pow2 - Return bit-level equivalent of expression f*(2^n) for
 *   floating point argument f and integer argument n.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   When argument is NaN, return argument.
 *   You can assume 0 <= n <= 255
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 50
 *   Rating: 6
 */
unsigned float_pow2(unsigned uf,int n) {
	unsigned sign1, result, secondresult, thirdresult, fourthresult;
	int exp, usedexp, finalexp, secondfinalexp, judge, tempfrac, usedfinalexp, used2finalexp, frac, temp;
	sign1 = uf & 0x80000000;
	exp = uf & 0x7f800000;
	usedexp = exp >> 23;
	finalexp = usedexp + n;
	frac = uf & 0x007fffff;
	temp = uf & 0x807fffff;
	tempfrac = frac;
	judge = 0;
	while(!(tempfrac & 0x00800000) && frac != 0)
	{
		tempfrac = tempfrac << 1;
		judge = judge + 1;
	}
	secondfinalexp = n - judge;
	usedfinalexp = finalexp << 23;
	used2finalexp = secondfinalexp << 23;
	result = temp + usedfinalexp;
	secondresult = (uf | 0x7f800000 )- frac;
	thirdresult = (frac << n )+ sign1;
	fourthresult = sign1 + used2finalexp + tempfrac;
	if(usedexp == 255)
	{
		return uf;
	}
	if(exp == 0)
	{
		if(frac == 0)
		{
			return uf;
		}
		if(n == 255 && judge == 1)
		{
			return sign1 + 0x7f800000;
		}
		if(n > judge)
		{
			return fourthresult;
		}
		return thirdresult;
	}
	if(finalexp >= 255)
	{
		return secondresult;
	}
  	return result;
}


//P19
/* 
 * float_i2f - Return bit-level equivalent of expression (float) x
 *   Result is returned as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point values.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 40
 *   Rating: 7
 */
unsigned float_i2f(int x) {
	unsigned usedx, sign, result;
	int exp, temp, carry, realexp, judge, highjudge, judge1, judge2, resultexp, resultfrac;
	usedx = x;
	sign = x & 0x80000000;
	exp = 0; 
	carry = 1;
	if(x == 0)
	{
		return 0;
	}
	if(x < 0)
	{
		usedx = -x;
	}
	temp = usedx;
	while(!(temp & 0x80000000))
	{
		usedx = usedx << 1;
		exp = exp + 1;
		temp = usedx;
	}
	usedx = usedx << 1;
	realexp = 158 - exp;
	judge = usedx & 0x1ff;
	highjudge = judge >> 8;
	if(!highjudge)
	{
		carry = 0;
	}
	judge1 = judge & 255;
	judge2 = usedx & 0x200;
	if(!judge2 & !judge1)
	{
		carry = 0;
	}
	resultexp = realexp << 23;
	resultfrac = usedx >> 9;
	result = sign + resultexp + resultfrac + carry;
	return result;
}


#ifdef NOT_SUPPOSED_TO_BE_DEFINED
#   __          __  _                          
#   \ \        / / | |                         
#    \ \  /\  / /__| | ___ ___  _ __ ___   ___ 
#     \ \/  \/ / _ \ |/ __/ _ \| '_ ' _ \ / _ \
#      \  /\  /  __/ | (_| (_) | | | | | |  __/
#       \/  \/ \___|_|\___\___/|_| |_| |_|\___|
#                                              

#  ██╗  ██╗ ██████╗ ███╗   ██╗ ██████╗ ██████╗     ██████╗  █████╗ ██████╗ ████████╗
#  ██║  ██║██╔═══██╗████╗  ██║██╔═══██╗██╔══██╗    ██╔══██╗██╔══██╗██╔══██╗╚══██╔══╝
#  ███████║██║   ██║██╔██╗ ██║██║   ██║██████╔╝    ██████╔╝███████║██████╔╝   ██║   
#  ██╔══██║██║   ██║██║╚██╗██║██║   ██║██╔══██╗    ██╔═══╝ ██╔══██║██╔══██╗   ██║   
#  ██║  ██║╚██████╔╝██║ ╚████║╚██████╔╝██║  ██║    ██║     ██║  ██║██║  ██║   ██║   
#  ╚═╝  ╚═╝ ╚═════╝ ╚═╝  ╚═══╝ ╚═════╝ ╚═╝  ╚═╝    ╚═╝     ╚═╝  ╚═╝╚═╝  ╚═╝   ╚═╝   
#                                                                                   
#endif

//P20
/*
 * oddParity - return the odd parity bit of x, that is, 
 *      when the number of 1s in the binary representation of x is even, then the return 1, otherwise return 0.
 *   Examples:oddParity(5) = 1, oddParity(7) = 0
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 56
 *   Challenge: You will get 1 extra point if you use less than or equal to 34 ops
 *   Rating: 2
 */
int oddParity(int x) {
	int judge1, judge11, judge2, judge22, judge3, judge33, judge4, midresult1, midresult2, result1;
	int midresult3, midresult4, result2, midresult5, result3, midresult6, result4, midresult7;
	judge1 = 15 + (15 << 8);
	judge11 = judge1 + (judge1 << 16);
	judge2 = 85 + (85 << 8);
	judge22 = judge2 + (judge2 << 16); 
	judge3 = 51 + (51 << 8);
	judge33 = judge3 + (judge3 << 16);
	judge4 = 255 + (255 << 16);
	midresult1 = judge22 & x;
	midresult2 = judge22 & (x >> 1);
	result1 = midresult1 + midresult2;
	midresult3 = result1 & judge33;
	midresult4 = judge33 & (result1 >> 2);
	result2 = midresult3 + midresult4;
	midresult5 = result2 + (result2 >> 4);
	result3 = judge11 & midresult5;
	midresult6 = result3 + (result3 >> 8);
	result4 = judge4 & midresult6;	
	midresult7 = result4 + (result4 >> 16);
  	return !(midresult7 & 1);
}

//P21
/*
 * bitReverse - Reverse bits in an 32-bit integer
 *   Examples: bitReverse(0x80000004) = 0x20000001
 *             bitReverse(0x7FFFFFFF) = 0xFFFFFFFE
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 56
 *   Challenge: You will get 1 extra point if you use less than or equal to 34 ops
 *   Rating: 2
 */
int bitReverse(int x) {
	int judge1, judge11, judge2, judge22, judge3, judge33, judge4, judge44, midresult1, midresult2, result1;
	int midresult3, midresult4, result2, midresult5, midresult6, result3, midresult7, midresult8, result4;
	int final1, final2, result;
	judge1 = 15 + (15 << 8);
	judge11 = judge1 + (judge1 << 16);
	judge2 = 85 + (85 << 8);
	judge22 = judge2 + (judge2 << 16); 
	judge3 = 51 + (51 << 8);
	judge33 = judge3 + (judge3 << 16);
	judge4 = 255 + (255 << 16);
	judge44 = 255 + (255 << 8);
	midresult1 = judge22 & (x >> 1);
	midresult2 = (x & judge22) << 1;
	result1 = midresult1 + midresult2;
	midresult3 = judge33 & (result1 >> 2);
	midresult4 = (result1 & judge33) << 2;
	result2 = midresult3 + midresult4;
	midresult5 = judge11 & (result2 >> 4);
	midresult6 = (result2 & judge11) << 4;
	result3 = midresult5 + midresult6;
	midresult7 = judge4 & (result3 >> 8);
	midresult8 = (result3 & judge4) << 8;
	result4 = midresult7 + midresult8;
	final1 = judge44 & (result4 >> 16);
	final2 = result4 << 16;
	result = final1 + final2;
	return result;
}

//P22
/*
 * mod7 - calculate x mod 7 without using %.
 *   Example: mod7(99) = 1
 *            mod7(-101) = -3
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 100
 *   Challenge: You will get 2 extra point if you use less than or equal to 56 ops
 *   Rating: 2
 */
int mod7(int x) {
	int highx, tempx1, midx1, tempx2, midx2, tempx3, midx3, tempx4, midx4, tempx5, midx5;
	int tempx6, midx6, tempx7, midx7, tempx8, midx8, tempx9, midx9, tempx10, midx10, tempx11, tempx12;
	int result1, rtemp1, rmid1, rtemp2, rmid2, result2, rrtemp, rrmid, result3, midresult3, high;
	int result, judge, finalresult;
	highx = x >> 31;
	tempx1 = x & 7;
	midx1 = x >> 3;
	tempx2 = midx1 & 7;
	midx2 = midx1 >> 3;
	tempx3 = midx2 & 7;
	midx3 = midx2 >> 3;
	tempx4 = midx3 & 7;
	midx4 = midx3 >> 3;
	tempx5 = midx4 & 7;
	midx5 = midx4 >> 3;
	tempx6 = midx5 & 7;
	midx6 = midx5 >> 3;
	tempx7 = midx6 & 7;
	midx7 = midx6 >> 3;
	tempx8 = midx7 & 7;
	midx8 = midx7 >> 3;
	tempx9 = midx8 & 7;
	midx9 = midx8 >> 3;
	tempx10 = midx9 & 7;
	midx10 = midx9 >> 3;
	tempx11 = midx10 & 1;
	tempx12 = highx & 5;
	result1 = tempx1 +tempx2 +tempx3 +tempx4 +tempx5 +tempx6 +tempx7 +tempx8 +tempx9 +tempx10 +tempx11 + tempx12;
	rtemp1 = result1 & 7;
	rmid1 = result1 >> 3;
	rtemp2 = rmid1 & 7;
	rmid2 = rmid1 >> 3;
	result2 = rtemp1 + rtemp2 + rmid2;
	rrtemp = result2 & 7;
	rrmid = result2 >> 3;
	result3 = rrmid + rrtemp;
	midresult3 = result3 + (~6);
	high = midresult3 >> 31;
	result = result3 & high;
	judge = highx & (~6);
	finalresult = result + judge;
	return finalresult & high;
}
