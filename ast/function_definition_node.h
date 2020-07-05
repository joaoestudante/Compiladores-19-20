#ifndef __OG_AST_FUNCTION_DEFINITION_NODE_H___
#define __OG_AST_FUNCTION_DEFINITION_NODE_H__

#include <cdk/ast/basic_node.h>
#include "ast/block_node.h"

namespace og {

  /**
   * Class for describing a function definition node.
   * A function definition looks like:
   *     int my_function(int a1, float a2) { ... }
   *
   * So, it has:
   *     - Return type
   *     - Function name
   *     - Sequence of arguments
   *     - Body statements
   */
  class function_definition_node: public cdk::typed_node {
    //std::shared_ptr<cdk::basic_type>    *_return_type;
    std::string        _function_name;
    cdk::sequence_node *_arguments;
    og::block_node    *_statements;
    std::string         _scope;



  public:
    inline function_definition_node(int lineno, std::shared_ptr<cdk::basic_type> return_type, std::string function_name,
		    cdk::sequence_node *arguments, og::block_node *statements, std::string scope) :
        cdk::typed_node(lineno), _function_name(function_name),
       _arguments(arguments), _statements(statements), _scope(scope) {
           type(return_type);
    }

  public:
    //inline std::shared_ptr<cdk::basic_type> return_type() { return *_return_type; }
    inline std::string &function_name() { return _function_name; }
    inline cdk::sequence_node *arguments() { return _arguments; }
    inline og::block_node *statements() { return _statements; }
    inline std::string &scope() { return _scope; }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_function_definition_node(this, level);
    }

  };

} // og

#endif
