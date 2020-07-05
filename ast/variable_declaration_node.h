#ifndef __OG_AST_VARIABLE_DECLARATION_NODE_H__
#define __OG_AST_VARIABLE_DECLARATION_NODE_H__

#include <cdk/ast/basic_node.h>

namespace og {

  /**
   * Class for describing variable declaration nodes.
   * A variable declaration looks like:
   *     int my_variable
   *
   * So, it has:
   *     - Type
   *     - Variable name
   */
  class variable_declaration_node: public cdk::expression_node {
    std::shared_ptr<cdk::basic_type> _variable_type;
    std::string          _variable_name;
    std::string          _scope;
    cdk::expression_node *_initial_val;


  public: /* Maybe pass the basic type and THEN make the shared pointer :) */
    inline variable_declaration_node(int lineno, std::shared_ptr<cdk::basic_type> variable_type, const std::string &variable_name, std::string scope, cdk::expression_node *initial_val) :
        expression_node(lineno), _variable_type(variable_type), _variable_name(variable_name), _scope(scope), _initial_val(initial_val) {
            type(variable_type);
    }


  public:
    inline std::shared_ptr<cdk::basic_type> variable_type() { return _variable_type; }
    inline void set_variable_type(std::shared_ptr<cdk::basic_type> type) { _variable_type = type;}
    inline std::string &variable_name() { return _variable_name; }
    inline std::string &scope() { return _scope; }
    inline cdk::expression_node *initial_val() { return _initial_val; }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_variable_declaration_node(this, level);
    }

  };

} // og

#endif
