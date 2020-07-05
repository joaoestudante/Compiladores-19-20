#include <string>
#include "targets/type_checker.h"
#include "ast/all.h"  // automatically generated
#include <cdk/types/primitive_type.h>

#define ASSERT_UNSPEC { if (node->type() != nullptr && !node->is_typed(cdk::TYPE_UNSPEC)) return; }

std::shared_ptr<og::symbol> v_symbol;
std::shared_ptr<og::symbol> tc_current_function;

std::vector<std::shared_ptr<cdk::basic_type>> get_args_types(std::vector<cdk::basic_node*> nodes) {
    /* This method is responsible for finding the types of a given vector of nodes.
     * It is meant to be used after registering a symbol for a function definition
     * or declaration.
     * This method cannot exist inside the symbol.h file due to problems with importing
     * nodes.
     */
    std::vector<std::shared_ptr<cdk::basic_type>> types = std::vector<std::shared_ptr<cdk::basic_type>>();
    for (cdk::basic_node* n : nodes) {
        // they're variable declarations, aka expressions - and we're only interested in the type
        cdk::expression_node *arg = dynamic_cast<cdk::expression_node*>(n);
        types.push_back(arg->type());
    }
    return types;
}


//---------------------------------------------------------------------------

void og::type_checker::do_sequence_node(cdk::sequence_node *const node, int lvl) {
  for (auto n : node->nodes()) n->accept(this, lvl + 2);
}


// Types --------------------------------------------------------------------

void og::type_checker::do_integer_node(cdk::integer_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(cdk::make_primitive_type(4, cdk::TYPE_INT));
}

void og::type_checker::do_string_node(cdk::string_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(cdk::make_primitive_type(4, cdk::TYPE_STRING));
}

void og::type_checker::do_double_node(cdk::double_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(cdk::make_primitive_type(8, cdk::TYPE_DOUBLE));
}

void og::type_checker::do_nullptr_node(og::nullptr_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(cdk::make_primitive_type(0, cdk::TYPE_VOID));
}

void og::type_checker::do_tuple_node(og::tuple_node *const node, int lvl) {
    ASSERT_UNSPEC;
    node->contents()->accept(this, lvl + 2);
    std::vector<std::shared_ptr<cdk::basic_type>> *types_vector = new std::vector<std::shared_ptr<cdk::basic_type>>();
    for (auto e : node->contents()->nodes()) {
        auto e_expression = static_cast<cdk::expression_node*>(e);
        types_vector->push_back(e_expression->type());
    }
    node->type(cdk::make_structured_type(*types_vector));
}

// Unary operations ----------------------------------------------------------

void og::type_checker::processUnaryExpression(cdk::unary_operation_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->argument()->accept(this, lvl + 2);
  node->type(node->argument()->type());
}

void og::type_checker::do_neg_node(cdk::neg_node *const node, int lvl) {
  processUnaryExpression(node, lvl);
}

void og::type_checker::do_identity_node(og::identity_node *const node, int lvl) {
  processUnaryExpression(node, lvl);
}

void og::type_checker::do_not_node(cdk::not_node *const node, int lvl) {
    ASSERT_UNSPEC;
    processUnaryExpression(node, lvl);
    node->type(cdk::make_primitive_type(4, cdk::TYPE_INT));
}

// Binary operations ---------------------------------------------------------

void og::type_checker::processExpression(cdk::binary_operation_node *const node, int lvl){
    ASSERT_UNSPEC;

  // Processes a binary operation where no elements may be a pointer.

  auto left = node->left();
  auto right = node->right();

  left->accept(this, lvl);
  if (!left->is_typed(cdk::TYPE_INT) && !left->is_typed(cdk::TYPE_DOUBLE))
     throw std::string("unsupported type in left of binary expression: " + cdk::to_string(left->type()));

  right->accept(this, lvl);
  if (!right->is_typed(cdk::TYPE_INT) && !right->is_typed(cdk::TYPE_DOUBLE))
    throw std::string("unsupported type in right of binary expression: " + cdk::to_string(right->type()));

  node->type(left->type());
}

