%{
//-- don't change *any* of these: if you do, you'll break the compiler.
#include <cdk/compiler.h>
#include "ast/all.h"
#define LINE               compiler->scanner()->lineno()
#define yylex()            compiler->scanner()->scan()
#define yyerror(s)         compiler->scanner()->error(s)
#define YYPARSE_PARAM_TYPE std::shared_ptr<cdk::compiler>
#define YYPARSE_PARAM      compiler
//-- don't change *any* of these --- END!
#include <algorithm>
%}

%union {
  int                                i;	         /* integer value */
  double                             d;
  std::string                       *s;	         /* symbol name or string literal */
  cdk::basic_node                   *node;	     /* node pointer */
  cdk::sequence_node                *sequence;
  cdk::expression_node              *expression; /* expression nodes */
  cdk::lvalue_node                  *lvalue;
  cdk::basic_type                   *type;
  cdk::primitive_type               *primitive_type;
  cdk::reference_type               *reference_type;
  og::block_node                    *block;
  og::for_node                      *for_type;
  cdk::string_node                  *string_node;
  og::tuple_index_node              *tindex;

};

%token <i> tINTEGER
%token <s> tIDENTIFIER tSTRING tPUBLIC tREQUIRE
%token <d> tREAL
%token <primitive_type> tAUTO

%token tINT tREAL tPTR tSTRING_TYPE
%token tPUBLIC tREQUIRE tPROCEDURE tRETURN tNULLPTR tFOR tDO
%token tINPUT tWRITE tWRITELN tSIZEOF tBREAK tCONTINUE
%token tIF tTHEN tELIF tELSE

%nonassoc tIF tIFX
%nonassoc tTHEN tDO
%nonassoc tELIF tELSE
%nonassoc tIDENTIFIER

%right '=' ',' tAND tOR
%left tGE tLE tEQ tNE '>' '<' '@'
%left '+' '-'
%left '*' '/' '%'
%nonassoc tUNARY '['

%type <expression> expr
%type <block>  block
%type <sequence> file declarations function_declarations variables expressions  instructions identifiers
%type <node> declaration function_declaration variable function procedure instruction elif conditional innerblock
%type<lvalue> identifier indexation tuple_index lvalue
%type <type> type
%type <reference_type> ptr
%type<for_type> iteration
%type<string_node> strings

%{
//-- The rules below will be included in yyparse, the main parsing function.
%}
%%

/* Main file ---------------------------------------------------------------- */

file         :              { compiler->ast($$ = new cdk::sequence_node(LINE)); }
             | declarations { compiler->ast($$ = $1);                           }
             ;

declaration  : variable ';' { $$ = $1; }
             | function     { $$ = $1; }
             | procedure    { $$ = $1; }
             ;

declarations : declaration              { $$ = new cdk::sequence_node(LINE, $1);     }
             | declarations declaration { $$ = new cdk::sequence_node(LINE, $2, $1); }
             ;

/* -------------------------------------------------------------------------- */


/* Variables ---------------------------------------------------------------- */

