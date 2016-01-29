/*
 * File name:		parser.c
 * Compiler:		MS Visual Studio 2012
 * Authors:			Daniel Spagnuolo		040 768 219
 * Course:			CST 8152 – Compilers, Lab Section: 012
 * Assignment:		4 - implement and incorporate a symbol table component in your PLATYPUS compiler
 * Date:			11 December 2015
 * Professor:		Svillen Ranev
 * Purpose:			integrate the Parser with your existing lexical analyzer and 
 symbol table in order to complete the front-end of your PLATYPUS compiler
*/

#include "parser.h"

/*
 * Purpose:				Parses the code contained in a buffer
 * Author:				Daniel Spagnuolo
 * Versions:			v1.0, 11 December 2015
 * Parameters:			The buffer containing the source file
 * Return Value:		void
 * Algorithm:			Initiates the token matching.
 * Called Functions:	mlwpar_next_token()
						program()
						match()
						gen_incode()
 
*/
void parser(Buffer * in_buf){
 sc_buf = in_buf;
 lookahead = mlwpar_next_token(sc_buf);
 program(); match(SEOF_T,NO_ATTR);
 gen_incode("PLATY: Source file parsed");
}/*
 * Purpose:				Matches the lookahead token with the expected token
 * Author:				Daniel Spagnuolo
 * Versions:			v1.0, 11 December 2015
 * Parameters:			1, The expected token
						2, the expected attribute of the token
 * Return Value:		void
 * Called Functions:	syn_eh()
						mlwpar_next_token()
 * Algorith: matches the given token to the lookahead token
 
*/void match(int pr_token_code,int pr_token_attribute){	/*aumatic error*/	if (lookahead.code != pr_token_code) {
		syn_eh(pr_token_code);
		return;
	}		/*match token codes*/	if (pr_token_code == KW_T){		if (pr_token_attribute == lookahead.attribute.kwt_idx){
			lookahead = mlwpar_next_token(sc_buf);
		} else {
			syn_eh(pr_token_code);
		}	} else if (pr_token_code == LOG_OP_T){		if (pr_token_attribute == lookahead.attribute.log_op) {
			lookahead = mlwpar_next_token(sc_buf);
		} else {
			syn_eh(pr_token_code);
		}	} else if (pr_token_code == ART_OP_T){		if (pr_token_attribute == lookahead.attribute.arr_op) {
			lookahead = mlwpar_next_token(sc_buf);
		} else {
			syn_eh(pr_token_code);
		}	} else if (pr_token_code == REL_OP_T){		if (pr_token_attribute == lookahead.attribute.rel_op) {
			lookahead = mlwpar_next_token(sc_buf);
		} else {
			syn_eh(pr_token_code);
		}	} else {		if (lookahead.code == SEOF_T){
			return;}
		lookahead = mlwpar_next_token(sc_buf);	}	if (lookahead.code == ERR_T){
		syn_printe();
		lookahead = mlwpar_next_token(sc_buf);
		synerrno++;
		return;
	}}/*
 * Purpose:				Handles a panic mode error recovery
 * Author:				Daniel Spagnuolo
 * Versions:			v1.0, 21 November 2015
 * Parameters:			Code of safe token
 * Return Value:		void
 * Called Functions:	syn_printe()
						mlwpar_next_token()
						exit()
 
*/void syn_eh(int sync_token_code){	syn_printe();	synerrno++;	if (sync_token_code == SEOF_T){
		return;
	}	while(1) {
		lookahead = mlwpar_next_token(sc_buf);
		if(lookahead.code == sync_token_code){
			lookahead = mlwpar_next_token(sc_buf);
			return;
		}

		else if(lookahead.code == SEOF_T)
			exit(synerrno);
	}}

