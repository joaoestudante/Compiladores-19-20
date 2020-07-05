#include <string>
#include <sstream>
#include "targets/type_checker.h"
#include "targets/postfix_writer.h"
#include "targets/localvars_checker.h"
#include "ast/all.h"  // all.h is automatically generated

//---------------------------------------------------------------------------

bool _in_function = false;
bool _in_arguments = false;
bool _in_body = false;
int _vars_offset;
std::shared_ptr<og::symbol> current_function;

void verificationBinaryExpression(cdk::binary_operation_node * const node, int side, cdk::basic_postfix_emitter &_pf){
  if(side == 1){
    if (node->is_typed(cdk::TYPE_INT) && node->left()->is_typed(cdk::TYPE_DOUBLE)) {
        _pf.D2I();
    } else if (node->is_typed(cdk::TYPE_DOUBLE) && node->left()->is_typed(cdk::TYPE_INT)) {
        _pf.I2D();
    }
  } else {
    if (node->is_typed(cdk::TYPE_INT) && node->right()->is_typed(cdk::TYPE_DOUBLE)) {
        _pf.D2I();
    } else if (node->is_typed(cdk::TYPE_DOUBLE) && node->right()->is_typed(cdk::TYPE_INT)) {
        _pf.I2D();
    }
  }
}

//---------------------------------------------------------------------------

void og::postfix_writer::do_nil_node(cdk::nil_node * const node, int lvl) {
  // EMPTY
}
void og::postfix_writer::do_data_node(cdk::data_node * const node, int lvl) {
  // EMPTY
}
void og::postfix_writer::do_double_node(cdk::double_node * const node, int lvl) {
    _pf.DOUBLE(node->value());
}
void og::postfix_writer::do_not_node(cdk::not_node * const node, int lvl) {
  // EMPTY
}
void og::postfix_writer::do_and_node(cdk::and_node * const node, int lvl) {
    ASSERT_SAFE_EXPRESSIONS;
    int lbl = ++_lbl;
    node->left()->accept(this, lvl + 2);
    _pf.DUP32();
    _pf.JZ(mklbl(lbl)); //lazy evaluation - dont run right side if the left side is FALSE
    node->right()->accept(this, lvl + 2);
    _pf.AND();
    _pf.ALIGN();
    _pf.LABEL(mklbl(lbl));

}
void og::postfix_writer::do_or_node(cdk::or_node * const node, int lvl) {
    ASSERT_SAFE_EXPRESSIONS;
    int lbl = ++_lbl;
    node->left()->accept(this, lvl + 2);
    _pf.DUP32();
    _pf.JNZ(mklbl(lbl)); // lazy evaluation - dont run right side if the left side is TRUE
    node->right()->accept(this, lvl + 2);
    _pf.OR();
    _pf.ALIGN();
    _pf.LABEL(mklbl(lbl));
}

//---------------------------------------------------------------------------

void og::postfix_writer::do_sequence_node(cdk::sequence_node * const node, int lvl) {
  for (size_t i = 0; i < node->size(); i++) {
    node->node(i)->accept(this, lvl);
  }
}

//---------------------------------------------------------------------------

void og::postfix_writer::do_integer_node(cdk::integer_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  _pf.INT(node->value());
}

void og::postfix_writer::do_string_node(cdk::string_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int lbl1;

  /* generate the string */
  _pf.RODATA(); // strings are DATA readonly
  _pf.ALIGN(); // make sure we are aligned
  _pf.LABEL(mklbl(lbl1 = ++_lbl)); // give the string a name
  _pf.SSTRING(node->value()); // output string characters


  /* leave the address on the stack */

  _pf.TEXT(); // return to the TEXT segment
  _pf.ADDR(mklbl(lbl1)); // the string to be printed */
}

//---------------------------------------------------------------------------

