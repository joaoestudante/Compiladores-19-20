#ifndef __OG_TARGETS_LOCALVARS_CHECKER_H__
#define __OG_TARGETS_LOCALVARS_CHECKER_H__

#include "targets/basic_ast_visitor.h"

namespace og {

  class localvars_checker: public basic_ast_visitor {
    cdk::symbol_table<og::symbol> &_symtab;

    basic_ast_visitor *_parent;
    int _localvars = 0;

  public:
    localvars_checker(std::shared_ptr<cdk::compiler> compiler, cdk::symbol_table<og::symbol> &symtab, basic_ast_visitor *parent) :
        basic_ast_visitor(compiler), _symtab(symtab), _parent(parent) {
    }

  public:
    ~localvars_checker() {
      os().flush();
    }

    void incrLocalVars(int incr) { _localvars = _localvars + incr; }

    int localVarsAmount() { return _localvars; }


  public:
    // do not edit these lines
#define __IN_VISITOR_HEADER__
#include "ast/visitor_decls.h"       // automatically generated
#undef __IN_VISITOR_HEADER__
    // do not edit these lines: end

  };

} // og

//---------------------------------------------------------------------------
//     HELPER MACRO FOR TYPE CHECKING 
//---------------------------------------------------------------------------

#define CHECK_TYPES(compiler, symtab, node) { \
  try { \
    og::type_checker checker(compiler, symtab, this); \
    (node)->accept(&checker, 0); \
  } \
  catch (const std::string &problem) { \
    std::cerr << (node)->lineno() << ": " << problem << std::endl; \
    return; \
  } \
}

#define ASSERT_SAFE_EXPRESSIONS CHECK_TYPES(_compiler, _symtab, node)

#endif
