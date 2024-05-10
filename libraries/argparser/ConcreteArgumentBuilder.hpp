#ifndef CONCRETEARGUMENTBUILDER_HPP_
#define CONCRETEARGUMENTBUILDER_HPP_

#include <sstream>

#include "ArgumentBuilder.hpp"
#include "ConcreteArgument.hpp"

namespace ArgumentParser {

template<typename T>
class ConcreteArgumentBuilder : public ArgumentBuilder {
 public:
  ConcreteArgumentBuilder() = delete;

  ConcreteArgumentBuilder(char short_name, const char* long_name, const char* description) {
    info_ = ArgumentInformation();
    info_.short_key = short_name;
    info_.long_key = long_name;
    info_.description = description;
    info_.type = typeid(T).name();
    default_value_ = T();
    stored_value_ = nullptr;
    stored_values_ = nullptr;
    was_created_temp_vector_ = false;
  }

  ConcreteArgumentBuilder(const ConcreteArgumentBuilder& other) = delete;

  ConcreteArgumentBuilder& operator=(const ConcreteArgumentBuilder& other) = delete;

  ~ConcreteArgumentBuilder() override {
    if (was_created_temp_vector_) {
      delete stored_values_;
    }
  }

  ConcreteArgumentBuilder& MultiValue(size_t min = 0) {
    info_.is_multi_value = true;
    info_.minimum_values = min;
    return *this;
  }

  ConcreteArgumentBuilder& Positional() {
    info_.is_positional = true;
    return *this;
  }

  ConcreteArgumentBuilder& StoreValue(T& value) {
    info_.has_store_value = true;
    stored_value_ = &value;
    return *this;
  }

  ConcreteArgumentBuilder& StoreValues(std::vector<T>& values) {
    info_.has_store_values = true;
    stored_values_ = &values;
    return *this;
  }

  ConcreteArgumentBuilder& Default(T value) {
    info_.has_default = true;
    default_value_ = value;
    return *this;
  }

  ConcreteArgumentBuilder& AddValidate(const std::function<bool(std::string&)>& validate) {
    info_.validate = validate;
    return *this;
  }

  ConcreteArgumentBuilder& AddIsGood(const std::function<bool(std::string&)>& is_good) {
    info_.is_good = is_good;
    return *this;
  }

  Argument* build() override {
    if (std::is_same<T, bool>::value && !info_.has_default) {
      info_.has_default = true;
    }

    if (info_.minimum_values == 0 && !info_.has_default && !info_.is_multi_value) {
      info_.minimum_values = 1;
    }

    if (!info_.has_store_values && !was_created_temp_vector_) {
      stored_values_ = new std::vector<T>;
      was_created_temp_vector_ = true;
    }

    return new ConcreteArgument<T>(info_, default_value_, stored_value_, stored_values_);
  }

  [[nodiscard]] const ArgumentInformation& GetInfo() const override {
    return info_;
  }

  [[nodiscard]] std::string GetDefaultValue() const override {
    std::ostringstream stream;
    stream << default_value_;
    return stream.str();
  }

 private:
  ArgumentInformation info_;
  T default_value_;
  T* stored_value_;
  std::vector<T>* stored_values_;
  bool was_created_temp_vector_;
};

} // namespace ArgumentParser

#endif //CONCRETEARGUMENTBUILDER_HPP_
