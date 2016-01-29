/*
File name:	buffer.c
Compiler:	MS Visual Studio 2012
Author:		Daniel Spagnuolo, 040 768 219
Course:		CST 8152 – Compilers, Lab Section: 012
Assignment: 1
Date:		Sept. 29th 2015
Professor:	Sv. Ranev
Purpose:	Programming and Using Dynamic Structures (buffers) with C
Function list: Function List: 
		b_create(),
		b_addc(),
		b_reset(),
		b_destroy(),
		b_isfull(),
		b_size(),
		b_capacity(),
		b_setmark(),
		b_mark(),
		b_mode(),
		b_inc_factor(),
		b_load(),
		b_isempty(),
		b_eob(),
		b_getc(),
		b_print(),
		b_pack(),
		b_rflag(),
		b_retract(),
		b_retract_to_mark(),
		b_getc_offset().
*/

#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include "buffer.h"

/*
Purpose: This function creates a new buffer in memory (on the program heap).
Author: Daniel Spagnuolo
History/Versions: v1: Sept. 29, 2015
Called functions: calloc(), malloc(), free()
Parameters: short init_capacity (0-SHRT_MAX)
			char inc_factor
			char o_mode ('f', 'a', or 'm')
Return value: Buffer* buff, or NULL if runtime error.
Algorithm:	-allocate memory for one Buffer structure
			-allocates memory for one dynamic char array using the init capacity
			-Check to determine what the mode and inc_factor will be based on parameters and given rules
			-Return pointer to Buffer on Success, NULL on fail.
*/
Buffer * b_create (short init_capacity,char inc_factor,char o_mode){
	/* Check for abnormal parameters. Initial capacity & inc factor must be greater than 1, mode must be 'f', 'a', or 'm' */
	if (init_capacity < 0 || ((o_mode != 'f' && o_mode != 'a' && o_mode != 'm'))){
		return NULL; /*Return Null if params are bad */
	}else { /*Proceed if params are good */
		Buffer* buff = (Buffer*)calloc(1, sizeof(Buffer)); /* Allocate space for Buffer Struct */
		if (buff == NULL){return NULL;} /* Return Null if allocate failed */
		buff->cb_head = (char *)malloc(init_capacity * sizeof(char)); /*Allocate space for char buffer measured in bytes */
		if (buff->cb_head){ /* Proceed if malloc succeeded. Else, return Null */
			/*Buffer mode assignments. Assign if conditions for mode is met*/
			if ( o_mode == 'f'){
				if (init_capacity < 1){return NULL;}
				buff->mode = FIXED_MODE;
				buff->inc_factor = 0;
				/* Set mode to Addative if o_mode is 'a' and inc_factor is between 1 and 255 inclusive,*/
			} else if(o_mode == 'a' && ((unsigned char)inc_factor >= 1 && (unsigned char)inc_factor <= UCHAR_MAX)){
				buff->mode = ADDITIVE_MODE;
				buff->inc_factor = (unsigned char)inc_factor;
				/* Set mode to Multiplicative if o_mode is 'm' and inc_factor is between 1 and 100 inclusive,*/
		    } else if(o_mode == 'm' && ((unsigned char)inc_factor >= 1 && (unsigned char)inc_factor <= MULTIPLICATIVE_MAX)){
				buff->mode = MULTIPLICATIVE_MODE;
				buff->inc_factor = (unsigned char)inc_factor;
			} else { /*If o_mode and inc_factor do not meet mode requirements, return NULL */
				//free
				free(buff->cb_head);
				free(buff);
				return NULL;
			}
			/*Set the buffer capacity */
			buff->capacity = init_capacity;
			return buff;
		} else { /*malloc failed */
			free(buff);
			return NULL;
		}
	}
}