/*
 * Purpose: Prints out an error found by the parser
 * Author: Svillen Ranev
*/
void syn_printe(){
	Token t = lookahead;

	printf("PLATY: Syntax error:  Line:%3d\n",line);
	printf("*****  Token code:%3d Attribute: ", t.code);
	switch(t.code){
	case  ERR_T: /* ERR_T     0   Error token */
		printf("%s\n",t.attribute.err_lex);
		break;
	case  SEOF_T: /*SEOF_T    1   Source end-of-file token */
		printf("NA\n" );
		break;
	case  AVID_T: /* AVID_T    2   Arithmetic Variable identifier token */
	case  SVID_T :/* SVID_T    3  String Variable identifier token */
		printf("%s\n",sym_table.pstvr[t.attribute.get_int].plex);
		break;
	case  FPL_T: /* FPL_T     4  Floating point literal token */
		printf("%5.1f\n",t.attribute.flt_value);
		break;
	case INL_T: /* INL_T      5   Integer literal token */
		printf("%d\n",t.attribute.get_int);
		break;
	case STR_T:/* STR_T     6   String literal token */
		printf("%s\n",b_setmark(str_LTBL,t.attribute.str_offset));
		break;

	case SCC_OP_T: /* 7   String concatenation operator token */
		printf("NA\n" );
		break;

	case  ASS_OP_T:/* ASS_OP_T  8   Assignment operator token */
		printf("NA\n" );
		break;
	case  ART_OP_T:/* ART_OP_T  9   Arithmetic operator token */
		printf("%d\n",t.attribute.get_int);
		break;
	case  REL_OP_T: /*REL_OP_T  10   Relational operator token */ 
		printf("%d\n",t.attribute.get_int);
		break;
	case  LOG_OP_T:/*LOG_OP_T 11  Logical operator token */
		printf("%d\n",t.attribute.get_int);
		break;

	case  LPR_T: /*LPR_T    12  Left parenthesis token */
		printf("NA\n" );
		break;
	case  RPR_T: /*RPR_T    13  Right parenthesis token */
		printf("NA\n" );
		break;
	case LBR_T: /*    14   Left brace token */
		printf("NA\n" );
		break;
	case RBR_T: /*    15  Right brace token */
		printf("NA\n" );
		break;

	case KW_T: /*     16   Keyword token */
		printf("%s\n",kw_table [t.attribute.get_int]);
		break;

	case COM_T: /* 17   Comma token */
		printf("NA\n");
		break;
	case EOS_T: /*    18  End of statement *(semi - colon) */
		printf("NA\n" );
		break; 		
	default:
	    printf("PLATY: Scanner error: invalid token code: %d\n", t.code);
    }/*end switch*/
}/* end syn_printe()*/

/*
 * Purpose: prints intermediate code
 * Author:				Daniel Spagnuolo
 * Versions:			v1.0, 21 November 2015
 * Parameters:			Code of safe token
 * Return Value:		void
 * Parameters:	code - A string of code that is added to the generated code
*/
void gen_incode(char * code){
	printf("%s\n", code);
}

/* -------------------------------------------------Production Functions------------------------------- */

/*
 * PRODUCTION: <program> -> PLATYPUS {<opt_statements>} 
 *FIRSTS: PLATYPUS
*/
void program(void){
	match(KW_T, PLATYPUS);match(LBR_T,NO_ATTR);opt_statements();
	match(RBR_T,NO_ATTR);
	gen_incode("PLATY: Program parsed");
}/*
 * PRODUCTION: <opt_statements> -> <statements> | empty 
 * FIRSTS: AVID_T, SVID_T, IF, USING, INPUT, OUTPUT, empty
*/void opt_statements(void){	int laCode = lookahead.code;	int laAttr = lookahead.attribute.int_value;	/*The FIRSTS for opt_statements*/
	if ( laCode == AVID_T 
		|| laCode == SVID_T 
		|| laCode == KW_T 
		&& (laAttr == IF 
		|| laAttr == USING 
		|| laAttr == INPUT 
		|| laAttr == OUTPUT) )
	{
		statements();
	}
	/*empty*/
	else {
		gen_incode("PLATY: Opt_statements parsed");	
	}}
