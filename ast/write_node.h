
#ifndef __OG_AST_WRITE_NODE_H__
#define __OG_AST_WRITE_NODE_H__

#include <cdk/ast/basic_node.h>
#include <cdk/ast/expression_node.h>

namespace og {

    /**
     * Class for describing write node.
     */
    class write_node: public cdk::basic_node {
        cdk::sequence_node *_sequence;

        public:
            inline write_node(int lineno, cdk::sequence_node *sequence) :
                    cdk::basic_node(lineno), _sequence(sequence) {
            }

        public:
            inline cdk::sequence_node *sequence() {
                return _sequence;
            }

        public:
            void accept(basic_ast_visitor *sp, int level) {
                sp->do_write_node(this, level);
            }
        };
} // og

#endif