/*
Purpose: The function tries to add the character symbol to the character array of the given buffer pointed by pBD.
Author: Daniel Spagnuolo
History/Versions: v1: Sept. 29, 2015
Called functions: realloc(), b_inc_factor().
Parameters: pBuffer pBD
			char symbol
Return value: Buffer* buff, or NULL if runtime error.
Algorithm:	-Checks if buffer is full. If it is not, it adds the symbol to the next free space
			-If it is full and the mode is additive or multiplicative, it increases buffer size in the following ways:
				-Additive: increases capacity by adding increment factor to the current capacity.
				-Multiplicative: Multiplies space available by the increment factor and adds it to the current capacity.
			-Capacity cannot go higher than SHRT_MAX
*/
pBuffer b_addc(pBuffer const pBD, char symbol){
	char *tempBuff; /* Temp buff pointer for realloc */
	short availableSpace = 0; /*short for holding the calculated space available */
	short newIncrement = 0; /* short for holding calculated new Increment when mode is -1 */
	if (pBD){ /*Check to make sure buffer pointer is valid */
		pBD->r_flag = 0;
		if(b_isfull(pBD) == 0){ /*if not full, add character to buffer */
			pBD->cb_head[pBD->addc_offset] = symbol;
			++pBD->addc_offset;
			return pBD;
		} else if (b_isfull(pBD) == 1){
			if (pBD->mode == FIXED_MODE) /*Do not expand buffer capacity when mode is fixed-size */
				return NULL; /*end fixed-size mode check */
			else if (pBD->mode == ADDITIVE_MODE){ /*addative self-increment */
				newIncrement = pBD->capacity + ((unsigned short)b_inc_factor(pBD) * sizeof(char)); /*Store the new capacity into a short so that we can check for overflow */
				/*if it overflowed, return NULL */
				if (newIncrement > 0){
					/* New cpacity is old capacity plus the increment factor */
					pBD->capacity += ((unsigned short)b_inc_factor(pBD) * sizeof(char)); 
				} else{ return NULL;}

			} /*end additive mode check */
			else if (pBD->mode == MULTIPLICATIVE_MODE){
				/* If the capacity is already at max, return NULL*/
				if (pBD->capacity == SHRT_MAX)
					return NULL;
				else {
					/*trying to increase buffer capacity using the given formula */
					availableSpace = SHRT_MAX - pBD->capacity;
					newIncrement = (short)(availableSpace * ((double)b_inc_factor(pBD)/MULTIPLICATIVE_DIVISOR));
					/*As the capacity approaches shrt_max, the newIncrement formula needs a bit of a push, because it will approach zero before the capacity can reach max
					so if newIncrement is zero, give it a push by making it equal to 1 */
					if (newIncrement == 0)
						newIncrement=1;
					
					/*If the available space for buffer expansion is smaller than required increased, 
					overflow has occured meaning we don't have enough room for the new increment. 
					But we still need more space, so set capacity to the max buffer size*/
					if ((availableSpace < newIncrement)){
						pBD->capacity = SHRT_MAX;
					} else {
						pBD->capacity += newIncrement;
					}
				}
			} /*end multiplicative mode check */
			/*realloc into a temp dyanamic array to avoid losing data on a realloc error */
			tempBuff = (char *)realloc(pBD->cb_head, pBD->capacity);
			if (tempBuff == NULL){
				return NULL;
			}
			if (pBD->cb_head != tempBuff){
				pBD->r_flag = SET_R_FLAG; /*set realloc flag*/
            }
			pBD->cb_head = tempBuff; /*assign the temp dynamic array*/
			pBD->cb_head[pBD->addc_offset] = symbol; /*Store the char into the dynamic array */
			pBD->addc_offset ++; /*increment the add char offset */
			return pBD; /* return the pointer to the buffer */
		} /*end of full operation */
	} else {
		return NULL; /*Return NUll if the passed pointer to the buffer is invalid */
	}
	return NULL; /* Satisfies "not all control paths return a value" warning. Unreachable code */
}

/*
Purpose: The function retains the memory space allocated to the buffer at the moment, but re-initializes all appropriate data members.
Author: Daniel Spagnuolo
History/Versions: v1: Sept. 29, 2015
Called functions: None.
Parameters: Buffer *pBD
Return value: Return fail flag on failure.
*/
int b_reset (Buffer * const pBD){
	if (pBD){
		pBD->addc_offset=0;
		pBD->getc_offset=0;
		pBD->mark_offset=0;
		pBD->eob=0;

	} else {
		/*Problem with buffer pointer */
		return R_FAIL_1;
	}
	return 0;
}

