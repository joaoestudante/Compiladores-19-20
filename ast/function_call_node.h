#ifndef __OG_AST_FUNCTION_CALL_NODE_H___
#define __OG_AST_FUNCTION_CALL_NODE_H__

#include <cdk/ast/expression_node.h>

namespace og {

  /**
   * Class for describing a function call node.
   * A function call looks like:
   *     my_function(a1, a2)
   *
   * So, it has:
   *     - Function name
   *     - Sequence of arguments
   */
  class function_call_node: public cdk::expression_node {
    std::string        _function_name;
    cdk::sequence_node *_arguments;

  public:
    inline function_call_node(int lineno, std::string function_name, cdk::sequence_node *arguments) :
        cdk::expression_node(lineno),  _function_name(function_name), _arguments(arguments) {
    }

  public:
    inline std::string &function_name() { return _function_name; }
    inline cdk::sequence_node *arguments() { return _arguments; }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_function_call_node(this, level);
    }

  };
} // og

#endif
