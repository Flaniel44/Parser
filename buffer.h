/* File Name:	buffer.h
 * Version:		1
   Compiler:	MS Visual Studio 2012
   Author:		Daniel Spagnuolo, 040 768 219
   Course:		CST 8152 – Compilers, Lab Section: 012
   Assignment:	1
   Date:		Sept. 29th 2015
   Professor:	Sv. Ranev
   Purpose:		Programming and Using Dynamic Structures (buffers) with C
 */
#ifndef BUFFER_H_
#define BUFFER_H_

/*#pragma warning(1:4001) *//*to enforce C89 type comments  - to make //comments an warning */

/*#pragma warning(error:4001)*//* to enforce C89 comments - to make // comments an error */

/* standard header files */
#include <stdio.h>  /* standard input/output */
#include <malloc.h> /* for dynamic memory allocation*/
#include <limits.h> /* implementation-defined data type ranges and limits */

/* constant definitions */
/* You may add your own constant definitions here */
#define R_FAIL_1 -1         /* fail return value */
#define R_FAIL_2 -2         /* fail return value */
#define LOAD_FAIL -2       /* load fail error */
#define SET_R_FLAG 1       /* realloc flag set value */
#define MULTIPLICATIVE_MAX 100 /*Max value for multiplicative mode increment */
#define FIXED_MODE 0 /*Fixed mode value */
#define ADDITIVE_MODE 1 /*addative mode value */
#define MULTIPLICATIVE_MODE -1 /*multiplicative mode value */
#define MULTIPLICATIVE_DIVISOR 100 /*Divisor for calculating new increment */ 
#define SIZE_T_MAX 256 /*Return as error for inc_factor getter */

/* user data type declarations */
typedef struct BufferDescriptor {
    char *cb_head;   /* pointer to the beginning of character array (character buffer) */
    short capacity;    /* current dynamic memory size (in bytes) allocated to character buffer */
    short addc_offset;  /* the offset (in chars) to the add-character location */
    short getc_offset;  /* the offset (in chars) to the get-character location */
    short mark_offset; /* the offset (in chars) to the mark location */
    char  inc_factor; /* character array increment factor */
    char  r_flag;     /* character array reallocation flag */
    char  mode;       /* operational mode indicator*/
    int   eob;       /* end-of-buffer reached flag */
} Buffer, *pBuffer;
/*typedef Buffer *pBuffer;*/

/* function declarations */
Buffer * b_create (short init_capacity,char inc_factor,char o_mode);
pBuffer b_addc(pBuffer const pBD, char symbol);
int b_reset (Buffer * const pBD);
void b_destroy(Buffer * const pBD);
int b_isfull (Buffer * const pBD);
short b_size (Buffer * const pBD);
short b_capacity(Buffer * const pBD);
char * b_setmark (Buffer * const pBD, short mark);
short b_mark (Buffer * const pBD);
int b_mode (Buffer * const pBD);
size_t b_inc_factor (Buffer * const pBD);
int b_load (FILE * const fi, Buffer * const pBD);
int b_isempty (Buffer * const pBD);
int b_eob (Buffer * const pBD);
char b_getc (Buffer * const pBD);
int b_print (Buffer * const pBD);
Buffer *b_pack(Buffer * const pBD);
char b_rflag (Buffer * const pBD);
short b_retract (Buffer * const pBD);
short b_retract_to_mark (Buffer * const pBD);
short b_getc_offset (Buffer * const pBD);


#endif