/*
 * PRODUCTION: <statements> -> <statement><statements'> 
 * FIRSTS: AVID_T, SVID_T, IF, USING, INPUT, OUTPUT
*/void statements(void){	statement();
	statements_p();}/* PRODUCTION: <statements'> -> <statement><statements'> | empty
 * FIRSTS: AVID_T, SVID_T, IF, USING, INPUT, OUTPUT
*/void statements_p(void){	int laCode = lookahead.code;	int laAttr = lookahead.attribute.int_value;	if ( laCode == AVID_T 
		|| laCode == SVID_T 
		|| laCode == KW_T 
		&& (laAttr == IF 
		|| laAttr == USING 
		|| laAttr == INPUT 
		|| laAttr == OUTPUT) )
	{
		statement();
		statements_p();
	}}/*
 * PRODUCTION: <statement> -> 
 <assignment statement> | <selection statement> | <iteration statement> | <input statement> | <output statement>
 * FIRSTS: AVID_T, SVID_T, IF, USING, INPUT, OUTPUT
*/void statement(void){	int laCode = lookahead.code;	int laAttr = lookahead.attribute.int_value;
	if (laCode == AVID_T || laCode == SVID_T){/*Assignment statement*/
		assignment_statement();
	} 
	else if (laAttr == IF) {/*selection statement*/
		selection_statement();
	}	
	else if (laAttr == USING){/*iteration statement*/
		iteration_statement();
	}
	else if (laAttr == INPUT){/*input statement*/
		input_statement();
	}
	else if (laAttr == OUTPUT){/*output statement*/
		output_statement();
	}
	else {/*Error*/
		syn_printe();
		return;
	}}/*
 * PRODUCTION: <assignment statement> -> <assignment expression>
 * FIRSTS: AVID_T, SVID_T
*/void assignment_statement(void){
	assignment_expression();
	match(EOS_T, NO_ATTR);
	gen_incode("PLATY: Assignment statement parsed");
}/*
 * PRODUCTION: <assignment expression> -> 
	AVID = <arithmetic expression>  | SVID = <arithmetic expression>
 * FIRSTS: AVID_T, SVID_T
*/void assignment_expression(void){	int laCode = lookahead.code;
	if (laCode == AVID_T) {/*AVID*/
		match(AVID_T, NO_ATTR);match(ASS_OP_T, NO_ATTR);
		arithmetic_expression();
		gen_incode("PLATY: Assignment expression (arithmetic) parsed");
	}
	else if (laCode == SVID_T){/*SVID*/
		match(SVID_T, NO_ATTR);match(ASS_OP_T, NO_ATTR);
		string_expression();
		gen_incode("PLATY: Assignment expression (string) parsed");
	}
	else {/*Error*/
		syn_printe();
	}}/*
 * PRODUCTION: <selection statement> -> 
 * IF (<conditional expression>) THEN <opt_statements> ELSE {<opt_statements>};
 * FIRSTS: IF
*/void selection_statement(void){	match(KW_T, IF);match(LPR_T, NO_ATTR);
	conditional_expression();
	match(RPR_T, NO_ATTR);match(KW_T, THEN);
	opt_statements();
	match(KW_T, ELSE);match(LBR_T, NO_ATTR);
	opt_statements();
	match(RBR_T, NO_ATTR);match(EOS_T, NO_ATTR);
	gen_incode("PLATY: IF statement parsed");}
/*
 * PRODUCTION: <iteration statement> -> 
 USING (<assignment expression>,<conditional expression>,<assignment expression>)
 REPEAT { <opt_statements> };
 * FIRSTS: USING
*/void iteration_statement(void){
	match(KW_T, USING);match(LPR_T, NO_ATTR);
	assignment_expression();
	match(COM_T, NO_ATTR);
	conditional_expression();
	match(COM_T, NO_ATTR);
	assignment_expression();
	match(RPR_T, NO_ATTR);match(KW_T, REPEAT);match(LBR_T, NO_ATTR);
	opt_statements();
	match(RBR_T, NO_ATTR);match(EOS_T, NO_ATTR);
	gen_incode("PLATY: USING statement parsed");
}

/*
 * PRODUCTION: <input statement> -> INPUT(<variable list>);
 * FIRSTS: INPUT
*/
void input_statement(void){
	match(KW_T, INPUT);	match(LPR_T, NO_ATTR);
	variable_list();
	match(RPR_T, NO_ATTR);match(EOS_T, NO_ATTR);
	gen_incode("PLATY: INPUT statement parsed");
}

/*
 * PRODUCTION: <variable list> -> <variable identifier><variable list'> 
 * FIRSTS: AVID_T, SVID_T
*/
void variable_list(void){
	variable_identifier();
	variable_list_p();
	gen_incode("PLATY: Variable list parsed");
}

/*
 * PRODUCTION: <variable list1> -> ,<variable identifier><variable list'> | empty 
 * FIRSTS: , empty
*/
void variable_list_p(void){
	if (lookahead.code != COM_T)/*empty*/
		return;
	
	match(COM_T, NO_ATTR);
	variable_identifier();
	variable_list_p();
}