variable    :          type  tIDENTIFIER                 { $$ = new og::variable_declaration_node(LINE, std::shared_ptr<cdk::basic_type>($1), *$2, "",  nullptr); }
            | tPUBLIC  type  tIDENTIFIER                 { $$ = new og::variable_declaration_node(LINE, std::shared_ptr<cdk::basic_type>($2), *$3, *$1, nullptr); }
            | tREQUIRE type  tIDENTIFIER                 { $$ = new og::variable_declaration_node(LINE, std::shared_ptr<cdk::basic_type>($2), *$3, *$1, nullptr); }
            |          type  tIDENTIFIER '=' expr        { $$ = new og::variable_declaration_node(LINE, std::shared_ptr<cdk::basic_type>($1), *$2, "",  $4);      }
            | tPUBLIC  type  tIDENTIFIER '=' expr        { $$ = new og::variable_declaration_node(LINE, std::shared_ptr<cdk::basic_type>($2), *$3, *$1, $5);      }
            | tAUTO identifiers '=' expressions {
                if($2->size() == 1 && $4->size() > 1) {      //auto a = 1,2,3,4
                    $$ = new og::variable_declaration_node(LINE, std::shared_ptr<cdk::basic_type>(new cdk::primitive_type(0, cdk::TYPE_STRUCT)), static_cast<cdk::variable_node*>($2->node(0))->name(), "", new og::tuple_node(LINE, $4));
                }
                else if($2->size() > 1 && $4->size() == 1) { //auto a,b = f() => a=f()@1, b=f()@2
                    cdk::sequence_node* new_variables_sequence = new cdk::sequence_node(LINE);
                    for (size_t i = 0; i < $2->size(); i++) {
                        std::string variable_name = static_cast<cdk::variable_node*>($2->node(i))->name();
                        cdk::basic_node* item = new og::variable_declaration_node(LINE, cdk::make_primitive_type(0, cdk::TYPE_UNSPEC), variable_name, "", new cdk::rvalue_node(LINE, new og::tuple_index_node(LINE, static_cast<cdk::expression_node*>($4->node(0)), new cdk::integer_node(LINE, int(i)))));
                        new_variables_sequence->nodes().push_back(item);
                    }
                    $$ = new_variables_sequence;
                }
                else if($2->size() != $4->size()) {
                    yyerror("length of identifiers and expressions is not equal");
                }
                else {
                      cdk::sequence_node* new_variables_sequence = new cdk::sequence_node(LINE);
                      for (size_t i = 0; i < $2->size(); i++) {
                          std::string variable_name = static_cast<cdk::variable_node*>($2->node(i))->name();
                          cdk::basic_node* item = new og::variable_declaration_node(LINE, cdk::make_primitive_type(0, cdk::TYPE_UNSPEC), variable_name, "", static_cast<cdk::expression_node*>($4->node(i)));
                          new_variables_sequence->nodes().push_back(item);
                      }
                      $$ = new_variables_sequence;
                }
            }
            | tPUBLIC tAUTO identifiers '=' expressions {
                if($3->size() == 1 && $5->size() > 1) {      //public auto a = 1,2,3,4
                    $$ = new og::variable_declaration_node(LINE, cdk::make_primitive_type(0, cdk::TYPE_UNSPEC), static_cast<cdk::variable_node*>($3->node(0))->name(), *$1, static_cast<cdk::expression_node*>(new og::tuple_node(LINE, $5)));
                }
                else if($3->size() > 1 && $5->size() == 1) { //public auto a,b = f() => a=f()@1, b=f()@2
                    cdk::sequence_node* new_variables_sequence = new cdk::sequence_node(LINE);
                    for (size_t i = 0; i < $3->size(); i++) {
                        std::string variable_name = static_cast<cdk::variable_node*>($3->node(i))->name();
                        cdk::basic_node* item = new og::variable_declaration_node(LINE, cdk::make_primitive_type(0, cdk::TYPE_UNSPEC), variable_name, *$1, new cdk::rvalue_node(LINE, new og::tuple_index_node(LINE, static_cast<cdk::expression_node*>($5->node(0)), new cdk::integer_node(LINE, int(i)))));
                        new_variables_sequence->nodes().push_back(item);
                    }
                    $$ = new_variables_sequence;
                }
                else if($3->size() != $5->size()) {
                    yyerror("length of identifiers and expressions is not equal");
                }
                else {
                    cdk::sequence_node* new_variables_sequence = new cdk::sequence_node(LINE);
                    for (size_t i = 0; i < $3->size(); i++) {
                        std::string variable_name = static_cast<cdk::variable_node*>($3->node(i))->name();
                        cdk::basic_node* item = new og::variable_declaration_node(LINE, cdk::make_primitive_type(0, cdk::TYPE_UNSPEC), variable_name, *$1, static_cast<cdk::expression_node*>($5->node(i)));
                        new_variables_sequence->nodes().push_back(item);
                }
                    $$ = new_variables_sequence;
                }
            }
            ;

variables   : variable                { $$ = new cdk::sequence_node(LINE, $1);     }
            | variables ',' variable  { $$ = new cdk::sequence_node(LINE, $3, $1); }
            ;



type        : tINT                 { $$ = new cdk::primitive_type(4, cdk::TYPE_INT);    } /*new cdk::primitive_type do type int */
            | tREAL                { $$ = new cdk::primitive_type(8, cdk::TYPE_DOUBLE);   }
            | tSTRING_TYPE         { $$ = new cdk::primitive_type(4, cdk::TYPE_STRING); }
            | ptr                  { $$ = $1; }
            ;