void og::postfix_writer::do_neg_node(cdk::neg_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl); // determine the value
  if (node->is_typed(cdk::TYPE_DOUBLE))
    _pf.DNEG();
  else
    _pf.NEG(); // 2-complement
}

//---------------------------------------------------------------------------

void og::postfix_writer::do_add_node(cdk::add_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->left()->accept(this, lvl);
  if (node->left()->is_typed(cdk::TYPE_INT) && node->right()->is_typed(cdk::TYPE_POINTER)){
      // we are calculating an offset
      _pf.INT(cdk::reference_type_cast(node->right()->type())->referenced()->size());
      _pf.MUL();
  }

  verificationBinaryExpression(node, 1, _pf);

  node->right()->accept(this, lvl);
  if (node->right()->is_typed(cdk::TYPE_INT) && node->left()->is_typed(cdk::TYPE_POINTER)){
      // we are calculating an offset
      _pf.INT(cdk::reference_type_cast(node->left()->type())->referenced()->size());
      _pf.MUL();
  }
  verificationBinaryExpression(node, 2, _pf);

  if (node->is_typed(cdk::TYPE_DOUBLE))
    _pf.DADD();
  else
    _pf.ADD();
}

void og::postfix_writer::do_sub_node(cdk::sub_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  verificationBinaryExpression(node, 1, _pf);
  node->right()->accept(this, lvl);
  verificationBinaryExpression(node, 2, _pf);
  if (node->is_typed(cdk::TYPE_DOUBLE))
    _pf.DSUB();
  else
    _pf.SUB();
}
void og::postfix_writer::do_mul_node(cdk::mul_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  verificationBinaryExpression(node, 1, _pf);
  node->right()->accept(this, lvl);
  verificationBinaryExpression(node, 2, _pf);
  if (node->is_typed(cdk::TYPE_DOUBLE))
    _pf.DMUL();
  else
    _pf.MUL();
}
void og::postfix_writer::do_div_node(cdk::div_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  verificationBinaryExpression(node, 1, _pf);

  node->right()->accept(this, lvl);
  verificationBinaryExpression(node, 2, _pf);

  if (node->is_typed(cdk::TYPE_DOUBLE))
    _pf.DDIV();
  else
    _pf.DIV();
}
void og::postfix_writer::do_mod_node(cdk::mod_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  verificationBinaryExpression(node, 1, _pf);
  node->right()->accept(this, lvl);
  verificationBinaryExpression(node, 2, _pf);
  _pf.MOD();
}
void og::postfix_writer::do_lt_node(cdk::lt_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.LT();
}
void og::postfix_writer::do_le_node(cdk::le_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.LE();
}
void og::postfix_writer::do_ge_node(cdk::ge_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.GE();
}
void og::postfix_writer::do_gt_node(cdk::gt_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.GT();
}
void og::postfix_writer::do_ne_node(cdk::ne_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.NE();
}
void og::postfix_writer::do_eq_node(cdk::eq_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.EQ();
}

//---------------------------------------------------------------------------

void og::postfix_writer::do_variable_node(cdk::variable_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  if (_in_function) {
      std::shared_ptr<og::symbol> var = _symtab.find(node->name());
      if(var->offset() == 0)
        _pf.ADDR(var->name());
      else
        _pf.LOCAL(var->offset());
  } else
      _pf.ADDR(node->name());
}

void og::postfix_writer::do_rvalue_node(cdk::rvalue_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->lvalue()->accept(this, lvl);
  if (node->lvalue()->is_typed(cdk::TYPE_INT) ||
      node->lvalue()->is_typed(cdk::TYPE_STRING) ||
      node->lvalue()->is_typed(cdk::TYPE_POINTER))
    _pf.LDINT();
  else if (node->lvalue()->is_typed(cdk::TYPE_DOUBLE))
    _pf.LDDOUBLE();
  else if (node->lvalue()->is_typed(cdk::TYPE_STRUCT)) {}
}


