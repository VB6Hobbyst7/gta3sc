/** \file
 *  This C header file was generated by $ANTLR version 3.5.2
 *
 *     -  From the grammar source file : gta3script.g
 *     -                            On : 2016-03-04 04:22:52
 *     -                for the parser : gta3scriptParserParser
 *
 * Editing it, at least manually, is not wise.
 *
 * C language generator and runtime by Jim Idle, jimi|hereisanat|idle|dotgoeshere|ws.
 *
 *
 * The parser 
gta3scriptParser

has the callable functions (rules) shown below,
 * which will invoke the code for the associated rule in the source grammar
 * assuming that the input stream is pointing to a token/text stream that could begin
 * this rule.
 *
 * For instance if you call the first (topmost) rule in a parser grammar, you will
 * get the results of a full parse, but calling a rule half way through the grammar will
 * allow you to pass part of a full token stream to the parser, such as for syntax checking
 * in editors and so on.
 *
 * The parser entry points are called indirectly (by function pointer to function) via
 * a parser context typedef pgta3scriptParser, which is returned from a call to gta3scriptParserNew().
 *
 * The methods in pgta3scriptParser are  as follows:
 *
 *  - 
 gta3scriptParser_start_return
      pgta3scriptParser->start(pgta3scriptParser)
 *  - 
 gta3scriptParser_statementList_return
      pgta3scriptParser->statementList(pgta3scriptParser)
 *  - 
 gta3scriptParser_statement_return
      pgta3scriptParser->statement(pgta3scriptParser)
 *  - 
 gta3scriptParser_labelStatement_return
      pgta3scriptParser->labelStatement(pgta3scriptParser)
 *  - 
 gta3scriptParser_loopControlStatement_return
      pgta3scriptParser->loopControlStatement(pgta3scriptParser)
 *  - 
 gta3scriptParser_scopeStatement_return
      pgta3scriptParser->scopeStatement(pgta3scriptParser)
 *  - 
 gta3scriptParser_variableDeclaration_return
      pgta3scriptParser->variableDeclaration(pgta3scriptParser)
 *  - 
 gta3scriptParser_variableDeclarationIdentity_return
      pgta3scriptParser->variableDeclarationIdentity(pgta3scriptParser)
 *  - 
 gta3scriptParser_whileStatement_return
      pgta3scriptParser->whileStatement(pgta3scriptParser)
 *  - 
 gta3scriptParser_ifStatement_return
      pgta3scriptParser->ifStatement(pgta3scriptParser)
 *  - 
 gta3scriptParser_repeatStatement_return
      pgta3scriptParser->repeatStatement(pgta3scriptParser)
 *  - 
 gta3scriptParser_switchStatement_return
      pgta3scriptParser->switchStatement(pgta3scriptParser)
 *  - 
 gta3scriptParser_switchStatementCase_return
      pgta3scriptParser->switchStatementCase(pgta3scriptParser)
 *  - 
 gta3scriptParser_switchStatementDefault_return
      pgta3scriptParser->switchStatementDefault(pgta3scriptParser)
 *  - 
 gta3scriptParser_conditionList_return
      pgta3scriptParser->conditionList(pgta3scriptParser)
 *  - 
 gta3scriptParser_conditionListSingle_return
      pgta3scriptParser->conditionListSingle(pgta3scriptParser)
 *  - 
 gta3scriptParser_conditionListAnd_return
      pgta3scriptParser->conditionListAnd(pgta3scriptParser)
 *  - 
 gta3scriptParser_conditionListOr_return
      pgta3scriptParser->conditionListOr(pgta3scriptParser)
 *  - 
 gta3scriptParser_expressionStatement_return
      pgta3scriptParser->expressionStatement(pgta3scriptParser)
 *  - 
 gta3scriptParser_assignmentOperators1_return
      pgta3scriptParser->assignmentOperators1(pgta3scriptParser)
 *  - 
 gta3scriptParser_assignmentOperators2_return
      pgta3scriptParser->assignmentOperators2(pgta3scriptParser)
 *  - 
 gta3scriptParser_binaryOperators_return
      pgta3scriptParser->binaryOperators(pgta3scriptParser)
 *  - 
 gta3scriptParser_relationalOperators_return
      pgta3scriptParser->relationalOperators(pgta3scriptParser)
 *  - 
 gta3scriptParser_unaryOperators_return
      pgta3scriptParser->unaryOperators(pgta3scriptParser)
 *  - 
 gta3scriptParser_commandStatement_return
      pgta3scriptParser->commandStatement(pgta3scriptParser)
 *  - 
 gta3scriptParser_positiveCommandStatement_return
      pgta3scriptParser->positiveCommandStatement(pgta3scriptParser)
 *  - 
 gta3scriptParser_argument_return
      pgta3scriptParser->argument(pgta3scriptParser)
 *  - 
 gta3scriptParser_identifier_return
      pgta3scriptParser->identifier(pgta3scriptParser)
 *  - 
 gta3scriptParser_integerConstant_return
      pgta3scriptParser->integerConstant(pgta3scriptParser)
 *  - 
 gta3scriptParser_newLine_return
      pgta3scriptParser->newLine(pgta3scriptParser)
 *
 * The return type for any particular rule is of course determined by the source
 * grammar file.
 */