ptr         : tPTR '<' tREAL '>'   { $$ = new cdk::reference_type(4, std::shared_ptr<cdk::primitive_type>(new cdk::primitive_type(8, cdk::TYPE_DOUBLE))); }
            | tPTR '<' tINT  '>'   { $$ = new cdk::reference_type(4, std::shared_ptr<cdk::primitive_type>(new cdk::primitive_type(4, cdk::TYPE_INT)));  }
            | tPTR '<' tAUTO '>'   { $$ = new cdk::reference_type(4, std::shared_ptr<cdk::primitive_type>(new cdk::primitive_type(0, cdk::TYPE_UNSPEC))); }
            | tPTR '<' ptr   '>'   { $$ = new cdk::reference_type(4, std::shared_ptr<cdk::reference_type>($3)); }
            ;

identifier  : tIDENTIFIER          { $$ = new cdk::variable_node(LINE, *$1); }
            ;

identifiers : identifier                   { $$ = new cdk::sequence_node(LINE, $1);     }
            | identifiers ',' identifier   { $$ = new cdk::sequence_node(LINE, $3, $1); }
            ;

/* -------------------------------------------------------------------------- */


/* Instructions ------------------------------------------------------------- */

instruction  : expr                 ';'  { $$ = new og::evaluation_node(LINE, $1);  }
             | tWRITE expressions   ';'  { $$ = new og::write_node(LINE, $2);       }
             | tWRITELN expressions ';'  { $$ = new og::writeln_node(LINE, $2);     }
             | tINPUT                    { $$ = new og::input_node(LINE);           }
             | tBREAK                    { $$ = new og::break_node(LINE);           }
             | tCONTINUE                 { $$ = new og::continue_node(LINE);        }
             | tRETURN expressions  ';'  {
                 if ($2->size() > 1)
                    $$ = new og::return_node(LINE, new og::tuple_node(LINE, $2));
                 else
                    $$ = new og::return_node(LINE, static_cast<cdk::expression_node*>($2->node(0)));
             }
             | tRETURN              ';'  { $$ = new og::return_node(LINE, nullptr); }
             | conditional               { $$ = $1;                                 }
             | iteration                 { $$ = $1;                                 }
             ;

instructions : instruction                { $$ = new cdk::sequence_node(LINE, $1);     }
             | instructions instruction   { $$ = new cdk::sequence_node(LINE, $2, $1); }
             ;

iteration    : tFOR             ';'             ';'             tDO innerblock  { $$ = new og::for_node(LINE, new cdk::sequence_node(LINE), new cdk::sequence_node(LINE), new cdk::sequence_node(LINE), $5); }
             | tFOR variables   ';'             ';'             tDO innerblock  { $$ = new og::for_node(LINE,                           $2, new cdk::sequence_node(LINE), new cdk::sequence_node(LINE), $6); }
             | tFOR             ';' expressions ';'             tDO innerblock  { $$ = new og::for_node(LINE, new cdk::sequence_node(LINE),                           $3, new cdk::sequence_node(LINE), $6); }
             | tFOR             ';'             ';' expressions tDO innerblock  { $$ = new og::for_node(LINE, new cdk::sequence_node(LINE), new cdk::sequence_node(LINE),                           $4, $6); }
             | tFOR variables   ';' expressions ';'             tDO innerblock  { $$ = new og::for_node(LINE,                           $2,                           $4, new cdk::sequence_node(LINE), $7); }
             | tFOR variables   ';'             ';' expressions tDO innerblock  { $$ = new og::for_node(LINE,                           $2, new cdk::sequence_node(LINE),                           $5, $7); }
             | tFOR             ';' expressions ';' expressions tDO innerblock  { $$ = new og::for_node(LINE, new cdk::sequence_node(LINE),                           $3,                           $5, $7); }
             | tFOR variables   ';' expressions ';' expressions tDO innerblock  { $$ = new og::for_node(LINE,                           $2,                           $4,                           $6, $8); }
             | tFOR expressions ';' expressions ';' expressions tDO innerblock  { $$ = new og::for_node(LINE,                           $2,                           $4,                           $6, $8); }
             ;