void og::postfix_writer::do_assignment_node(cdk::assignment_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->rvalue()->accept(this, lvl); // determine the new value
  bool converted = false;

  if (node->is_typed(cdk::TYPE_DOUBLE) && node->rvalue()->is_typed(cdk::TYPE_INT)) {
    _pf.I2D();
    _pf.DUP64();
    converted = true;
  }
  else if (node->is_typed(cdk::TYPE_INT) && node->rvalue()->is_typed(cdk::TYPE_DOUBLE)) {
    _pf.D2I();
    _pf.DUP32();
    converted = true;
  }

  if (!converted && node->rvalue()->is_typed(cdk::TYPE_DOUBLE) && !node->lvalue()->is_typed(cdk::TYPE_POINTER)) {
      _pf.DUP64();
  } else if (!converted) {
      _pf.DUP32();
  }

  node->lvalue()->accept(this, lvl); // where to store the value

  if (!node->lvalue()->is_typed(cdk::TYPE_POINTER) &&
        node->rvalue()->is_typed(cdk::TYPE_DOUBLE) &&
        node->is_typed(cdk::TYPE_DOUBLE))
    _pf.STDOUBLE();
  else if (node->rvalue()->is_typed(cdk::TYPE_INT) && node->is_typed(cdk::TYPE_INT))
    _pf.STINT();
  else if (node->rvalue()->is_typed(cdk::TYPE_STRING))
    _pf.STINT();
  else if (node->rvalue()->is_typed(cdk::TYPE_POINTER) && node->is_typed(cdk::TYPE_POINTER))
    _pf.STINT();
  else if (node->lvalue()->is_typed(cdk::TYPE_POINTER))
    _pf.STINT();
  else if(node->is_typed(cdk::TYPE_DOUBLE))
    _pf.STDOUBLE();
}

//---------------------------------------------------------------------------

void og::postfix_writer::do_evaluation_node(og::evaluation_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl); // determine the value

  if (node->argument()->is_typed(cdk::TYPE_INT) || node->argument()->is_typed(cdk::TYPE_POINTER))
    _pf.TRASH(4);
  else if (node->argument()->is_typed(cdk::TYPE_STRING))
    _pf.TRASH(4);
  else if (node->argument()->is_typed(cdk::TYPE_DOUBLE))
    _pf.TRASH(8);
  else if(node->argument()->is_typed(cdk::TYPE_VOID)) {
      // empty, nothing to trash
  } else {
    std::cerr << "ERROR: no compatible type in evaluation found" << std::endl;
    exit(1);
  }
}

//---------------------------------------------------------------------------

void og::postfix_writer::do_if_node(og::if_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int lbl1;
  node->condition()->accept(this, lvl);
  _pf.JZ(mklbl(lbl1 = ++_lbl));
  node->block()->accept(this, lvl + 2);
  _pf.LABEL(mklbl(lbl1));
}

//---------------------------------------------------------------------------

void og::postfix_writer::do_if_else_node(og::if_else_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int lbl1, lbl2;
  node->condition()->accept(this, lvl);
  _pf.JZ(mklbl(lbl1 = ++_lbl));
  node->thenblock()->accept(this, lvl + 2);
  _pf.JMP(mklbl(lbl2 = ++_lbl));
  _pf.LABEL(mklbl(lbl1));
  node->elseblock()->accept(this, lvl + 2);
  _pf.LABEL(mklbl(lbl1 = lbl2));
}

//---------------------------------------------------------------------------