/*
Purpose: The function de-allocates (frees) the memory occupied by the character buffer and the Buffer structure
Author: Daniel Spagnuolo
History/Versions: v1: Sept. 29, 2015
Called functions: None.
Parameters: Buffer *pBD
Return value: void.
*/
void b_destroy(Buffer * const pBD){
	if (pBD){
		/*Closes memory leaks */
		if (pBD->cb_head){
			free(pBD->cb_head);
		}
		free(pBD);
	}
}

/*
Purpose: The function returns 1 if the character buffer is full; it returns 0 otherwise
Author: Daniel Spagnuolo
History/Versions: v1: Sept. 29, 2015
Called functions: None.
Parameters: Buffer *pBD
Return value: 1 if the character buffer is full; it returns 0 otherwise
Algorithm:	-if capacity - the add character offset is bigger than zero, the buffer is not full
			-if capacity - the add character offset is zero, the buffer is full
*/
int b_isfull (Buffer * const pBD){
	if (pBD){
		short remainingSize = pBD->capacity - pBD->addc_offset;
		if (remainingSize > 0){
			return 0; /*not full*/
		} else if (remainingSize == 0){
			return 1; /*full*/
		} else {
			/*Problem with buffer */
			return R_FAIL_1;
		}
	} else {
		/*Problem with buffer pointer */
		return R_FAIL_1;
	}
}

/*
Purpose: The function returns the current size of the character buffer.
Author: Daniel Spagnuolo
History/Versions: v1: Sept. 29, 2015
Called functions: None.
Parameters: Buffer *pBD
Return value: add char offset in bytes or fail flag (-1)
*/
short b_size (Buffer * const pBD){
	if(pBD){
		return pBD->addc_offset;
	} else {
		/*Problem with buffer pointer */
		return R_FAIL_1;
	}
}

/*
Purpose: The function returns the current capacity of the character buffer.
Author: Daniel Spagnuolo
History/Versions: v1: Sept. 29, 2015
Called functions: None.
Parameters: Buffer *pBD
Return value: add char offset in bytes or fail flag (-1)
*/
short b_capacity(Buffer * const pBD){
	if (pBD){
		return pBD->capacity;
	} else {
		/*Problem with buffer pointer */
		return R_FAIL_1;
	}
}

/*
Purpose: The function sets mark_offset to mark.
Author: Daniel Spagnuolo
History/Versions: v1: Sept. 29, 2015
Called functions: None.
Parameters: Buffer *pBD, short mark
Return value: returns a pointer to the currently set mark_offset location in the buffer character array.
			Returns NULL on run-time error
*/
char * b_setmark (Buffer * const pBD, short mark){
	/*Check that buffer pointer is valid, and that mark is within 0 to addc_offset inclusive */
	if (pBD && (mark >= 0 && mark <= pBD->addc_offset)){
		pBD->mark_offset = mark; /*Assign mark to the buffer mark member */
		return &pBD->cb_head[pBD->mark_offset]; /*return character at mark */
	} else {
		/*param error */
		return NULL;
	}
}

/*
Purpose: The function returns mark_offset
Author: Daniel Spagnuolo
History/Versions: v1: Sept. 29, 2015
Called functions: None.
Parameters: Buffer *pBD
Return value: The function returns mark_offset
			Returns fail flag on run-time error
*/
short b_mark (Buffer * const pBD){
	if(pBD){
		return pBD->mark_offset;
	} else {
		/*buff pointer error */
		return R_FAIL_1;
	}
}

/*
Purpose: The function returns mode
Author: Daniel Spagnuolo
History/Versions: v1: Sept. 29, 2015
Called functions: None.
Parameters: Buffer *pBD
Return value: The function returns mode
			Returns fail flag on run-time error
*/
int b_mode (Buffer * const pBD){
	if(pBD){
		return pBD->mode;
	} else {
		/*buff pointer error */
		/*return R_FAIL_1;*/
		return R_FAIL_2;
	}
}

/*
Purpose: The function returns inc_factor
Author: Daniel Spagnuolo
History/Versions: v1: Sept. 29, 2015
Called functions: abs()
Parameters: Buffer *pBD
Return value: The function returns inc_factor
			Returns 256 on run-time error
*/
size_t b_inc_factor (Buffer * const pBD){
	if(pBD){
		/* return positive value of inc factor */
		return abs(pBD->inc_factor);
	} else {
		/*buff pointer error */
		return SIZE_T_MAX;
	}
}