/*
 * PRODUCTION: <variable identifier> -> AVID_T | SVID_T
 * FIRSTS: AVID_T, SVID_T
*/
void variable_identifier(void){
	int laCode = lookahead.code;
	if (laCode == AVID_T){
		match(AVID_T, NO_ATTR);
	}
	else if (laCode == SVID_T){
		match(SVID_T, NO_ATTR);
	}
	else {
		syn_printe();
	}
}

/*
 * PRODUCTION: <output statement> -> OUTPUT(<output_list>);
 * FIRSTS: OUTPUT
*/
void output_statement(void){
	match(KW_T, OUTPUT);match(LPR_T, NO_ATTR);
	output_list();
	match(RPR_T, NO_ATTR);match(EOS_T, NO_ATTR);
	gen_incode("PLATY: OUTPUT statement parsed");
}

/*
 * PRODUCTION: <output_list> -> <opt_variable list> | STR_T
 * FIRSTS: AVID_T, SVID_T, STR_T, empty
*/
void output_list(void){
	/*String literal*/
	if (lookahead.code == STR_T){
		match(STR_T, NO_ATTR);
		gen_incode("PLATY: Output list (string literal) parsed");
	}
	/*Optional Variable list*/
	else {
		opt_variable_list();
	}
}

/*
 * PRODUCTION: <opt_variable list> -> <variable list> | empty
 * FIRSTS: AVID_T, SVID_T, empty
*/
void opt_variable_list(void){
	/*variable list*/
	if (lookahead.code == AVID_T || lookahead.code == SVID_T){
		variable_list();
	}
	/*empty*/
	else {
		gen_incode("PLATY: Output list (empty) parsed");
	}
}

/*
 * PRODUCTION: <arithmetic expression> -> 
	<unary arithmetic expression> | <additive arithmetic expression>
 * FIRSTS: +, -, AVID_T, FPL_T, INL_T, LPR_T
*/
void arithmetic_expression(void){
	int laCode = lookahead.code;
	Arr_Op laAttr = lookahead.attribute.arr_op;

	/*unary arithmetic expression*/
	if (laCode == ART_OP_T && (laAttr == PLUS || laAttr == MINUS)){
		unary_arithmetic_expression();
	}
	/*additive arithmetic expression*/
	else if (laCode == AVID_T || laCode == FPL_T || laCode == INL_T || laCode == LPR_T){
		additive_arithmetic_expression();
	}
	else {/*Otherwise we have an error*/
		syn_printe();
		return;
	}
}

/*
 * PRODUCTION: <unary arithmetic expression> -> 
	- <primary arithmetic expression> | + <primary arithmetic expression>
 * FIRSTS: +, -
*/
void unary_arithmetic_expression(void){
	
	if (lookahead.attribute.arr_op == MINUS){
		match(ART_OP_T, MINUS);
	}
	else if (lookahead.attribute.arr_op == PLUS){
		match(ART_OP_T, PLUS);
	}
	primary_arithmetic_expression();
	gen_incode("PLATY: Unary arithmetic expression parsed");
	gen_incode("PLATY: Arithmetic expression parsed");
}

/*
 * PRODUCTION: <additive arithmetic expression> -> 
	<multiplicative arithmetic expression><additive arithmetic expression'>
 * FIRSTS: AVID_T, INL_T, FPL_T, LPR_T
*/
void additive_arithmetic_expression(void){
	multiplicative_arithmetic_expression();
	additive_arithmetic_expression_p();
	gen_incode("PLATY: Arithmetic expression parsed");
}

/*
 * PRODUCTION: <additive arithmetic expression'> ->   
					  + <multiplicative arithmetic expression><additive arithmetic expression'>
					| - <multiplicative arithmetic expression><additive arithmetic expression'>
					| empty
 * FIRSTS: +, -, empty
*/
void additive_arithmetic_expression_p(void){
	Arr_Op laAttr = lookahead.attribute.arr_op;

	/*Match the two possible cases*/
	if (lookahead.code == ART_OP_T){
		if (laAttr == MINUS){
			match(ART_OP_T, MINUS);
		}
		else if (laAttr == PLUS){
			match(ART_OP_T, PLUS);
		}
		else {
			return;
		}

		/*Now do the rest*/
		multiplicative_arithmetic_expression();
		additive_arithmetic_expression_p();
		gen_incode("PLATY: Additive arithmetic expression parsed");
	}
	else{
	/*empty*/
	}
}