void og::postfix_writer::do_for_node(og::for_node* const node, int lvl) {
    _symtab.push();
    ASSERT_SAFE_EXPRESSIONS;
    int lbl1, lbl2, lbl3;
    mklbl(lbl1 = ++_lbl);
    mklbl(lbl2 = ++_lbl);
    mklbl(lbl3 = ++_lbl);
    add_for(lbl1, lbl2, lbl3);

    node->declaration()->accept(this, lvl);   // evaluate initializer

    _pf.LABEL(mklbl(lbl1));                   // LABEL condition
    node->condition()->accept(this, lvl);     // evaluate condition

    _pf.JZ(mklbl(lbl2));                      // JZ endfor

    node->block()->accept(this, lvl);         // evaluate block

    _pf.LABEL(mklbl(lbl3));                   // LABEL increment
    node->increment()->accept(this, lvl);     // evaluate increment

    _pf.JMP(mklbl(lbl1));
    _pf.LABEL(mklbl(lbl2));
    _stack.pop();
    _symtab.pop();
}

void og::postfix_writer::do_function_declaration_node(og::function_declaration_node *const node, int lvl) {
    ASSERT_SAFE_EXPRESSIONS;

    _extern_functions->insert(node->function_name());
    // add function name to extern_functions;
}

void og::postfix_writer::do_function_definition_node(og::function_definition_node *const node, int lvl) {
    ASSERT_SAFE_EXPRESSIONS;

    if(*(_extern_functions->find(node->function_name())) == node->function_name())
      _extern_functions->erase(_extern_functions->find(node->function_name()));


    localvars_checker locals = localvars_checker(_compiler, _symtab, this);
    node->accept(&locals, lvl);
    int localVarsAmount = locals.localVarsAmount();

    _in_function = true;
    _symtab.push();
    if (node->function_name() == "og") {
        // generate the main function (RTS mandates that its name be "_main")
        _pf.TEXT();
        _pf.ALIGN();
        _pf.GLOBAL("_main", _pf.FUNC());
        _pf.LABEL("_main");

        _pf.ENTER(localVarsAmount);

        _in_body = true;
        current_function = _symtab.find(node->function_name());
        node->statements()->accept(this, lvl); // declarations
        _in_body = false;
        current_function = nullptr;
    } else {
        _pf.TEXT();
        _pf.ALIGN();
        if (node->scope() == "public")
            _pf.GLOBAL(node->function_name(), _pf.FUNC());
        _pf.LABEL(node->function_name());

        _pf.ENTER(localVarsAmount);

        _vars_offset = 8;
        _in_arguments = true;
        node->arguments()->accept(this, lvl);
        _in_arguments = false;

        _vars_offset = 0;
        _in_body = true;
        current_function = _symtab.find(node->function_name());
        node->statements()->accept(this, lvl);
        _in_body = false;
        current_function = nullptr;

        // function decides on return value or lack of it
    }
    if (node->is_typed(cdk::TYPE_VOID)) {
        _pf.LEAVE();
        _pf.RET();
    }
    _symtab.pop();
    _in_function = false;
}

void og::postfix_writer::do_function_call_node(og::function_call_node *const node, int lvl) {
    ASSERT_SAFE_EXPRESSIONS;

    auto function_symbol = _symtab.find(node->function_name());

    int to_trash = 0;

    if (node->arguments()) {
      for (int n_arg = node->arguments()->size(); n_arg > 0; n_arg--) { // put in stack from end to start
        cdk::expression_node *arg = dynamic_cast<cdk::expression_node*>(node->arguments()->node(n_arg - 1));
        arg->accept(this, lvl);
        std::shared_ptr<cdk::basic_type> actualType = function_symbol->function_args().at(n_arg - 1);
        if (actualType->name() == cdk::TYPE_DOUBLE && arg->is_typed(cdk::TYPE_INT))
            _pf.I2D();
        else if (actualType->name() == cdk::TYPE_INT && arg->is_typed(cdk::TYPE_DOUBLE))
            _pf.D2I();
        to_trash += actualType->size();
      }
    }
    // if function returns tuple, alloc memory here with ALLOC and pass SP as first argument - NOT IMPLEMENTED

    _pf.CALL(node->function_name());
    if (to_trash != 0) {
      _pf.TRASH(to_trash);
    }

    if (function_symbol->is_typed(cdk::TYPE_POINTER) ||
        function_symbol->is_typed(cdk::TYPE_STRING) ||
        function_symbol->is_typed(cdk::TYPE_INT)
       )
        _pf.LDFVAL32();
    else if (function_symbol->is_typed(cdk::TYPE_DOUBLE))
      _pf.LDFVAL64();
}

