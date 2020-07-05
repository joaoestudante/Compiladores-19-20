#ifndef __OG_AST_FUNCTION_DECLARATION_NODE_H__
#define __OG_AST_FUNCTION_DECLARATION_NODE_H__

#include <cdk/ast/basic_node.h>

namespace og {

  /**
   * Class for describing function declaration nodes.
   * A function declaration looks like:
   *     int my_function(int a1, float a2)
   *
   * So, it has:
   *     - Return type
   *     - Function name
   *     - Sequence of arguments
   */
  class function_declaration_node: public cdk::basic_node {
    std::shared_ptr<cdk::basic_type>    _return_type;
    std::string        _function_name;
    cdk::sequence_node *_arguments;
    std::string         _scope;

  public:
    inline function_declaration_node(int lineno, std::shared_ptr<cdk::basic_type> return_type, std::string function_name,
		    cdk::sequence_node *arguments, std::string scope) :
        cdk::basic_node(lineno), _return_type(return_type), _function_name(function_name),
       _arguments(arguments), _scope(scope){
    }

  public:
    inline std::shared_ptr<cdk::basic_type> return_type() { return _return_type; }
    inline std::string &function_name() { return _function_name; }
    inline cdk::sequence_node *arguments() { return _arguments; }
    inline std::string &scope() { return _scope; }


    void accept(basic_ast_visitor *sp, int level) {
      sp->do_function_declaration_node(this, level);
    }

  };

} // og

#endif
