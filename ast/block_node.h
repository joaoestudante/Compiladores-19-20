#ifndef __OG_AST_BLOCK_H__
#define __OG_AST_BLOCK_H__

#include <cdk/ast/basic_node.h>

namespace og {

  class block_node: public cdk::basic_node {
    cdk::sequence_node *_instructs;
    cdk::sequence_node *_decs;

  public:
    block_node(int lineno, cdk::sequence_node *decs, cdk::sequence_node *instructs) :
        cdk::basic_node(lineno), _instructs(instructs), _decs(decs) {
    }

  public:
    cdk::sequence_node *instructs() {
      return _instructs;
    }
    cdk::sequence_node *decs() {
      return _decs;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_block_node(this, level);
    }
  };
} // og

#endif
