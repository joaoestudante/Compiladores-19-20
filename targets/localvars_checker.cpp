#include <string>
#include "targets/localvars_checker.h"
#include "targets/type_checker.h"
#include "ast/all.h"  // automatically generated


void og::localvars_checker::do_sequence_node(cdk::sequence_node *const node, int lvl) {
  for (auto n : node->nodes()) n->accept(this, lvl);
}

void og::localvars_checker::do_variable_declaration_node(og::variable_declaration_node *const node, int lvl) {
    ASSERT_SAFE_EXPRESSIONS;

    if(node->is_typed(cdk::TYPE_UNSPEC)) {
        if (node->initial_val()->type())
            this->incrLocalVars(node->initial_val()->type()->size());
        else
            throw std::string("initial val in auto variable declaration did not have type");
    } else {
        this->incrLocalVars(node->type()->size());
    }
}

void og::localvars_checker::do_for_node(og::for_node *const node, int lvl) {
    node->declaration()->accept(this, lvl);
    node->block()->accept(this, lvl);
}

void og::localvars_checker::do_function_definition_node(og::function_definition_node *const node, int lvl) {
    node->statements()->accept(this, lvl);
}

void og::localvars_checker::do_block_node(og::block_node *const node, int lvl) {
    node->decs()->accept(this, lvl);
    node->instructs()->accept(this, lvl);
}

void og::localvars_checker::do_if_node(og::if_node *const node, int lvl) {
    node->condition()->accept(this, lvl);
}

void og::localvars_checker::do_if_else_node(og::if_else_node *const node, int lvl) {
    node->condition()->accept(this, lvl);
}

/*  No need to do anything in the rest of the nodes, because there are no
 *  declarations there.
 */

void og::localvars_checker::do_nil_node(cdk::nil_node *const node, int lvl) {
}
void og::localvars_checker::do_data_node(cdk::data_node *const node, int lvl) {
}
void og::localvars_checker::do_double_node(cdk::double_node *const node, int lvl) {
}
void og::localvars_checker::do_not_node(cdk::not_node *const node, int lvl) {
}
void og::localvars_checker::do_and_node(cdk::and_node *const node, int lvl) {
}
void og::localvars_checker::do_or_node(cdk::or_node *const node, int lvl) {
}
void og::localvars_checker::do_integer_node(cdk::integer_node *const node, int lvl) {
}
void og::localvars_checker::do_string_node(cdk::string_node *const node, int lvl) {
}
void og::localvars_checker::do_neg_node(cdk::neg_node *const node, int lvl) {
}
void og::localvars_checker::do_identity_node(og::identity_node *const node, int lvl) {
}
void og::localvars_checker::do_add_node(cdk::add_node *const node, int lvl) {
}
void og::localvars_checker::do_sub_node(cdk::sub_node *const node, int lvl) {
}
void og::localvars_checker::do_mul_node(cdk::mul_node *const node, int lvl) {
}
void og::localvars_checker::do_div_node(cdk::div_node *const node, int lvl) {
}
void og::localvars_checker::do_mod_node(cdk::mod_node *const node, int lvl) {
}
void og::localvars_checker::do_lt_node(cdk::lt_node *const node, int lvl) {
}
void og::localvars_checker::do_le_node(cdk::le_node *const node, int lvl) {
}
void og::localvars_checker::do_ge_node(cdk::ge_node *const node, int lvl) {
}
void og::localvars_checker::do_gt_node(cdk::gt_node *const node, int lvl) {
}
void og::localvars_checker::do_ne_node(cdk::ne_node *const node, int lvl) {
}
void og::localvars_checker::do_eq_node(cdk::eq_node *const node, int lvl) {
}
void og::localvars_checker::do_variable_node(cdk::variable_node *const node, int lvl) {
}
void og::localvars_checker::do_rvalue_node(cdk::rvalue_node *const node, int lvl) {
}
void og::localvars_checker::do_assignment_node(cdk::assignment_node *const node, int lvl) {
}
void og::localvars_checker::do_evaluation_node(og::evaluation_node *const node, int lvl) {
}
void og::localvars_checker::do_function_declaration_node(og::function_declaration_node *const node, int lvl) {
}
void og::localvars_checker::do_function_call_node(og::function_call_node *const node, int lvl) {
}
void og::localvars_checker::do_address_node(og::address_node *const node, int lvl) {
}
void og::localvars_checker::do_index_node(og::index_node *const node, int lvl) {
}
void og::localvars_checker::do_tuple_index_node(og::tuple_index_node *const node, int lvl) {
}
void og::localvars_checker::do_memory_alloc_node(og::memory_alloc_node *const node, int lvl) {
}
void og::localvars_checker::do_break_node(og::break_node *const node, int lvl) {
}
void og::localvars_checker::do_continue_node(og::continue_node *const node, int lvl) {
}
void og::localvars_checker::do_return_node(og::return_node *const node, int lvl) {
}
void og::localvars_checker::do_input_node(og::input_node *const node, int lvl) {
}
void og::localvars_checker::do_write_node(og::write_node *const node, int lvl) {
}
void og::localvars_checker::do_writeln_node(og::writeln_node *const node, int lvl) {
}
void og::localvars_checker::do_sizeof_node(og::sizeof_node *const node, int lvl) {
}
void og::localvars_checker::do_nullptr_node(og::nullptr_node *const node, int lvl) {
}
void og::localvars_checker::do_tuple_node(og::tuple_node *const node, int lvl) {
}
