//
//  rmd160.h
//  wif_test
//
//  Created by 宋赓 on 2018/3/9.
//  Copyright © 2018年 宋赓. All rights reserved.
//

#ifndef rmd160_h
#define rmd160_h

#include <stdio.h>
/********************************************************************\
 *
 *      FILE:     rmd160.h
 *
 *      CONTENTS: Header file for a sample C-implementation of the
 *                RIPEMD-160 hash-function.
 *      TARGET:   any computer with an ANSI C compiler
 *
 *      AUTHOR:   Antoon Bosselaers, ESAT-COSIC
 *      DATE:     1 March 1996
 *      VERSION:  1.0
 *
 *      Copyright (c) Katholieke Universiteit Leuven
 *      1996, All Rights Reserved
 *
 \********************************************************************/

#ifndef  RMD160H           /* make sure this file is read only once */
#define  RMD160H

/********************************************************************/

/* typedef 8 and 32 bit types, resp.  */
/* adapt these, if necessary,
 for your operating system and compiler */
typedef    unsigned char        byte;
typedef    unsigned int         dword;

/* if this line causes a compiler error,
 adapt the defintion of dword above */
typedef int the_correct_size_was_chosen [sizeof (dword) == 4? 1: -1];

/********************************************************************/

/* macro definitions */

/* collect four bytes into one word: */
#define BYTES_TO_DWORD(strptr)                    \
(((dword) *((strptr)+3) << 24) | \
((dword) *((strptr)+2) << 16) | \
((dword) *((strptr)+1) <<  8) | \
((dword) *(strptr)))

/* ROL(x, n) cyclically rotates x over n bits to the left */
/* x must be of an unsigned 32 bits type and 0 <= n < 32. */
#define ROL(x, n)        (((x) << (n)) | ((x) >> (32-(n))))

/* the five basic functions F(), G() and H() */
#define F(x, y, z)        ((x) ^ (y) ^ (z))
#define G(x, y, z)        (((x) & (y)) | (~(x) & (z)))
#define H(x, y, z)        (((x) | ~(y)) ^ (z))
#define I(x, y, z)        (((x) & (z)) | ((y) & ~(z)))
#define J(x, y, z)        ((x) ^ ((y) | ~(z)))

/* the ten basic operations FF() through III() */
#define FF(a, b, c, d, e, x, s)        {\
(a) += F((b), (c), (d)) + (x);\
(a) = ROL((a), (s)) + (e);\
(c) = ROL((c), 10);\
}
#define GG(a, b, c, d, e, x, s)        {\
(a) += G((b), (c), (d)) + (x) + 0x5a827999UL;\
(a) = ROL((a), (s)) + (e);\
(c) = ROL((c), 10);\
}
#define HH(a, b, c, d, e, x, s)        {\
(a) += H((b), (c), (d)) + (x) + 0x6ed9eba1UL;\
(a) = ROL((a), (s)) + (e);\
(c) = ROL((c), 10);\
}
#define II(a, b, c, d, e, x, s)        {\
(a) += I((b), (c), (d)) + (x) + 0x8f1bbcdcUL;\
(a) = ROL((a), (s)) + (e);\
(c) = ROL((c), 10);\
}
#define JJ(a, b, c, d, e, x, s)        {\
(a) += J((b), (c), (d)) + (x) + 0xa953fd4eUL;\
(a) = ROL((a), (s)) + (e);\
(c) = ROL((c), 10);\
}
#define FFF(a, b, c, d, e, x, s)        {\
(a) += F((b), (c), (d)) + (x);\
(a) = ROL((a), (s)) + (e);\
(c) = ROL((c), 10);\
}
#define GGG(a, b, c, d, e, x, s)        {\
(a) += G((b), (c), (d)) + (x) + 0x7a6d76e9UL;\
(a) = ROL((a), (s)) + (e);\
(c) = ROL((c), 10);\
}
#define HHH(a, b, c, d, e, x, s)        {\
(a) += H((b), (c), (d)) + (x) + 0x6d703ef3UL;\
(a) = ROL((a), (s)) + (e);\
(c) = ROL((c), 10);\
}
#define III(a, b, c, d, e, x, s)        {\
(a) += I((b), (c), (d)) + (x) + 0x5c4dd124UL;\
(a) = ROL((a), (s)) + (e);\
(c) = ROL((c), 10);\
}
#define JJJ(a, b, c, d, e, x, s)        {\
(a) += J((b), (c), (d)) + (x) + 0x50a28be6UL;\
(a) = ROL((a), (s)) + (e);\
(c) = ROL((c), 10);\
}

/********************************************************************/

/* function prototypes */

void MDinit(dword *MDbuf);
/*
 *  initializes MDbuffer to "magic constants"
 */

void compress(dword *MDbuf, dword *X);
/*
 *  the compression function.
 *  transforms MDbuf using message bytes X[0] through X[15]
 */

void MDfinish(dword *MDbuf, byte *strptr, dword lswlen, dword mswlen);
/*
 *  puts bytes from strptr into X and pad out; appends length
 *  and finally, compresses the last block(s)
 *  note: length in bits == 8 * (lswlen + 2^32 mswlen).
 *  note: there are (lswlen mod 64) bytes left in strptr.
 */


byte *RMD(byte *message , int msglen);
#endif  /* RMD160H */

/*********************** end of file rmd160.h ***********************/

#endif /* rmd160_h */
