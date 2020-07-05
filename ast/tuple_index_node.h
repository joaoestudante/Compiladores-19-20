#ifndef __OG_TUPLE_INDEX_NODE_H__
#define __OG_TUPLE_INDEX_NODE_H__


#include <cdk/ast/lvalue_node.h>
#include "targets/basic_ast_visitor.h"

namespace og {
  /**
   * Class for describing the tuple index operator node.
   * An address operator is applied to, an expression, as such:
   *    a@1
   *    f(1)@2
   * The indexing is done only and only by an integer - it can't be an expression
   */
  class tuple_index_node: public cdk::lvalue_node {
    cdk::expression_node *_base;
    cdk::integer_node *_index;

  public:
    tuple_index_node(int lineno, cdk::expression_node *base, cdk::integer_node *index) :
        cdk::lvalue_node(lineno), _base(base), _index(index) {
    }

  public:
    cdk::expression_node *base() {
      return _base;
    }
    cdk::integer_node *index() {
      return _index;
    }

  public:
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_tuple_index_node(this, level);
    }

  };

} // og

#endif
