#ifndef __OG_AST_NEG_NODE_H__
#define __OG_AST_NEG_NODE_H__

#include <cdk/ast/unary_operation_node.h>

namespace og {

  class identity_node: public cdk::unary_operation_node {
  public:
    identity_node(int lineno, expression_node *arg) :
        unary_operation_node(lineno, arg) {
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_identity_node(this, level);
    }

  };

} // og

#endif