// [The "BSD license"]
// Copyright (c) 2005-2009 Jim Idle, Temporal Wave LLC
// http://www.temporal-wave.com
// http://www.linkedin.com/in/jimidle
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
// 3. The name of the author may not be used to endorse or promote products
//    derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
// IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
// OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
// IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
// NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
// THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef	_gta3scriptParser_H
#define _gta3scriptParser_H
/* =============================================================================
 * Standard antlr3 C runtime definitions
 */
#include    <antlr3.h>

/* End of standard antlr 3 runtime definitions
 * =============================================================================
 */

#ifdef __cplusplus
extern "C" {
#endif

// Forward declare the context typedef so that we can use it before it is
// properly defined. Delegators and delegates (from import statements) are
// interdependent and their context structures contain pointers to each other
// C only allows such things to be declared if you pre-declare the typedef.
//
typedef struct gta3scriptParser_Ctx_struct gta3scriptParser, * pgta3scriptParser;



   #define _empty NULL


#ifdef	ANTLR3_WINDOWS
// Disable: Unreferenced parameter,							- Rules with parameters that are not used
//          constant conditional,							- ANTLR realizes that a prediction is always true (synpred usually)
//          initialized but unused variable					- tree rewrite variables declared but not needed
//          Unreferenced local variable						- lexer rule declares but does not always use _type
//          potentially unitialized variable used			- retval always returned from a rule
//			unreferenced local function has been removed	- susually getTokenNames or freeScope, they can go without warnigns
//
// These are only really displayed at warning level /W4 but that is the code ideal I am aiming at
// and the codegen must generate some of these warnings by necessity, apart from 4100, which is
// usually generated when a parser rule is given a parameter that it does not use. Mostly though
// this is a matter of orthogonality hence I disable that one.
//
#pragma warning( disable : 4100 )
#pragma warning( disable : 4101 )
#pragma warning( disable : 4127 )
#pragma warning( disable : 4189 )
#pragma warning( disable : 4505 )
#pragma warning( disable : 4701 )
#endif
typedef struct gta3scriptParser_start_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    gta3scriptParser_start_return;



typedef struct gta3scriptParser_statementList_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    gta3scriptParser_statementList_return;



typedef struct gta3scriptParser_statement_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    gta3scriptParser_statement_return;



typedef struct gta3scriptParser_labelStatement_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    gta3scriptParser_labelStatement_return;



typedef struct gta3scriptParser_loopControlStatement_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    gta3scriptParser_loopControlStatement_return;



typedef struct gta3scriptParser_scopeStatement_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    gta3scriptParser_scopeStatement_return;



typedef struct gta3scriptParser_variableDeclaration_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    gta3scriptParser_variableDeclaration_return;



typedef struct gta3scriptParser_variableDeclarationIdentity_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    gta3scriptParser_variableDeclarationIdentity_return;



typedef struct gta3scriptParser_whileStatement_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    gta3scriptParser_whileStatement_return;



typedef struct gta3scriptParser_ifStatement_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    gta3scriptParser_ifStatement_return;



typedef struct gta3scriptParser_repeatStatement_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    gta3scriptParser_repeatStatement_return;



typedef struct gta3scriptParser_switchStatement_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    gta3scriptParser_switchStatement_return;



typedef struct gta3scriptParser_switchStatementCase_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    gta3scriptParser_switchStatementCase_return;



typedef struct gta3scriptParser_switchStatementDefault_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    gta3scriptParser_switchStatementDefault_return;



typedef struct gta3scriptParser_conditionList_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    gta3scriptParser_conditionList_return;



typedef struct gta3scriptParser_conditionListSingle_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    gta3scriptParser_conditionListSingle_return;



typedef struct gta3scriptParser_conditionListAnd_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    gta3scriptParser_conditionListAnd_return;



typedef struct gta3scriptParser_conditionListOr_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    gta3scriptParser_conditionListOr_return;



typedef struct gta3scriptParser_expressionStatement_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    gta3scriptParser_expressionStatement_return;



typedef struct gta3scriptParser_assignmentOperators1_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    gta3scriptParser_assignmentOperators1_return;



typedef struct gta3scriptParser_assignmentOperators2_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    gta3scriptParser_assignmentOperators2_return;



typedef struct gta3scriptParser_binaryOperators_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    gta3scriptParser_binaryOperators_return;



typedef struct gta3scriptParser_relationalOperators_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    gta3scriptParser_relationalOperators_return;



typedef struct gta3scriptParser_unaryOperators_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    gta3scriptParser_unaryOperators_return;



typedef struct gta3scriptParser_commandStatement_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    gta3scriptParser_commandStatement_return;



typedef struct gta3scriptParser_positiveCommandStatement_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    gta3scriptParser_positiveCommandStatement_return;



typedef struct gta3scriptParser_argument_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    gta3scriptParser_argument_return;



typedef struct gta3scriptParser_identifier_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    gta3scriptParser_identifier_return;



typedef struct gta3scriptParser_integerConstant_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    gta3scriptParser_integerConstant_return;



typedef struct gta3scriptParser_newLine_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    gta3scriptParser_newLine_return;




/** Context tracking structure for 
gta3scriptParser

 */
struct gta3scriptParser_Ctx_struct
{
    /** Built in ANTLR3 context tracker contains all the generic elements
     *  required for context tracking.
     */
    pANTLR3_PARSER   pParser;

     gta3scriptParser_start_return
     (*start)	(struct gta3scriptParser_Ctx_struct * ctx);

     gta3scriptParser_statementList_return
     (*statementList)	(struct gta3scriptParser_Ctx_struct * ctx);

     gta3scriptParser_statement_return
     (*statement)	(struct gta3scriptParser_Ctx_struct * ctx);

     gta3scriptParser_labelStatement_return
     (*labelStatement)	(struct gta3scriptParser_Ctx_struct * ctx);

     gta3scriptParser_loopControlStatement_return
     (*loopControlStatement)	(struct gta3scriptParser_Ctx_struct * ctx);

     gta3scriptParser_scopeStatement_return
     (*scopeStatement)	(struct gta3scriptParser_Ctx_struct * ctx);

     gta3scriptParser_variableDeclaration_return
     (*variableDeclaration)	(struct gta3scriptParser_Ctx_struct * ctx);

     gta3scriptParser_variableDeclarationIdentity_return
     (*variableDeclarationIdentity)	(struct gta3scriptParser_Ctx_struct * ctx);

     gta3scriptParser_whileStatement_return
     (*whileStatement)	(struct gta3scriptParser_Ctx_struct * ctx);

     gta3scriptParser_ifStatement_return
     (*ifStatement)	(struct gta3scriptParser_Ctx_struct * ctx);

     gta3scriptParser_repeatStatement_return
     (*repeatStatement)	(struct gta3scriptParser_Ctx_struct * ctx);

     gta3scriptParser_switchStatement_return
     (*switchStatement)	(struct gta3scriptParser_Ctx_struct * ctx);

     gta3scriptParser_switchStatementCase_return
     (*switchStatementCase)	(struct gta3scriptParser_Ctx_struct * ctx);

     gta3scriptParser_switchStatementDefault_return
     (*switchStatementDefault)	(struct gta3scriptParser_Ctx_struct * ctx);

     gta3scriptParser_conditionList_return
     (*conditionList)	(struct gta3scriptParser_Ctx_struct * ctx);

     gta3scriptParser_conditionListSingle_return
     (*conditionListSingle)	(struct gta3scriptParser_Ctx_struct * ctx);

     gta3scriptParser_conditionListAnd_return
     (*conditionListAnd)	(struct gta3scriptParser_Ctx_struct * ctx);

     gta3scriptParser_conditionListOr_return
     (*conditionListOr)	(struct gta3scriptParser_Ctx_struct * ctx);

     gta3scriptParser_expressionStatement_return
     (*expressionStatement)	(struct gta3scriptParser_Ctx_struct * ctx);

     gta3scriptParser_assignmentOperators1_return
     (*assignmentOperators1)	(struct gta3scriptParser_Ctx_struct * ctx);

     gta3scriptParser_assignmentOperators2_return
     (*assignmentOperators2)	(struct gta3scriptParser_Ctx_struct * ctx);

     gta3scriptParser_binaryOperators_return
     (*binaryOperators)	(struct gta3scriptParser_Ctx_struct * ctx);

     gta3scriptParser_relationalOperators_return
     (*relationalOperators)	(struct gta3scriptParser_Ctx_struct * ctx);

     gta3scriptParser_unaryOperators_return
     (*unaryOperators)	(struct gta3scriptParser_Ctx_struct * ctx);

     gta3scriptParser_commandStatement_return
     (*commandStatement)	(struct gta3scriptParser_Ctx_struct * ctx);

     gta3scriptParser_positiveCommandStatement_return
     (*positiveCommandStatement)	(struct gta3scriptParser_Ctx_struct * ctx);

     gta3scriptParser_argument_return
     (*argument)	(struct gta3scriptParser_Ctx_struct * ctx);

     gta3scriptParser_identifier_return
     (*identifier)	(struct gta3scriptParser_Ctx_struct * ctx);

     gta3scriptParser_integerConstant_return
     (*integerConstant)	(struct gta3scriptParser_Ctx_struct * ctx);

     gta3scriptParser_newLine_return
     (*newLine)	(struct gta3scriptParser_Ctx_struct * ctx);
    // Delegated rules

    const char * (*getGrammarFileName)();
    void            (*reset)  (struct gta3scriptParser_Ctx_struct * ctx);
    void	    (*free)   (struct gta3scriptParser_Ctx_struct * ctx);
/* @headerFile.members() */
pANTLR3_BASE_TREE_ADAPTOR	adaptor;
pANTLR3_VECTOR_FACTORY		vectors;
/* End @headerFile.members() */
};

// Function protoypes for the constructor functions that external translation units
// such as delegators and delegates may wish to call.
//
ANTLR3_API pgta3scriptParser gta3scriptParserNew         (
pANTLR3_COMMON_TOKEN_STREAM
 instream);
ANTLR3_API pgta3scriptParser gta3scriptParserNewSSD      (
pANTLR3_COMMON_TOKEN_STREAM
 instream, pANTLR3_RECOGNIZER_SHARED_STATE state);

/** Symbolic definitions of all the tokens that the 
parser
 will work with.
 * \{
 *
 * Antlr will define EOF, but we can't use that as it it is too common in
 * in C header files and that would be confusing. There is no way to filter this out at the moment
 * so we just undef it here for now. That isn't the value we get back from C recognizers
 * anyway. We are looking for ANTLR3_TOKEN_EOF.
 */
#ifdef	EOF
#undef	EOF
#endif
#ifdef	Tokens
#undef	Tokens
#endif
#define EOF      -1
#define T__98      98
#define T__99      99
#define T__100      100
#define T__101      101
#define T__102      102
#define T__103      103
#define T__104      104
#define T__105      105
#define T__106      106
#define T__107      107
#define T__108      108
#define T__109      109
#define T__110      110
#define T__111      111
#define T__112      112
#define T__113      113
#define T__114      114
#define T__115      115
#define T__116      116
#define T__117      117
#define T__118      118
#define T__119      119
#define T__120      120
#define T__121      121
#define T__122      122
#define T__123      123
#define T__124      124
#define T__125      125
#define T__126      126
#define T__127      127
#define T__128      128
#define T__129      129
#define T__130      130
#define T__131      131
#define T__132      132
#define T__133      133
#define T__134      134
#define T__135      135
#define T__136      136
#define A      4
#define AND      5
#define ARRAY      6
#define B      7
#define BLOCK      8
#define BREAK      9
#define C      10
#define CASE      11
#define COMMAND      12
#define COMMENT      13
#define CONTINUE      14
#define D      15
#define DEC_DIGIT      16
#define DEC_LITERAL      17
#define DEFAULT      18
#define E      19
#define ELSE      20
#define ENDIF      21
#define ENDREPEAT      22
#define ENDSWITCH      23
#define ENDWHILE      24
#define ESC_SEQ      25
#define EXPONENT      26
#define F      27
#define FLOAT      28
#define FLOAT_LITERAL      29
#define G      30
#define H      31
#define HEX_DIGIT      32
#define HEX_LITERAL      33
#define I      34
#define IDENTIFIER      35
#define IF      36
#define INTEGER      37
#define J      38
#define K      39
#define L      40
#define LABEL      41
#define LONG_STRING      42
#define LVAR_FLOAT      43
#define LVAR_INT      44
#define LVAR_TEXT_LABEL      45
#define LVAR_TEXT_LABEL16      46
#define M      47
#define N      48
#define NOT      49
#define O      50
#define OCTAL_ESC      51
#define OCT_DIGIT      52
#define OCT_LITERAL      53
#define OP_ADD      54
#define OP_AND      55
#define OP_CAST      56
#define OP_DEC      57
#define OP_DIV      58
#define OP_EQ      59
#define OP_GE      60
#define OP_GEQ      61
#define OP_INC      62
#define OP_LE      63
#define OP_LEQ      64
#define OP_MOD      65
#define OP_MUL      66
#define OP_OR      67
#define OP_SHL      68
#define OP_SHR      69
#define OP_SUB      70
#define OP_TIMED_ADD      71
#define OP_TIMED_SUB      72
#define OP_XOR      73
#define OR      74
#define P      75
#define Q      76
#define R      77
#define REPEAT      78
#define S      79
#define SCOPE      80
#define SHORT_STRING      81
#define SKIPS      82
#define SWITCH      83
#define T      84
#define U      85
#define UNICODE_ESC      86
#define V      87
#define VAR_FLOAT      88
#define VAR_INT      89
#define VAR_TEXT_LABEL      90
#define VAR_TEXT_LABEL16      91
#define W      92
#define WHILE      93
#define WS      94
#define X      95
#define Y      96
#define Z      97
#ifdef	EOF
#undef	EOF
#define	EOF	ANTLR3_TOKEN_EOF
#endif

#ifndef TOKENSOURCE
#define TOKENSOURCE(lxr) lxr->pLexer->rec->state->tokSource
#endif

/* End of token definitions for gta3scriptParser
 * =============================================================================
 */
/** } */

#ifdef __cplusplus
}
#endif

#endif

/* END - Note:Keep extra line feed to satisfy UNIX systems */
