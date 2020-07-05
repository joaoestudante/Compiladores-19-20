#ifndef __OG_TARGETS_SYMBOL_H__
#define __OG_TARGETS_SYMBOL_H__

#include <string>
#include <memory>
#include <cdk/types/basic_type.h>

namespace og {

  class symbol {
    std::shared_ptr<cdk::basic_type> _type;
    std::vector<std::shared_ptr<cdk::basic_type>> _function_args; // needed to check in function calls for correctness/converting types
    std::string _name;
    bool _is_function;
    int _offset;

  public:
    symbol(std::shared_ptr<cdk::basic_type> type, const std::string &name, bool is_function) :
        _type(type), _name(name), _is_function(is_function), _offset(0){
    }

    virtual ~symbol() {
      // EMPTY
    }

    std::shared_ptr<cdk::basic_type> type() const {
      return _type;
    }
    void type(std::shared_ptr<cdk::basic_type> type) {
        _type = type;
    }
    bool is_typed(cdk::typename_type name) const {
      return _type->name() == name;
    }
    const std::string &name() const {
      return _name;
    }

    int offset(int o) {
        return _offset = o;
    }

    int offset() {
        return _offset;
    }

    std::vector<std::shared_ptr<cdk::basic_type>> function_args() {
        return _function_args;
    };

    void function_args(std::vector<std::shared_ptr<cdk::basic_type>> types) {
        _function_args = types;
    }

  };

} // og

#endif
