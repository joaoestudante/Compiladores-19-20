#ifndef __OG_AST_TUPLE_NODE_H__
#define __OG_AST_TUPLE_NODE_H__

#include <cdk/ast/basic_node.h>

namespace og {

/**
 * Class for describing tuple nodes.
 * A tuple contains several elements inside.
 */
    class tuple_node: public cdk::expression_node {
      cdk::sequence_node *_contents;

    public:
        tuple_node(int lineno, cdk::sequence_node *contents) :
          cdk::expression_node(lineno), _contents(contents){
        }

    public:
      cdk::sequence_node *contents() {
        return _contents;
    }


    public:
        void accept(basic_ast_visitor *sp, int level) {
            sp->do_tuple_node(this, level);
        }
    };
} // og

#endif