/*
 * PRODUCTION: <multiplicative arithmetic expression> -> 
					<primary arithmetic expression><multiplicative arithmetic expression'>	
 * FIRSTS: AVID_T, FPL_T, INL_T, LPR_T
*/
void multiplicative_arithmetic_expression(void){
	primary_arithmetic_expression();
	multiplicative_arithmetic_expression_p();
}

/*
 * PRODUCTION: <multiplicative arithmetic expression'> ->   
					  * <primary arithmetic expression><multiplicative arithmetic expression'>
					| / <primary arithmetic expression><multiplicative arithmetic expression'>
					| empty
 * FIRSTS: *, /
*/
void multiplicative_arithmetic_expression_p(void){
	Arr_Op laAttr = lookahead.attribute.arr_op;

	/*Match one of the two possible cases*/
	if (lookahead.code == ART_OP_T){
		if (laAttr == MULT){
			match(ART_OP_T, MULT);
		}
		else if (laAttr == DIV){
			match(ART_OP_T, DIV);
		}
		else {
			return;
		}

		/*Now do the rest*/
		primary_arithmetic_expression();
		multiplicative_arithmetic_expression_p();
		gen_incode("PLATY: Multiplicative arithmetic expression parsed");
	}
	else{
	/*empty*/
	}
}


/*
 * PRODUCTION: <primary arithmetic expression> -> 
					AVID_T | FPL_T | INL_T | (<arithmetic expression>)	
 * FIRSTS: AVID_T, FPL_T, INL_T, LPR_T
*/
void primary_arithmetic_expression(void){
	int laCode = lookahead.code;

	if (laCode == AVID_T){
		match(AVID_T, NO_ATTR);
	}
	else if (laCode == FPL_T){
		match(FPL_T, NO_ATTR);
	}
	else if (laCode == INL_T){
		match(INL_T, NO_ATTR);
	}
	else if (laCode == LPR_T){
		match(LPR_T, NO_ATTR);
		arithmetic_expression();
		match(RPR_T, NO_ATTR);
	}
	else {
		syn_printe();
		return;
	}
	gen_incode("PLATY: Primary arithmetic expression parsed");
}

/*
 * PRODUCTION: <string expression> ->
					<primary string expression><string expression'>
 * FIRSTS: SVID_T, STR_T
*/
void string_expression(void){
	primary_string_expression();
	string_expression_p();
	gen_incode("PLATY: String expression parsed");
}


/*
 * PRODUCTION: <string expression'> -> 
					# <primary string expression><string expression'> | empty
 * FIRSTS: SCC_OP_T
*/
void string_expression_p(void){
	/*String concatenation*/
	if (lookahead.code == SCC_OP_T){
		match(SCC_OP_T, NO_ATTR);
		primary_string_expression();
		string_expression_p();
	}
	else{
	/*do nothing for empty*/
	}
}


/*
 * PRODUCTION: <primary string expression> -> SVID_T | STR_T	
 * FIRSTS: SVID_T, STR_T
*/
void primary_string_expression(void){
	int laCode = lookahead.code;

	if (laCode == SVID_T){/*SVID*/
		match(SVID_T, NO_ATTR);
	}
	else if (laCode == STR_T){/*STRING LITERAL*/
		match(STR_T, NO_ATTR);
	}
	else { /*NO MATCH*/
		syn_printe();
		return;
	}
	gen_incode("PLATY: Primary string expression parsed");
}

/*
 * PRODUCTION: <conditional expression> -> <logical OR expression>
 * FIRSTS: AVID_T, FPL_T, INL_T
*/
void conditional_expression(void){
	logical_OR_expression();
	gen_incode("PLATY: Conditional expression parsed");
}


/*
 * PRODUCTION: <logical or expression> -> <logical AND expression> <logical OR expression'>	
 * FIRSTS: AVID_T, FPL_T, INL_T
*/
void logical_OR_expression(void){
	logical_AND_expression();
	logical_OR_expression_p();

}

/*
 * PRODUCTION: <logical or expression'> -> 
					.OR. <logical AND expression> <logical OR expression'> | empty		
 * FIRSTS: .OR., empty
*/
void logical_OR_expression_p(void){
	if (lookahead.code != LOG_OP_T || lookahead.attribute.log_op != OR)
		return;

	match(LOG_OP_T,OR);
	logical_AND_expression();
	logical_OR_expression_p();
	gen_incode("PLATY: Logical OR expression parsed");
}

