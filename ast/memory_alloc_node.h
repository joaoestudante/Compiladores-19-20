#ifndef __OG_MEMORY_ALLOC_NODE_H__
#define __OG_MEMORY_ALLOC_NODE_H__

#include <cdk/ast/expression_node.h>
#include <cdk/ast/unary_operation_node.h>

namespace og {

  /**
   * Class for describing the memory allocation node.
   * Memory is 'allocced' like so:
   *   ptr<type>varname = [#things stored]
   */
  class memory_alloc_node: public cdk::unary_operation_node {

  public:
    memory_alloc_node(int lineno, expression_node *amount) :
        cdk::unary_operation_node(lineno, amount) {
    }

  public:
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_memory_alloc_node(this, level);
    }

  };

} // og

#endif