conditional  : tIF expr tTHEN innerblock %prec tIFX   { $$ = new og::if_node(LINE, $2, $4);          }
             | tIF expr tTHEN innerblock elif         { $$ = new og::if_else_node(LINE, $2, $4, $5); }
             ;

elif         : tELSE innerblock                       { $$ = $2;                                     }
             | tELIF expr tTHEN innerblock            { $$ = new og::if_node(LINE, $2,      $4);     }
             | tELIF expr tTHEN innerblock elif       { $$ = new og::if_else_node(LINE, $2, $4, $5); }
             ;


/* -------------------------------------------------------------------------- */


/* Functions and procedures-------------------------------------------------- */

function  :           type  tIDENTIFIER '('           ')'                 { $$ = new og::function_declaration_node(LINE,  std::shared_ptr<cdk::basic_type>($1), *$2, new cdk::sequence_node(LINE), ""); }
          |           tAUTO tIDENTIFIER '('           ')'                 { $$ = new og::function_declaration_node(LINE,  cdk::make_primitive_type(0, cdk::TYPE_UNSPEC), *$2, new cdk::sequence_node(LINE), ""); }
          |           type  tIDENTIFIER '(' variables ')'                 { $$ = new og::function_declaration_node(LINE,  std::shared_ptr<cdk::basic_type>($1), *$2,                           $4, ""); }
          |           tAUTO tIDENTIFIER '(' variables ')'                 { $$ = new og::function_declaration_node(LINE,  cdk::make_primitive_type(0, cdk::TYPE_UNSPEC), *$2,                           $4, ""); }
          |           type  tIDENTIFIER '('           ')' block           { $$ = new og::function_definition_node (LINE,  std::shared_ptr<cdk::basic_type>($1), *$2, new cdk::sequence_node(LINE), $5, ""); }
          |           tAUTO tIDENTIFIER '('           ')' block           { $$ = new og::function_definition_node (LINE,  cdk::make_primitive_type(0, cdk::TYPE_UNSPEC), *$2, new cdk::sequence_node(LINE), $5, ""); }
          |           type  tIDENTIFIER '(' variables ')' block           { $$ = new og::function_definition_node (LINE,  std::shared_ptr<cdk::basic_type>($1), *$2,                           $4, $6, ""); }
          |           tAUTO tIDENTIFIER '(' variables ')' block           { $$ = new og::function_definition_node (LINE,  cdk::make_primitive_type(0, cdk::TYPE_UNSPEC), *$2,                           $4, $6, ""); }
          | tPUBLIC   type  tIDENTIFIER '('           ')'                 { $$ = new og::function_declaration_node(LINE,  std::shared_ptr<cdk::basic_type>($2), *$3, new cdk::sequence_node(LINE), *$1); }
          | tPUBLIC   tAUTO tIDENTIFIER '('           ')'                 { $$ = new og::function_declaration_node(LINE,  cdk::make_primitive_type(0, cdk::TYPE_UNSPEC), *$3, new cdk::sequence_node(LINE), *$1); }
          | tPUBLIC   type  tIDENTIFIER '(' variables ')'                 { $$ = new og::function_declaration_node(LINE,  std::shared_ptr<cdk::basic_type>($2), *$3,                           $5, *$1); }
          | tPUBLIC   tAUTO tIDENTIFIER '(' variables ')'                 { $$ = new og::function_declaration_node(LINE,  cdk::make_primitive_type(0, cdk::TYPE_UNSPEC), *$3,                           $5, *$1); }
          | tPUBLIC   type  tIDENTIFIER '('           ')' block           { $$ = new og::function_definition_node (LINE,  std::shared_ptr<cdk::basic_type>($2), *$3, new cdk::sequence_node(LINE), $6, *$1); }
          | tPUBLIC   tAUTO tIDENTIFIER '('           ')' block           { $$ = new og::function_definition_node (LINE,  cdk::make_primitive_type(0, cdk::TYPE_UNSPEC), *$3, new cdk::sequence_node(LINE), $6, *$1); }
          | tPUBLIC   type  tIDENTIFIER '(' variables ')' block           { $$ = new og::function_definition_node (LINE,  std::shared_ptr<cdk::basic_type>($2), *$3,                           $5, $7, *$1); }
          | tPUBLIC   tAUTO tIDENTIFIER '(' variables ')' block           { $$ = new og::function_definition_node (LINE,  cdk::make_primitive_type(0, cdk::TYPE_UNSPEC), *$3,                           $5, $7, *$1); }
          | tREQUIRE  type  tIDENTIFIER '('           ')'                 { $$ = new og::function_declaration_node(LINE,  std::shared_ptr<cdk::basic_type>($2), *$3, new cdk::sequence_node(LINE), *$1); }
          | tREQUIRE  tAUTO tIDENTIFIER '('           ')'                 { $$ = new og::function_declaration_node(LINE,  cdk::make_primitive_type(0, cdk::TYPE_UNSPEC), *$3, new cdk::sequence_node(LINE), *$1); }
          | tREQUIRE  type  tIDENTIFIER '(' variables ')'                 { $$ = new og::function_declaration_node(LINE,  std::shared_ptr<cdk::basic_type>($2), *$3,                           $5, *$1); }
          | tREQUIRE  tAUTO tIDENTIFIER '(' variables ')'                 { $$ = new og::function_declaration_node(LINE,  cdk::make_primitive_type(0, cdk::TYPE_UNSPEC), *$3,                           $5, *$1); }
          ;

