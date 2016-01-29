/* File Name:	parser.h
 * Version:		1
   Compiler:	MS Visual Studio 2012
   Author:		Daniel Spagnuolo, 040 768 219
   Course:		CST 8152 – Compilers, Lab Section: 012
   Assignment:	4
   Date:		Dec. 11 2015
   Professor:	Sv. Ranev
   Purpose:		integrate the Parser with your existing lexical analyzer and 
 symbol table in order to complete the front-end of your PLATYPUS compiler
 */

#ifndef PARSER_H
#define PARSER_H

#include "buffer.h"
#include "token.h"
#include "stable.h"
#include <stdlib.h>

/* DEFINES */
#define NO_ATTR 0

#define ELSE 0
#define IF 1
#define INPUT 2
#define OUTPUT 3
#define PLATYPUS 4
#define REPEAT 5
#define THEN 6
#define USING 7 

/* Globals */
static Token lookahead;
static Buffer* sc_buf;
int synerrno;
extern STD sym_table;
extern pBuffer str_LTBL;
extern int line;
extern char * kw_table [];

/* function declerations */
void parser(Buffer * in_buf);
void match(int pr_token_code,int pr_token_attribute);
void syn_eh(int sync_token_code);
void syn_printe();
void gen_incode(char *);
void program();
Token mlwpar_next_token(Buffer *);

void program(void);
void opt_statements(void);
void statements(void);
void statement(void);
void statements_p(void);
void assignment_statement(void);
void assignment_expression(void);
void selection_statement(void);
void iteration_statement(void);
void input_statement(void);
void variable_list(void);
void variable_list_p(void);
void variable_identifier(void);
void output_statement(void);
void output_list(void);
void opt_variable_list(void);
void arithmetic_expression(void);
void unary_arithmetic_expression(void);
void additive_arithmetic_expression(void);
void additive_arithmetic_expression_p(void);
void multiplicative_arithmetic_expression(void);
void multiplicative_arithmetic_expression_p(void);
void primary_arithmetic_expression(void);
void primary_arithmetic_expression_p(void);
void string_expression(void);
void string_expression_p(void);
void primary_string_expression(void);
void conditional_expression(void);
void logical_OR_expression(void);
void logical_OR_expression_p(void);
void logical_AND_expression(void);
void logical_AND_expression_p(void);
void relational_expression(void);
void primary_a_relational_expression(void);
void primary_a_relational_expression_p(void);
void primary_s_relational_expression(void);
void primary_s_relational_expression_p(void);

#endif
