#include <string>
#include "targets/xml_writer.h"
#include "targets/type_checker.h"
#include "ast/all.h"  // automatically generated

//---------------------------------------------------------------------------

void og::xml_writer::do_nil_node(cdk::nil_node * const node, int lvl) {
  openTag(node, lvl);
  closeTag(node, lvl);
}
void og::xml_writer::do_data_node(cdk::data_node * const node, int lvl) {
  os() << std::string(lvl, ' ')
  << "<data_node size='"
  << node->size()
  << " bucket ="
  << node->bucket()
  << "'>" << std::endl;
  closeTag(node, lvl);
}
void og::xml_writer::do_double_node(cdk::double_node * const node, int lvl) {
  process_literal(node, lvl);
}
void og::xml_writer::do_not_node(cdk::not_node * const node, int lvl) {
  do_unary_operation(node, lvl);
}
void og::xml_writer::do_and_node(cdk::and_node * const node, int lvl) {
  do_binary_operation(node, lvl);
}
void og::xml_writer::do_or_node(cdk::or_node * const node, int lvl) {
  do_binary_operation(node, lvl);
}

//---------------------------------------------------------------------------

void og::xml_writer::do_sequence_node(cdk::sequence_node * const node, int lvl) {
  os() << std::string(lvl, ' ') << "<sequence_node size='" << node->size() << "'>" << std::endl;
  for (size_t i = 0; i < node->size(); i++) {
    node->node(i)->accept(this, lvl + 2);
  }
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void og::xml_writer::do_integer_node(cdk::integer_node * const node, int lvl) {
  process_literal(node, lvl);
}

void og::xml_writer::do_string_node(cdk::string_node * const node, int lvl) {
  process_literal(node, lvl);
}

//---------------------------------------------------------------------------

void og::xml_writer::do_unary_operation(cdk::unary_operation_node * const node, int lvl) {
  // ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  node->argument()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

void og::xml_writer::do_neg_node(cdk::neg_node * const node, int lvl) {
  do_unary_operation(node, lvl);
}

//---------------------------------------------------------------------------

void og::xml_writer::do_binary_operation(cdk::binary_operation_node * const node, int lvl) {
  // ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  node->left()->accept(this, lvl + 2);
  node->right()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

void og::xml_writer::do_add_node(cdk::add_node * const node, int lvl) {
  do_binary_operation(node, lvl);
}
void og::xml_writer::do_sub_node(cdk::sub_node * const node, int lvl) {
  do_binary_operation(node, lvl);
}
void og::xml_writer::do_mul_node(cdk::mul_node * const node, int lvl) {
  do_binary_operation(node, lvl);
}
void og::xml_writer::do_div_node(cdk::div_node * const node, int lvl) {
  do_binary_operation(node, lvl);
}
void og::xml_writer::do_mod_node(cdk::mod_node * const node, int lvl) {
  do_binary_operation(node, lvl);
}
void og::xml_writer::do_lt_node(cdk::lt_node * const node, int lvl) {
  do_binary_operation(node, lvl);
}
void og::xml_writer::do_le_node(cdk::le_node * const node, int lvl) {
  do_binary_operation(node, lvl);
}
void og::xml_writer::do_ge_node(cdk::ge_node * const node, int lvl) {
  do_binary_operation(node, lvl);
}
void og::xml_writer::do_gt_node(cdk::gt_node * const node, int lvl) {
  do_binary_operation(node, lvl);
}
void og::xml_writer::do_ne_node(cdk::ne_node * const node, int lvl) {
  do_binary_operation(node, lvl);
}
void og::xml_writer::do_eq_node(cdk::eq_node * const node, int lvl) {
  do_binary_operation(node, lvl);
}

//---------------------------------------------------------------------------

void og::xml_writer::do_variable_node(cdk::variable_node * const node, int lvl) {
  // ASSERT_SAFE_EXPRESSIONS;
  os() << std::string(lvl, ' ')
  << "<" << node->label()
  << ">" << node->name()
  << "</" << node->label()
  << ">" << std::endl;
}

void og::xml_writer::do_rvalue_node(cdk::rvalue_node * const node, int lvl) {
  // ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  node->lvalue()->accept(this, lvl + 4);
  closeTag(node, lvl);
}

void og::xml_writer::do_assignment_node(cdk::assignment_node * const node, int lvl) {
  // ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);

  node->lvalue()->accept(this, lvl);
  reset_new_symbol();

  node->rvalue()->accept(this, lvl + 4);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

#if 0
void og::xml_writer::do_program_node(og::program_node * const node, int lvl) {
  openTag(node, lvl);
  node->statements()->accept(this, lvl + 4);
  closeTag(node, lvl);
}
#endif

//---------------------------------------------------------------------------

void og::xml_writer::do_evaluation_node(og::evaluation_node * const node, int lvl) {
  // ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  node->argument()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

#if 0
void og::xml_writer::do_print_node(og::print_node * const node, int lvl) {
  // ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  node->argument()->accept(this, lvl + 2);
  closeTag(node, lvl);
}
#endif

//---------------------------------------------------------------------------

#if 0
void og::xml_writer::do_read_node(og::read_node * const node, int lvl) {
  // ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  node->argument()->accept(this, lvl + 2);
  closeTag(node, lvl);
}
#endif

//---------------------------------------------------------------------------

#if 0
void og::xml_writer::do_while_node(og::while_node * const node, int lvl) {
  // ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  openTag("condition", lvl + 2);
  node->condition()->accept(this, lvl + 4);
  closeTag("condition", lvl + 2);
  openTag("block", lvl + 2);
  node->block()->accept(this, lvl + 4);
  closeTag("block", lvl + 2);
  closeTag(node, lvl);
}
#endif

//---------------------------------------------------------------------------

void og::xml_writer::do_if_node(og::if_node * const node, int lvl) {
  // ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  openTag("condition", lvl + 2);
  node->condition()->accept(this, lvl + 4);
  closeTag("condition", lvl + 2);
  openTag("then", lvl + 2);
  node->block()->accept(this, lvl + 4);
  closeTag("then", lvl + 2);
  closeTag(node, lvl);
}

void og::xml_writer::do_if_else_node(og::if_else_node * const node, int lvl) {
  // ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  openTag("condition", lvl + 2);
  node->condition()->accept(this, lvl + 4);
  closeTag("condition", lvl + 2);
  openTag("then", lvl + 2);
  node->thenblock()->accept(this, lvl + 4);
  closeTag("then", lvl + 2);
  openTag("else", lvl + 2);
  node->elseblock()->accept(this, lvl + 4);
  closeTag("else", lvl + 2);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void og::xml_writer::do_for_node(og::for_node * const node, int lvl) {
    openTag(node, lvl);
    openTag("declaration", lvl + 2);
    node->declaration()->accept(this, lvl + 4);
    closeTag("declaration", lvl + 2);
    openTag("condition", lvl + 2);
    node->condition()->accept(this, lvl + 4);
    closeTag("condition", lvl + 2);
    openTag("increment", lvl + 2);
    node->increment()->accept(this, lvl + 4);
    closeTag("increment", lvl + 2);
    openTag("block", lvl + 2);
    node->block()->accept(this, lvl + 4);
    closeTag("block", lvl + 2);
    closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void og::xml_writer::do_function_declaration_node(og::function_declaration_node *const node, int lvl) {
    os() << std::string(lvl, ' ')
    << "<function_declaration_node "
    << "name = '"  << node->function_name() << "' "
    << "scope = '" << node->scope() << "' "
    << "type = '"  << cdk::to_string(node->return_type()) << "'>" << std::endl;
    node->arguments()->accept(this, lvl + 2);
    closeTag("function_declaration_node", lvl);
}
void og::xml_writer::do_function_definition_node(og::function_definition_node *const node, int lvl) {
    os() << std::string(lvl, ' ')
    << "<function_definition_node "
    << "name = '"  << node->function_name() << "' "
    << "scope = '" << node->scope() << "' "
    << "type = '"  << cdk::to_string(node->type()) << "'>" << std::endl;
    node->arguments()->accept(this, lvl + 2);
    node->statements()->accept(this, lvl + 4);
    closeTag("function_definition_node", lvl);
}
void og::xml_writer::do_function_call_node(og::function_call_node *const node, int lvl) {
  os() << std::string(lvl, ' ')
  << "<function_call_node "
  << "name = '"  << node->function_name() << "'>" << std::endl;
  openTag("arguments", lvl + 2);
  node->arguments()->accept(this, lvl + 4);
  closeTag("arguments", lvl + 2);
  closeTag("function_call_node", lvl);
}
void og::xml_writer::do_variable_declaration_node(og::variable_declaration_node *const node, int lvl) {
    // ASSERT_SAFE_EXPRESSIONS;
    os() << std::string(lvl, ' ')
    << "<" << "variable_declaration_node name='"
    << node->variable_name()
    << "' scope='" << node->scope()
    << "' type='" << cdk::to_string(node->variable_type()) << "'>" << std::endl;
    if(node->initial_val())
        node->initial_val()->accept(this, lvl + 2);
    closeTag("variable_declaration_node", lvl);
}
void og::xml_writer::do_address_node(og::address_node *const node, int lvl) {
  openTag(node, lvl);
  openTag("lvalue", lvl + 2);
  node->lvalue()->accept(this, lvl + 4);
  closeTag("lvalue", lvl + 2);
  closeTag(node, lvl);
}
void og::xml_writer::do_index_node(og::index_node *const node, int lvl) {
  openTag(node, lvl);
  openTag("base", lvl + 2);
  node->base()->accept(this, lvl + 4);
  closeTag("base", lvl + 2);
  openTag("index", lvl + 2);
  node->index()->accept(this, lvl + 4);
  closeTag("index", lvl + 2);
  closeTag(node, lvl);
}
void og::xml_writer::do_tuple_index_node(og::tuple_index_node *const node, int lvl) {
  openTag(node, lvl);
  openTag("base", lvl + 2);
  node->base()->accept(this, lvl + 4);
  closeTag("base", lvl + 2);
  openTag("index", lvl + 2);
  node->index()->accept(this, lvl + 4);
  closeTag("index", lvl + 2);
  closeTag(node, lvl);
}
void og::xml_writer::do_memory_alloc_node(og::memory_alloc_node *const node, int lvl) {
  do_unary_operation(node, lvl);
}


void og::xml_writer::do_break_node(og::break_node *const node, int lvl) {
  // ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  closeTag(node, lvl);
}
void og::xml_writer::do_continue_node(og::continue_node *const node, int lvl) {
  // ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  closeTag(node, lvl);
}
void og::xml_writer::do_return_node(og::return_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  node->argument()->accept(this, lvl + 2);
  closeTag(node, lvl);
}
void og::xml_writer::do_input_node(og::input_node *const node, int lvl) {
  openTag(node, lvl);
  closeTag(node, lvl);
}
void og::xml_writer::do_write_node(og::write_node *const node, int lvl) {
  openTag(node, lvl);
  node->sequence()->accept(this, lvl + 2);
  closeTag(node, lvl);
}
void og::xml_writer::do_writeln_node(og::writeln_node *const node, int lvl) {
  openTag(node, lvl);
  node->sequence()->accept(this, lvl + 2);
  closeTag(node, lvl);
}
void og::xml_writer::do_sizeof_node(og::sizeof_node *const node, int lvl) {
  openTag(node, lvl);
  node->argument()->accept(this, lvl + 2);
  closeTag(node, lvl);
}
void og::xml_writer::do_nullptr_node(og::nullptr_node *const node, int lvl) {
  openTag(node, lvl);
  closeTag(node, lvl);
}
void og::xml_writer::do_tuple_node(og::tuple_node *const node, int lvl) {
  openTag(node, lvl);
  node->contents()->accept(this, lvl + 2);
  closeTag(node, lvl);
}
void og::xml_writer::do_block_node(og::block_node *const node, int lvl) {
    openTag(node, lvl);
    node->decs()->accept(this, lvl + 4);
    node->instructs()->accept(this, lvl + 4);
    closeTag(node, lvl);
}
void og::xml_writer::do_identity_node(og::identity_node *const node, int lvl) {
    do_unary_operation(node, lvl);
}