procedure :           tPROCEDURE tIDENTIFIER '('           ')'            { $$ = new og::function_declaration_node(LINE,  cdk::make_primitive_type(0, cdk::TYPE_VOID), *$2, new cdk::sequence_node(LINE),  "");  }
          |           tPROCEDURE tIDENTIFIER '(' variables ')'            { $$ = new og::function_declaration_node(LINE,  cdk::make_primitive_type(0, cdk::TYPE_VOID), *$2,                           $4,  "");  }
          |           tPROCEDURE tIDENTIFIER '('           ')' block      { $$ = new og::function_definition_node (LINE,  cdk::make_primitive_type(0, cdk::TYPE_VOID), *$2, new cdk::sequence_node(LINE),  $5, "");   }
          |           tPROCEDURE tIDENTIFIER '(' variables ')' block      { $$ = new og::function_definition_node (LINE,  cdk::make_primitive_type(0, cdk::TYPE_VOID), *$2,                           $4,  $6,   ""); }
          | tPUBLIC   tPROCEDURE tIDENTIFIER '('           ')'            { $$ = new og::function_declaration_node(LINE,  cdk::make_primitive_type(0, cdk::TYPE_VOID), *$3, new cdk::sequence_node(LINE),  *$1); }
          | tPUBLIC   tPROCEDURE tIDENTIFIER '(' variables ')'            { $$ = new og::function_declaration_node(LINE,  cdk::make_primitive_type(0, cdk::TYPE_VOID), *$3,                           $5,  *$1); }
          | tPUBLIC   tPROCEDURE tIDENTIFIER '('           ')' block      { $$ = new og::function_definition_node (LINE,  cdk::make_primitive_type(0, cdk::TYPE_VOID), *$3, new cdk::sequence_node(LINE),   $6, *$1); }
          | tPUBLIC   tPROCEDURE tIDENTIFIER '(' variables ')' block      { $$ = new og::function_definition_node (LINE,  cdk::make_primitive_type(0, cdk::TYPE_VOID), *$3,                           $5,   $7, *$1); }
          | tREQUIRE  tPROCEDURE tIDENTIFIER '('           ')'            { $$ = new og::function_declaration_node(LINE,  cdk::make_primitive_type(0, cdk::TYPE_VOID), *$3, new cdk::sequence_node(LINE),   *$1); }
          | tREQUIRE  tPROCEDURE tIDENTIFIER '(' variables ')'            { $$ = new og::function_declaration_node(LINE,  cdk::make_primitive_type(0, cdk::TYPE_VOID), *$3,                           $5,   *$1); }
          ;


/* -------------------------------------------------------------------------- */


/* Blocks ------------------------------------------------------------------- */

block      : '{'                                     '}' { $$ = new og::block_node(LINE, new cdk::sequence_node(LINE), new cdk::sequence_node(LINE)); }
           | '{' function_declarations               '}' { $$ = new og::block_node(LINE,                           $2, new cdk::sequence_node(LINE)); }
           | '{'                        instructions '}' { $$ = new og::block_node(LINE, new cdk::sequence_node(LINE),                           $2); }
           | '{' function_declarations  instructions '}' { $$ = new og::block_node(LINE,                           $2,                           $3); }
           ;