void og::type_checker::processExpressionWithPointer(cdk::binary_operation_node *const node, int lvl){
  ASSERT_UNSPEC;

  // Processes a binary operation where one of the elements may be a pointer.

  auto left = node->left();
  auto right = node->right();

  left->accept(this, lvl);
  if (!left->is_typed(cdk::TYPE_INT) && !left->is_typed(cdk::TYPE_DOUBLE) && !left->is_typed(cdk::TYPE_POINTER))
     throw std::string("unsupported type in left of binary expression: '" + cdk::to_string(left->type()) + "', was expecting int, double, or pointer");

  right->accept(this, lvl);
  if (!right->is_typed(cdk::TYPE_INT) && !right->is_typed(cdk::TYPE_DOUBLE) && !right->is_typed(cdk::TYPE_POINTER))
  throw std::string("unsupported type in right of binary expression: '" + cdk::to_string(right->type()) + "', was expecting int, double, or pointer");


  if (
      (left->is_typed(cdk::TYPE_DOUBLE) && right->is_typed(cdk::TYPE_POINTER)) ||
      (left->is_typed(cdk::TYPE_POINTER) && right->is_typed(cdk::TYPE_DOUBLE))
  ) {
      throw std::string("cannot perform binary expression with double and pointer");
  }

  node->type(left->type());
}

void og::type_checker::do_add_node(cdk::add_node *const node, int lvl) {
  processExpressionWithPointer(node, lvl);
}

void og::type_checker::do_sub_node(cdk::sub_node *const node, int lvl) {
  processExpressionWithPointer(node, lvl);
}

void og::type_checker::do_mul_node(cdk::mul_node *const node, int lvl) {
  processExpression(node, lvl);
}

void og::type_checker::do_div_node(cdk::div_node *const node, int lvl) {
  processExpression(node, lvl);
}

void og::type_checker::do_mod_node(cdk::mod_node *const node, int lvl) {
  processExpression(node, lvl);
}

void og::type_checker::do_lt_node(cdk::lt_node *const node, int lvl) {
  processExpression(node, lvl);
}

void og::type_checker::do_le_node(cdk::le_node *const node, int lvl) {
  processExpression(node, lvl);
}

void og::type_checker::do_ge_node(cdk::ge_node *const node, int lvl) {
  processExpression(node, lvl);
}

void og::type_checker::do_gt_node(cdk::gt_node *const node, int lvl) {
  processExpression(node, lvl);
}

void og::type_checker::do_ne_node(cdk::ne_node *const node, int lvl) {
  processExpressionWithPointer(node, lvl);
}

void og::type_checker::do_eq_node(cdk::eq_node *const node, int lvl) {
  processExpressionWithPointer(node, lvl);
}

void og::type_checker::processBinaryExpression(cdk::binary_operation_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  if (!node->left()->is_typed(cdk::TYPE_INT)) throw std::string("wrong type in left argument of binary expression");

  node->right()->accept(this, lvl + 2);
  if (!node->right()->is_typed(cdk::TYPE_INT)) throw std::string("wrong type in right argument of binary expression");

  node->type(cdk::make_primitive_type(4, cdk::TYPE_INT));
}

void og::type_checker::do_and_node(cdk::and_node *const node, int lvl) {
    ASSERT_UNSPEC;
    processBinaryExpression(node, lvl);
    node->type(cdk::make_primitive_type(4, cdk::TYPE_INT));
}

void og::type_checker::do_or_node(cdk::or_node *const node, int lvl) {
    ASSERT_UNSPEC;
    processBinaryExpression(node, lvl);
    node->type(cdk::make_primitive_type(4, cdk::TYPE_INT));
}

//---------------------------------------------------------------------------

void og::type_checker::do_variable_node(cdk::variable_node *const node, int lvl) {
  ASSERT_UNSPEC;
  const std::string &id = node->name();
  std::shared_ptr<og::symbol> symbol = _symtab.find(id);
  if (symbol != nullptr) {
    node->type(symbol->type());
    v_symbol = symbol;
  } else
    throw id;

}

void og::type_checker::do_rvalue_node(cdk::rvalue_node *const node, int lvl) {
  ASSERT_UNSPEC;
  try {
    node->lvalue()->accept(this, lvl);
    node->type(node->lvalue()->type());
  } catch (const std::string &id) {
    throw "undeclared variable '" + id + "'";
  }
}

void og::type_checker::do_assignment_node(cdk::assignment_node *const node, int lvl) {
  ASSERT_UNSPEC;
  try {
    node->lvalue()->accept(this, lvl);
  } catch (const std::string &id) {
    auto symbol = std::make_shared<og::symbol>(cdk::make_primitive_type(4, cdk::TYPE_INT), id, false);
    _symtab.insert(id, symbol);
    _parent->set_new_symbol(symbol);  // advise parent that a symbol has been inserted
    node->lvalue()->accept(this, lvl);  //DAVID: bah!
  }

  node->rvalue()->accept(this, lvl + 2);

  if(node->lvalue()->is_typed(cdk::TYPE_INT)) {

      if(node->rvalue()->is_typed(cdk::TYPE_INT) || node->rvalue()->is_typed(cdk::TYPE_DOUBLE)) /* int = int or double, assignment is int */
          node->type(cdk::make_primitive_type(4, cdk::TYPE_INT));

  } else if (node->lvalue()->is_typed(cdk::TYPE_DOUBLE)) {

      if(node->rvalue()->is_typed(cdk::TYPE_DOUBLE) || node->rvalue()->is_typed(cdk::TYPE_INT)) { /* double = double or int, assignment is double */
          node->type(cdk::make_primitive_type(8, cdk::TYPE_DOUBLE));
      }

  } else if (node->lvalue()->is_typed(cdk::TYPE_POINTER)) {
      node->type(node->lvalue()->type());

  } else if (node->lvalue()->is_typed(cdk::TYPE_STRING)) {

      if(node->rvalue()->is_typed(cdk::TYPE_STRING)) { /* string = string, assignment is string */
          node->type(cdk::make_primitive_type(4, cdk::TYPE_STRING));
      }

  } else {
      throw std::string("problem with types assignment");
  }

}