/*
Purpose: The function loads (reads) an open input file specified by fi into the buffer
Author: Daniel Spagnuolo
History/Versions: v1: Sept. 29, 2015
Called functions: fgetc(), b_addc()
Parameters: FILE fi, Buffer *pBD
Return value: The function returns number of chairs read
			Returns fail flag on run-time error
Algorithm:	-While the read char is not an EOF character,
			-add the character to the buffer, checking for null and incrementing numchars 
*/
int b_load (FILE * const fi, Buffer * const pBD){
	char c = 0; /*Holds the current char */
	unsigned int numChars = 0; /*Number of chars read */
	if (pBD){ /*check buffer pointer */
		while(c!=EOF){ /*Loops until the char is an EOF char */
			c = (char)fgetc(fi); /*Assign next char to c */
			if (c != EOF){ /*Only add to buffer if the character is not an EOF character. Second check since fgetc is within loop*/
				if(b_addc(pBD,c)==NULL){return LOAD_FAIL;} /* return if read character is NULL */
				numChars++; /*increment numchars */
			}
		}
		return numChars; /*return numchars */
	} else {
		/*buff pointer error */
		return R_FAIL_1;
	}
}

/*
Purpose: The function checks to see if the buffer is empty
Author: Daniel Spagnuolo
History/Versions: v1: Sept. 29, 2015
Called functions: none
Parameters: Buffer *pBD
Return value: The function returns 1 if full,
				0 if empty. fail flag if buffer pointer error.
*/
int b_isempty (Buffer * const pBD){
	if(pBD){
		if (pBD->addc_offset == 0){
			/*full */
			return 1;
		} else {
			/*empty*/
			return 0;
		}
	} else {
		/*buff pointer error */
		return R_FAIL_1;
	}
}

/*
Purpose: The function returns the end of buffer member
Author: Daniel Spagnuolo
History/Versions: v1: Sept. 29, 2015
Called functions: none
Parameters: Buffer *pBD
Return value: The function returns 1 if full,
				0 if empty. fail flag if buffer pointer error.
*/
int b_eob (Buffer * const pBD){
	if(pBD){
		return pBD->eob;
	} else {
		/*buff pointer error */
		return R_FAIL_1;
	}
}

/*
Purpose: The function returns the character located at getc_offset.
Author: Daniel Spagnuolo
History/Versions: v1: Sept. 29, 2015
Called functions: none
Parameters: Buffer *pBD
Return value: The function returns fail flag (-1) if end of buffer has been reached
				else it returns the char at getc_offset
				Returns fail flag (-2) if there is a buffer pointer error
*/
char b_getc (Buffer * const pBD){
	if(pBD){
		if (pBD->getc_offset == pBD->addc_offset){ /*end of buffer reached */
			pBD->eob=1; /*set eob flag */
			return R_FAIL_1; 
		} else if(pBD->getc_offset < pBD->addc_offset)  {
			pBD->eob=0; /*set eob flag */
			++pBD->getc_offset; /*increment getc offset */
			return pBD->cb_head[pBD->getc_offset-1]; /* -1 because of getc_offset increment on previous line */
		}
	} else {
		/*buff pointer error */
		return R_FAIL_2;
	}
	return R_FAIL_2; /* Satisfyies "not all control paths return a value" warning. Unreachable code*/
}

/*
Purpose: The function prints the contents of the buffer.
Author: Daniel Spagnuolo
History/Versions: v1: Sept. 29, 2015
Called functions: b_size(), b_getc(), printf()
Parameters: Buffer *pBD
Return value: The function returns number of chars printed
				Returns fail flag (-1) if there is a buffer pointer error
Algorithm:	-Checks to see if buffer is not empty. If so, loop through dynamic array, printing
			each character by calling b_getc and keeping count of number of chars printed.
			-Notice the user if the buffer is full
*/
int b_print (Buffer * const pBD){
	unsigned int numChars = 0; /*For counting number of chars printed*/
	short i=0; /*For looping through buffer */
	char character = 0; /*for holding character returned by get character function */

	if (pBD){
		if (!b_isempty(pBD)){ /*If buffer is not empty, proceed*/
			pBD->eob = 0;
			pBD->getc_offset = 0; /*reset get char offset */
			for (i = 0; i < b_size(pBD); i++){ /*loop through buffer */
				character = b_getc(pBD); /*Assign fetched character to the variable character */
				printf("%c", character); /*print character */
				numChars ++; /* increment numchars */
			}
			pBD->getc_offset = 0; /*reset get char offset after loop */
			printf("\n"); /*print newline char */
			return numChars; /*return num of printed chars */
		} else { /*Buffer is empty */
			printf("The buffer is empty.\n");
			pBD->getc_offset = 0;
			return numChars; /*return num of printed chars */
		}
	}else {
		/*buff pointer error */
		return R_FAIL_1;
	}
}