void og::postfix_writer::do_variable_declaration_node(og::variable_declaration_node *const node, int lvl) {
    ASSERT_SAFE_EXPRESSIONS;

    int decl_offset = 0;
    int var_type_size = node->type()->size();

    /* Get offset of this declaration, depending on where we are */
    /* The starting offset was already properly set before, globally */
    if (_in_body) {
        _vars_offset -= var_type_size;
        decl_offset = _vars_offset;
    } else if (_in_arguments) {
        decl_offset = _vars_offset;
        _vars_offset += var_type_size; // next argument has to worry about this argument's size
    }


    std::shared_ptr<og::symbol> newVar = new_symbol();
    if(newVar) {
      newVar->offset(decl_offset);
      _symtab.replace(newVar->name(), newVar);
    }

    if (_in_body) { // declaring local variable
        if (node->initial_val() != nullptr) {
            node->initial_val()->accept(this, lvl);
            if (node->is_typed(cdk::TYPE_INT) || node->is_typed(cdk::TYPE_POINTER) || node->is_typed(cdk::TYPE_STRING)) {
                _pf.LOCAL(decl_offset);
                _pf.STINT();
            } else if (node->is_typed(cdk::TYPE_DOUBLE)) {
                if (node->initial_val()->is_typed(cdk::TYPE_INT))
                    _pf.I2D();
                _pf.LOCAL(decl_offset);
                _pf.STDOUBLE();
            }
        }
    } else if (!_in_function && !_in_arguments) { // declaring global variable
        if (node->is_typed(cdk::TYPE_STRUCT)) { // tuple
            _pf.DATA();
            _pf.ALIGN();
            _pf.LABEL(node->variable_name());
            node->initial_val()->accept(this, lvl);
            return;
        }

        if (node->is_typed(cdk::TYPE_STRING)) {
            node->initial_val()->accept(this, lvl);
            _pf.TRASH(4);
            _pf.DATA();
            _pf.ALIGN();
            _pf.LABEL(node->variable_name());
            _pf.SADDR(mklbl(_lbl));
            _pf.TEXT();
            _pf.ALIGN();
            return;
        }

        if (node->is_typed(cdk::TYPE_POINTER) && node->initial_val() != nullptr) {
            node->initial_val()->accept(this, lvl);
            _pf.TRASH(4);
            _pf.DATA();
            _pf.ALIGN();
            _pf.LABEL(node->variable_name());
            _pf.SADDR(mklbl(_lbl));
            _pf.TEXT();
            _pf.ALIGN();
            return;
        }

        if (node->initial_val() != nullptr) {
            _pf.DATA();
            _pf.ALIGN();
        } else {
            _pf.BSS();
            _pf.ALIGN();
        }
        if (node->scope() == "public")
            _pf.GLOBAL(node->variable_name(), _pf.OBJ());
        _pf.LABEL(node->variable_name());

        if (node->is_typed(cdk::TYPE_INT) || node->is_typed(cdk::TYPE_POINTER)) {
            if (node->initial_val() != nullptr && node->initial_val()->is_typed(cdk::TYPE_INT)) {
                int val = ( dynamic_cast<cdk::integer_node*>(node->initial_val()) )->value();
                _pf.SINT(val);
            } else if (node->initial_val() != nullptr && node->initial_val()->is_typed(cdk::TYPE_DOUBLE)) {
                double val = ( dynamic_cast<cdk::double_node*>(node->initial_val()) )->value();
                _pf.SINT((int) val);
            } else {
                _pf.SALLOC(4);
            }
        } else if (node->is_typed(cdk::TYPE_DOUBLE)) {
            if (node->initial_val() != nullptr && node->initial_val()->is_typed(cdk::TYPE_DOUBLE)) {
                double val = ( dynamic_cast<cdk::double_node*>(node->initial_val()) )->value();
                _pf.SDOUBLE(val);
            } else if (node->initial_val() != nullptr && node->initial_val()->is_typed(cdk::TYPE_INT)){
                int val = ( dynamic_cast<cdk::integer_node*>(node->initial_val()) )->value();
                _pf.SDOUBLE( (double) val);
            } else {
                _pf.SALLOC(8);
            }
        }
    } // end of global variable
}