void og::type_checker::do_variable_declaration_node(og::variable_declaration_node *const node, int lvl) {

    if (node->initial_val() != nullptr) {
        auto symbol = std::make_shared<og::symbol>(node->variable_type(), node->variable_name(), false);
        _symtab.insert(node->variable_name(), symbol);
        _parent->set_new_symbol(symbol);

        v_symbol = symbol;

        node->initial_val()->accept(this, lvl);

        if(node->variable_type()->name() == cdk::TYPE_UNSPEC || node->variable_type()->name() == cdk::TYPE_STRUCT) {
            node->set_variable_type(node->initial_val()->type()); //has type because it was accepted before
            node->type(node->initial_val()->type());
            symbol->type(node->type());
            _symtab.replace(node->variable_name(), symbol);

        } else {
            if (node->is_typed(cdk::TYPE_DOUBLE) && node->initial_val()->is_typed(cdk::TYPE_INT)) {
                return; // return immediately because conversions will be handled down the line in the writer
            } else if (node->variable_type()->name() == cdk::TYPE_POINTER) {
                node->type(node->variable_type());
                return;
            }
        node->type(node->initial_val()->type());
        }

    } else {
        auto symbol = std::make_shared<og::symbol>(node->variable_type(), node->variable_name(), false);
        _symtab.insert(node->variable_name(), symbol);
        _parent->set_new_symbol(symbol);
        node->type(node->variable_type());
    }

}

//---------------------------------------------------------------------------

void og::type_checker::do_evaluation_node(og::evaluation_node *const node, int lvl) {
  node->argument()->accept(this, lvl + 2);
}

//---------------------------------------------------------------------------

void og::type_checker::do_if_node(og::if_node *const node, int lvl) {
  node->condition()->accept(this, lvl + 4);
}

void og::type_checker::do_if_else_node(og::if_else_node *const node, int lvl) {
  node->condition()->accept(this, lvl + 4);
}

//---------------------------------------------------------------------------

void og::type_checker::do_for_node(og::for_node *const node, int lvl) {
  if (node->declaration() != nullptr)
    node->declaration()->accept(this, lvl);
  if (node->condition() != nullptr)
    node->condition()->accept(this, lvl);
  if (node->increment() != nullptr)
    node->increment()->accept(this, lvl);
}

// Functions -----------------------------------------------------------------
void og::type_checker::do_function_declaration_node(og::function_declaration_node *const node, int lvl) {
    // NOTE: declaration is simply saying the function exists
    auto symbol = std::make_shared<og::symbol>(cdk::make_primitive_type(node->return_type()->size(), node->return_type()->name()), node->function_name(), true);
    _symtab.insert(node->function_name(), symbol);
    auto newSymb = _symtab.find(node->function_name());
    newSymb->function_args(get_args_types(node->arguments()->nodes()));
    _symtab.replace(node->function_name(), newSymb);
    _parent->set_new_symbol(symbol);
}

void og::type_checker::do_function_definition_node(og::function_definition_node *const node, int lvl) {
    // NOTE: definition includes the code inside the function
    auto symbol = std::make_shared<og::symbol>(node->type(), node->function_name(), true);
    _symtab.insert(node->function_name(), symbol);
    auto newSymb = _symtab.find(node->function_name());
    newSymb->function_args(get_args_types(node->arguments()->nodes()));
    _symtab.replace(node->function_name(), newSymb);
    _parent->set_new_symbol(symbol);
    tc_current_function = newSymb;
}

