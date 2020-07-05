%option c++ prefix="og_scanner_" outfile="og_scanner.cpp"
%option stack noyywrap yylineno 8bit debug
%{
// make relevant includes before including the parser's tab file
#include <string>
#include <sstream>
#include <cdk/ast/sequence_node.h>
#include <cdk/ast/expression_node.h>
#include <cdk/ast/lvalue_node.h>
#include "og_parser.tab.h"
#include <cmath>
// don't change this
#define yyerror LexerError
#define CHECK_OVERFLOW_ERROR if (errno == ERANGE) yyerror("given integer is too large")
%}
%x X_STRING X_COMMENT
%%

">="                return tGE;
"<="                return tLE;
"=="                return tEQ;
"!="                return tNE;
"||"                return tOR;
"&&"                return tAND;

"auto"              return tAUTO;
"int"               return tINT;
"real"              return tREAL;
"string"            return tSTRING_TYPE;
"ptr"               return tPTR;
"public"            yylval.s = new std::string("public"); return tPUBLIC;
"require"           yylval.s = new std::string("require"); return tREQUIRE;
"sizeof"            return tSIZEOF;
"input"             return tINPUT;
"nullptr"           return tNULLPTR;

"return"            return tRETURN;
"procedure"         return tPROCEDURE;
"break"             return tBREAK;
"continue"          return tCONTINUE;
"if"                return tIF;
"then"              return tTHEN;
"elif"              return tELIF;
"else"              return tELSE;
"for"               return tFOR;
"do"                return tDO;
"write"             return tWRITE;
"writeln"           return tWRITELN;



[A-Za-z][A-Za-z0-9_]*  yylval.s = new std::string(yytext); return tIDENTIFIER;


\"                                          yy_push_state(X_STRING); yylval.s = new std::string("");
<X_STRING>\"                                yy_pop_state()         ; return tSTRING;
<X_STRING>\\0[0-9a-fA-F]                    *yylval.s += std::stoul(&yytext[2], nullptr, 16);
<X_STRING>\\[0-9a-fA-F]|\\[0-9a-fA-F]{2}    *yylval.s += std::stoul(&yytext[1], nullptr, 16);
<X_STRING>\\0[^0-9a-fA-F][^"]*\"            yy_pop_state()         ; return tSTRING;
<X_STRING>\\\\[A-Za-z0-9]                   *yylval.s += yytext + 1;
<X_STRING>\\n                               *yylval.s += "\n"      ;
<X_STRING>\\r                               *yylval.s += "\r"      ;
<X_STRING>\\t                               *yylval.s += "\t"      ;
<X_STRING>\\\"                              *yylval.s += yytext + 1;
<X_STRING>.                                 *yylval.s += yytext    ;

"/*"                   yy_push_state(X_COMMENT);
<X_COMMENT>"/*"        yy_push_state(X_COMMENT);
<X_COMMENT>"*/"        yy_pop_state()          ;
<X_COMMENT>.|\n        ;
"//".*$                ;


[0-9]+                                                  yylval.i = strtol(yytext, nullptr, 10)        ; CHECK_OVERFLOW_ERROR; return tINTEGER;
[0-9]+[Ee][0-9]+                                        yylval.i = std::round(strtod(yytext, nullptr)); CHECK_OVERFLOW_ERROR; return tINTEGER;
"0x"[0-9a-fA-F]+                                        yylval.i = strtol(yytext, nullptr, 16)        ; CHECK_OVERFLOW_ERROR; return tINTEGER;

([0-9]*\.[0-9]+|[0-9]+\.[0-9])([Ee]([-+])?[0-9]+)?      yylval.d = strtod(yytext, nullptr)            ; CHECK_OVERFLOW_ERROR; return tREAL;

[-()<>=+*/%;{},.@\[\]?]                                 return *yytext;

[ \t\n]+                                                ; /* ignore whitespace */

.                                                       yyerror("Unknown character");

%%