void og::postfix_writer::do_tuple_node(og::tuple_node *const node, int lvl) {
    ASSERT_SAFE_EXPRESSIONS;
    if (_in_function){
      int already_added = 0;
      auto var = new_symbol();
      int offset = var->offset();
      reset_new_symbol();

      for (auto n : node->contents()->nodes()){
        n->accept(this, lvl);

        auto typed_n = static_cast<cdk::expression_node*>(n);
        _pf.LOCAL(offset);
        _pf.INT(already_added);
        _pf.ADD();

        if (typed_n->is_typed(cdk::TYPE_DOUBLE))
            _pf.STDOUBLE();
        else
            _pf.STINT();

        already_added += typed_n->type()->size();
      }
        _pf.LOCAL(offset);
    } else {
        // NOT IMPLEMENTED
    }
}


void og::postfix_writer::do_address_node(og::address_node *const node, int lvl) {
     ASSERT_SAFE_EXPRESSIONS;
     node->lvalue()->accept(this, lvl);
}

void og::postfix_writer::do_index_node(og::index_node *const node, int lvl) {
    ASSERT_SAFE_EXPRESSIONS;
    node->base()->accept(this, lvl); // base address in stack
    std::shared_ptr<cdk::reference_type> reference = cdk::reference_type_cast(node->base()->type());
    int var_type_size = reference->referenced()->size();
    node->index()->accept(this, lvl);
    _pf.INT(var_type_size);
    _pf.MUL(); // index * type
    _pf.ADD();
}

void og::postfix_writer::do_tuple_index_node(og::tuple_index_node *const node, int lvl) {
    ASSERT_SAFE_EXPRESSIONS;
     node->base()->accept(this, lvl);
     std::shared_ptr<cdk::structured_type> stype = cdk::structured_type_cast(node->base()->type());
     size_t types_sum = 0;
     for (int i = 0; i < node->index()->value(); i++)
         types_sum += stype->component(i)->size();

     _pf.INT(types_sum);
     _pf.ADD();
}

void og::postfix_writer::do_memory_alloc_node(og::memory_alloc_node *const node, int lvl) {
    ASSERT_SAFE_EXPRESSIONS;
    node->argument()->accept(this, lvl);
    if(cdk::reference_type_cast(node->type())->referenced()->name() == cdk::TYPE_DOUBLE)
        _pf.INT(8);
    else
        _pf.INT(4);

    _pf.MUL();
    _pf.ALLOC();
    _pf.SP();
}


void og::postfix_writer::do_break_node(og::break_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  for_labels eval = _stack.top();
  _pf.JMP(mklbl(eval.lbl2));
}

void og::postfix_writer::do_continue_node(og::continue_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  for_labels eval = _stack.top();
  _pf.JMP(mklbl(eval.lbl3));
}

void og::postfix_writer::do_return_node(og::return_node *const node, int lvl) {
    ASSERT_SAFE_EXPRESSIONS;
    // if argument type is TYPE_STRUCT (should return a tuple), then use address of
    // first argument as the base for the tuple - NOT IMPLEMENTED

    node->argument()->accept(this, lvl);
    if (!current_function){
        std::cerr << "ERROR: global function not declared" << std::endl;
        exit(1);
    }
    if(node->argument()->is_typed(cdk::TYPE_INT) && current_function->type()->name() == cdk::TYPE_DOUBLE) {
        _pf.I2D();
        _pf.STFVAL64();
    } else if (node->argument()->is_typed(cdk::TYPE_DOUBLE))
        _pf.STFVAL64();
    else
        _pf.STFVAL32();

    _pf.LEAVE();
    _pf.RET();
}