void og::type_checker::do_function_call_node(og::function_call_node *const node, int lvl) {
    ASSERT_UNSPEC;

    node->arguments()->accept(this, lvl);
    auto symbol = _symtab.find(node->function_name());

    if (!symbol) {
        throw std::string("ERROR: declaration for function with name '" + node->function_name() + "' could not be found");
    } else {
        std::vector<std::shared_ptr<cdk::basic_type>> call_types = get_args_types(node->arguments()->nodes());
        std::vector<std::shared_ptr<cdk::basic_type>> symb_types = symbol->function_args();
        if (call_types.size() != symb_types.size())
            throw std::string("ERROR: calling the function '" + node->function_name() +
            "' with wrong size of arguments");
        for (size_t i = 0; i < symb_types.size(); i++) {
            auto call_type = call_types.at(i).get();
            auto symb_type = symb_types.at(i).get();

            if (call_type->name() != symb_type->name() || call_type->size() != symb_type->size()) {
                if(
                    (call_type->name() == cdk::TYPE_DOUBLE && symb_type->name() == cdk::TYPE_INT) ||
                    (call_type->name() == cdk::TYPE_INT && symb_type->name() == cdk::TYPE_DOUBLE)
                ) {
                    // do nothing, not an error: function might need real but we can pass int
                } else {
                    throw std::string("ERROR: wrong type in function call");
                }
            }
        }

    }
    node->type(symbol->type());

}

// Instructions --------------------------------------------------------------
void og::type_checker::do_address_node(og::address_node *const node, int lvl) {
    node->lvalue()->accept(this, lvl);
    node->type(cdk::make_reference_type(4, node->lvalue()->type()));
}

void og::type_checker::do_index_node(og::index_node *const node, int lvl) {
    node->base()->accept(this, lvl);
    node->type(cdk::reference_type_cast(node->base()->type())->referenced());
}

void og::type_checker::do_tuple_index_node(og::tuple_index_node *const node, int lvl) {
    node->base()->accept(this, lvl);
    node->type(cdk::structured_type_cast(node->base()->type())->component(node->index()->value()-1));

}

void og::type_checker::do_memory_alloc_node(og::memory_alloc_node *const node, int lvl) {
    // when this method is called, a variable declaration or an assignment was/is being made.
    // in both cases, the v_symbol (variable_symbol) object is set, so we can know
    // the type we are allocating.

    node->argument()->accept(this, lvl);
    if(v_symbol != nullptr) {
        node->type(v_symbol->type());
    } else {
        std::cout << ";could not get symbol type" << std::endl;
    }
}

void og::type_checker::do_return_node(og::return_node *const node, int lvl) {
    node->argument()->accept(this, lvl);

    // Ensure given type matches with the function this return is part of
    if (node->argument()->is_typed(cdk::TYPE_STRUCT)) {
        if (tc_current_function->type()->name() != cdk::TYPE_UNSPEC)
            throw std::string(
                "ERROR: function '" + tc_current_function->name() +
                "' should return expression of type '" + cdk::to_string(tc_current_function->type()) +
                "' but return has type 'struct' (tuple)"
            );
    } else {
        if (node->argument()->type()->size() != tc_current_function->type()->size() && node->argument()->type()->name() != tc_current_function->type()->name()) {
            if (
                (node->argument()->is_typed(cdk::TYPE_DOUBLE) && tc_current_function->is_typed(cdk::TYPE_INT)) ||
                (node->argument()->is_typed(cdk::TYPE_INT) && tc_current_function->is_typed(cdk::TYPE_DOUBLE))
               ) {
                    // empty, we allow conversions
            } else {
                throw std::string(
                    "ERROR: function '" + tc_current_function->name() +
                    "' should return expression of type '" + cdk::to_string(tc_current_function->type()) +
                    "' but return has type '" + cdk::to_string(node->argument()->type()) + "'"
                );
            }
        }
    }
}

void og::type_checker::do_write_node(og::write_node *const node, int lvl) {
    node->sequence()->accept(this, lvl + 2);
}

void og::type_checker::do_writeln_node(og::writeln_node *const node, int lvl) {
    node->sequence()->accept(this, lvl + 2);
}

void og::type_checker::do_sizeof_node(og::sizeof_node *const node, int lvl) {
    ASSERT_UNSPEC;
    node->argument()->accept(this, lvl + 2);
    node->type(cdk::make_primitive_type(4, cdk::TYPE_INT));
}


// Nodes without type/need for type checking ---------------------------------

void og::type_checker::do_block_node(og::block_node *const node, int lvl) {
    // EMPTY
}

void og::type_checker::do_nil_node(cdk::nil_node *const node, int lvl) {
  // EMPTY
}

void og::type_checker::do_data_node(cdk::data_node *const node, int lvl) {
  // EMPTY
}

void og::type_checker::do_input_node(og::input_node *const node, int lvl) {
    node->type(cdk::make_primitive_type(4, cdk::TYPE_INT));
}

void og::type_checker::do_break_node(og::break_node *const node, int lvl) {
    // EMPTY
}

void og::type_checker::do_continue_node(og::continue_node *const node, int lvl) {
    // EMPTY
}