/*
Purpose: the function shrinks (or in some cases may expand) the buffer to a new capacity
Author: Daniel Spagnuolo
History/Versions: v1: Sept. 29, 2015
Called functions: realloc()
Parameters: Buffer *pBD
Return value: The function returns number of pointer to buffer
				Returns NULL if there is a buffer pointer error
Algorithm:	-The function uses realloc() to adjust the new capacity, and 
			then updates all the necessary members of the buffer descriptor structure
*/
Buffer *b_pack(Buffer * const pBD){
	char *temp; /* Temp buff pointer for realloc */
	if (pBD){ /*check if buffer pointer is valid */
		if(pBD->capacity == SHRT_MAX){return NULL;} /*if capacity is already at max, return NULL*/
		/*realloc into a temp dyanamic array to avoid losing data on a realloc error */
		temp = (char *)realloc(pBD->cb_head, (pBD->addc_offset +1) * (sizeof(char)));
		if (temp){ /*if temp realloc is valid */
			if (pBD->cb_head != temp){
				pBD->r_flag = SET_R_FLAG; /*set realloc flag*/
            }
			pBD->cb_head = temp; /* assign temp to the original dynamic array */
			pBD->capacity = pBD->addc_offset+1; /* Capacity is now the add character offset +1 */
			return pBD; /*return pointer to buffer */
		} else{ /*temp is invalid */
			return NULL;
		}
	} else {
		/*buff pointer error */
		return NULL;
	}
}

/*
Purpose: The function returns r_flag
Author: Daniel Spagnuolo
History/Versions: v1: Sept. 29, 2015
Called functions: none
Parameters: Buffer *pBD
Return value: The function returns r_flag
				Returns fail flag (-1) if there is a buffer pointer error
*/
char b_rflag (Buffer * const pBD){
	if(pBD){
		return pBD->r_flag; 
	} else {
		/*buff pointer error */
		return R_FAIL_1;
	}
}

/*
Purpose: The function decrements getc_offset by 1
Author: Daniel Spagnuolo
History/Versions: v1: Sept. 29, 2015
Called functions: none
Parameters: Buffer *pBD
Return value: The function returns getc_offset
				Returns fail flag (-1) if there is a buffer pointer error
*/
short b_retract (Buffer * const pBD){
	if(pBD && pBD->getc_offset > 0){
		pBD->getc_offset --; /*decrements getc_offset by 1*/
		return pBD->getc_offset; 
	} else {
		/*buff pointer error */
		return R_FAIL_1;
	}
}

/*
Purpose: The function sets getc_offset to the value of the current mark_offset
Author: Daniel Spagnuolo
History/Versions: v1: Sept. 29, 2015
Called functions: none
Parameters: Buffer *pBD
Return value: The function returns getc_offset
				Returns fail flag (-1) if there is a buffer pointer error
*/
short b_retract_to_mark (Buffer * const pBD){
	if(pBD){
		/* assign mark to the get char offset  */
		pBD->getc_offset = pBD->mark_offset;
		return pBD->getc_offset;
	}else {
		/*buff pointer error */
		return R_FAIL_1;
	}
}

/*
Purpose: The function returns getc_offset
Author: Daniel Spagnuolo
History/Versions: v1: Sept. 29, 2015
Called functions: none
Parameters: Buffer *pBD
Return value: The function returns getc_offset
				Returns fail flag (-1) if there is a buffer pointer error
*/
short b_getc_offset (Buffer * const pBD){
	if (pBD){
		return pBD->getc_offset;
	} else  {
		/*buff pointer error */
		return R_FAIL_1;
	}
}

