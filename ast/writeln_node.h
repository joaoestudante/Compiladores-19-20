
#ifndef __OG_AST_WRITELN_NODE_H__
#define __OG_AST_WRITELN_NODE_H__

#include <cdk/ast/sequence_node.h>
#include <cdk/ast/expression_node.h>
#include <cdk/ast/basic_node.h>

namespace og {

    /**
     * Class for describing write new line node.
     */
    class writeln_node: public cdk::basic_node {
        cdk::sequence_node *_sequence;

    public:
        inline writeln_node(int lineno, cdk::sequence_node *sequence) :
                cdk::basic_node(lineno), _sequence(sequence) {
        }

    public:
        inline cdk::sequence_node *sequence() {
            return _sequence;
        }

    public:
        void accept(basic_ast_visitor *sp, int level) {
            sp->do_writeln_node(this, level);
        }
    };
} // og

#endif