innerblock : instruction { $$ = $1;}
           | block       { $$ = $1;}
           ;

/* -------------------------------------------------------------------------- */


/* Declarations inside of functions ----------------------------------------- */

function_declaration  : variable ';' { $$ = $1; }
                      ;

function_declarations : function_declaration                        { $$ = new cdk::sequence_node(LINE, $1);     }
                      | function_declarations function_declaration  { $$ = new cdk::sequence_node(LINE, $2, $1); }
                      ;

/* -------------------------------------------------------------------------- */

/* Expressions -------------------------------------------------------------- */

expr        : tINTEGER                        { $$ = new cdk::integer_node(LINE, $1); }
      	    | strings                         { $$ = $1;                              }
            | tNULLPTR                        { $$ = new og::nullptr_node(LINE);      }
            | tREAL                           { $$ = new cdk::double_node(LINE, $1);  }
            | '-' expr %prec tUNARY           { $$ = new cdk::neg_node(LINE, $2);     }
            | '+' expr %prec tUNARY           { $$ = new og::identity_node(LINE, $2); }
            | expr '+' expr	                  { $$ = new cdk::add_node(LINE, $1, $3); }
            | expr '-' expr	                  { $$ = new cdk::sub_node(LINE, $1, $3); }
            | expr '*' expr	                  { $$ = new cdk::mul_node(LINE, $1, $3); }
            | expr '/' expr	                  { $$ = new cdk::div_node(LINE, $1, $3); }
            | expr '%' expr	                  { $$ = new cdk::mod_node(LINE, $1, $3); }
            | expr '<' expr	                  { $$ = new cdk::lt_node(LINE, $1, $3);  }
            | expr '>' expr	                  { $$ = new cdk::gt_node(LINE, $1, $3);  }
            | expr tGE expr	                  { $$ = new cdk::ge_node(LINE, $1, $3);  }
            | expr tLE expr                   { $$ = new cdk::le_node(LINE, $1, $3);  }
            | expr tNE expr	                  { $$ = new cdk::ne_node(LINE, $1, $3);  }
            | expr tEQ expr	                  { $$ = new cdk::eq_node(LINE, $1, $3);  }
            | expr tOR expr                   { $$ = new cdk::or_node(LINE, $1, $3);  }
            | expr tAND expr                  { $$ = new cdk::and_node(LINE, $1, $3); }
            | '(' expr ')'                    { $$ = $2;                              }
            | '[' expr ']'                    { $$ = new og::memory_alloc_node(LINE, $2); }
            | lvalue        '?'               { $$ = new og::address_node(LINE, $1);  }
            | lvalue                          { $$ = new cdk::rvalue_node(LINE, $1);  }
            | tIDENTIFIER '(' ')'             { $$ = new og::function_call_node(LINE, *$1, new cdk::sequence_node(LINE)); }
            | tIDENTIFIER '(' expressions ')' { $$ = new og::function_call_node(LINE, *$1,                           $3); }
            | lvalue '=' expr                 { $$ = new cdk::assignment_node(LINE, $1, $3); }
            | tSIZEOF '(' expressions ')'     { $$ = new og::sizeof_node(LINE, new og::tuple_node(LINE, $3)); }
            ;

expressions : expr                  { $$ = new cdk::sequence_node(LINE, $1);     }
            | expressions ',' expr  { $$ = new cdk::sequence_node(LINE, $3, $1); }
            ;

/* Others ------------------------------------------------------------------- */

lvalue      : tuple_index           { $$ = $1; }
            | indexation            { $$ = $1; }
            | identifier            { $$ = $1; }
            ;

indexation  : expr '[' expr ']'     { $$ = new og::index_node(LINE, $1, $3); }
            ;

tuple_index : expr '@' tINTEGER     { $$ = new og::tuple_index_node(LINE, $1, new cdk::integer_node(LINE, $3)); }
            ;

strings     : tSTRING               { $$ = new cdk::string_node(LINE, $1); }
            | strings tSTRING       { std::string sold($1->value()); sold.append(std::string(*$2)); $$ = new cdk::string_node(LINE,sold); delete $1; }
            ;

/* -------------------------------------------------------------------------- */
%%