/*
 * PRODUCTION: <logical AND expression> -> <relational expression> <logical AND expression'>
 * FIRSTS: AVID_T, INL_T, FPL_T
*/
void logical_AND_expression(void){
	relational_expression();
	logical_AND_expression_p();
}

/*
 * PRODUCTION: <logical AND expression'> -> 
					.AND. <relational expression> <logical OR expression'> |empty
 * FIRSTS: .AND., empty
*/
void logical_AND_expression_p(void){
	
	/*AND*/
	if (lookahead.code == LOG_OP_T && lookahead.attribute.log_op == AND){
		match(LOG_OP_T, AND);
		relational_expression();
		logical_AND_expression_p();
		gen_incode("PLATY: Logical AND expression parsed");
	}
	else {
	/*empty*/
	}
}

/*
 * PRODUCTION: <relational expression> -> 
					<primary a_relational expression><primary a_relational expression'>
					| <primary s_relational expression><primary s_relational expression'>
 * FIRSTS: AVID_T, INL_T, FPL_T
*/
void relational_expression(void){
	int laCode = lookahead.code;

	/* A_RELATIONAL*/
	if (laCode == AVID_T || laCode == FPL_T || laCode == INL_T){
		primary_a_relational_expression();
		primary_a_relational_expression_p();
	}
	/* S_RELATIONAL*/
	else if (laCode == SVID_T || laCode == STR_T){
		primary_s_relational_expression();
		primary_s_relational_expression_p();
	}
	/*INVALID*/
	else 
		syn_printe();
	
	gen_incode("PLATY: Relational expression parsed");
}

/*
 * PRODUCTION: <primary a_relational expression> -> AVID_T | FPL_T | INL_T
 * FIRSTS: AVID_T, FPL_T, INL_T
*/
void primary_a_relational_expression(void){
	int laCode = lookahead.code;

	if (laCode == AVID_T)/*AVID*/
		match(AVID_T, NO_ATTR);
	
	else if (laCode == FPL_T)/*FLOATING POINT LITERAL*/
		match(FPL_T, NO_ATTR);
	
	else if (laCode == INL_T)/*INTEGER LITERAL*/
		match(INL_T, NO_ATTR);
	
	else /*Nothing else allowed*/
		syn_printe();
	
	gen_incode("PLATY: Primary a_relational expression parsed");
}

/*
 * PRODUCTION: <primary a_relational expression'> -> 
						== <primary a_relational expression>
						| != <primary a_relational expression>
						| > <primary a_relational expression>
						| < <primary a_relational expression>
 * FIRSTS: ==, !=, >, <
*/
void primary_a_relational_expression_p(void){
	Rel_Op laAttr = lookahead.attribute.rel_op;

	/*error*/
	if (lookahead.code != REL_OP_T){
		syn_printe();
		return;
	}

	/*IS EQUAL TO*/
	if (laAttr == EQ)
		match(REL_OP_T, EQ);
	
	/*NOT EQUAL*/
	else if (laAttr == NE)
		match(REL_OP_T, NE);
	
	/*GREATER THAN*/
	else if (laAttr == GT)
		match(REL_OP_T, GT);
	
	/*LESS THAN*/
	else if (laAttr == LT)
		match(REL_OP_T, LT);
	
	primary_a_relational_expression();
}

/*
 * PRODUCTION: <primary s_relational expression> -> <primary string expression>
 * FIRSTS: ==, !=, >, <
*/
void primary_s_relational_expression(void){
	primary_string_expression();
	gen_incode("PLATY: Primary s_relational expression parsed");
}


/* 
 *PRODUCTION: <primary s_relational expression'> ->
						== <primary s_relational expression>
						| != <primary s_relational expression>
						| > <primary s_relational expression>
						| < <primary s_relational expression>
 * FIRSTS: ==, !=, >, <
*/
void primary_s_relational_expression_p(void){
	Rel_Op laAttr = lookahead.attribute.rel_op;

		/*error*/
	if (lookahead.code != REL_OP_T){
		syn_printe();
		return;
	}
	if (laAttr == EQ)/*IS EQUAL TO*/
		match(REL_OP_T, EQ);
	
	else if (laAttr == NE)/*NOT EQUAL*/
		match(REL_OP_T, NE);
	
	else if (laAttr == GT)/*GREATER THAN*/
		match(REL_OP_T, GT);
	
	else if (laAttr == LT)/*LESS THAN*/
		match(REL_OP_T, LT);
	
	primary_s_relational_expression();
}