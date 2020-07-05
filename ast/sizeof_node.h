
#ifndef __OG_AST_SIZEOF_NODE_H__
#define __OG_AST_SIZEOF_NODE_H__

#include <cdk/ast/expression_node.h>
#include <cdk/ast/basic_node.h>

namespace og {

    /**
     * Class for describing sizeof nodes.
     */
    class sizeof_node: public cdk::expression_node {
        cdk::expression_node *_argument;

        public:
            inline sizeof_node(int lineno, cdk::expression_node *argument) :
                    cdk::expression_node(lineno), _argument(argument) {
            }

        public:
            inline cdk::expression_node *argument() {
                return _argument;
                /* return  _size */
            }

            void accept(basic_ast_visitor *sp, int level) {
                sp->do_sizeof_node(this, level);
            }
        };
} // og

#endif