void og::postfix_writer::do_input_node(og::input_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  _extern_functions->insert("readi");
  _pf.CALL("readi");
  _pf.LDFVAL32();
  _pf.STINT();
}

void og::postfix_writer::do_write_node(og::write_node *const node, int lvl) {
    ASSERT_SAFE_EXPRESSIONS;

    for (auto n: node->sequence()->nodes()) {
        auto to_evaluate = static_cast<cdk::typed_node*>(n);
        to_evaluate->accept(this, lvl);

        if (to_evaluate->is_typed(cdk::TYPE_INT) || to_evaluate->is_typed(cdk::TYPE_POINTER)) {
          _extern_functions->insert("printi");
          _pf.CALL("printi");
          _pf.TRASH(4);
      } else if (to_evaluate->is_typed(cdk::TYPE_STRING)) {
          _extern_functions->insert("prints");
          _pf.CALL("prints");
          _pf.TRASH(4);
      } else if (to_evaluate->is_typed(cdk::TYPE_DOUBLE)) {
        _extern_functions->insert("printd");
          _pf.CALL("printd");
          _pf.TRASH(8);
      } else {
          std::cerr << "ERROR: Attempted to write expression of unknown/unsuported type. Type was: '" << cdk::to_string(to_evaluate->type()) << "'" << std::endl;
          exit(1);
      }

    }

}
void og::postfix_writer::do_writeln_node(og::writeln_node *const node, int lvl) {
    ASSERT_SAFE_EXPRESSIONS;

    for (auto n: node->sequence()->nodes()) {
        auto to_evaluate = static_cast<cdk::expression_node*>(n);
        to_evaluate->accept(this, lvl);
        if (to_evaluate->is_typed(cdk::TYPE_INT) || to_evaluate->is_typed(cdk::TYPE_POINTER)) {
          _extern_functions->insert("printi");
          _extern_functions->insert("println");
          _pf.CALL("printi");
          _pf.CALL("println");
          _pf.TRASH(4);
      } else if (to_evaluate->is_typed(cdk::TYPE_STRING)) {
          _extern_functions->insert("prints");
          _extern_functions->insert("println");
          _pf.CALL("prints");
          _pf.CALL("println");
          _pf.TRASH(4);
      } else if (to_evaluate->is_typed(cdk::TYPE_DOUBLE)) {
          _extern_functions->insert("printd");
          _extern_functions->insert("println");
          _pf.CALL("printd");
          _pf.CALL("println");
          _pf.TRASH(8);
      } else {
          std::cerr << "ERROR: Attempted to write expression of unknown/unsuported type. Type was: '" << cdk::to_string(to_evaluate->type()) << "'" << std::endl;
          exit(1);
      }
    }
}

void og::postfix_writer::do_sizeof_node(og::sizeof_node *const node, int lvl) {
    ASSERT_SAFE_EXPRESSIONS;
    // argument is always a tuple aka structured_type, so its size is already "inbuilt" in the type
    _pf.INT(node->argument()->type()->size());
}

void og::postfix_writer::do_nullptr_node(og::nullptr_node *const node, int lvl) {
    ASSERT_SAFE_EXPRESSIONS;
    _pf.INT(0);
}

void og::postfix_writer::do_block_node(og::block_node *const node, int lvl) {
    node->decs()->accept(this, lvl);
    node->instructs()->accept(this, lvl);
}
void og::postfix_writer::do_identity_node(og::identity_node *const node, int lvl) {
    node->argument()->accept(this, lvl);
}
