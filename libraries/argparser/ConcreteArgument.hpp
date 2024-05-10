#ifndef CONCRETEARGUMENT_HPP_
#define CONCRETEARGUMENT_HPP_

#include <limits>

#include "Argument.hpp"

namespace ArgumentParser {

template<typename T>
class ConcreteArgument : public Argument {
 public:
  ConcreteArgument() = delete;
  ConcreteArgument(const ArgumentInformation& info, T default_value, T* stored_value, std::vector<T>* stored_values);
  [[nodiscard]] T GetValue(size_t index) const;
  std::vector<size_t> ValidateArgument(const std::vector<std::string>& argv, size_t position) override;
  bool CheckLimit() override;
  [[nodiscard]] ArgumentParsingStatus GetValueStatus() const override;
  [[nodiscard]] const std::string& GetType() const override;
  [[nodiscard]] const ArgumentInformation& GetInfo() const override;
  [[nodiscard]] size_t GetUsedValues() const override;
  void ClearStored() override;
 protected:
  inline size_t ObtainValue(const std::vector<std::string>& argv, std::string& value_string,
                            std::vector<size_t>& used_values, size_t position) override;

 private:
  ArgumentInformation info_;
  ArgumentParsingStatus value_status_;
  size_t value_counter_;
  T value_;
  T default_value_;
  T* stored_value_;
  std::vector<T>* stored_values_;
};

template<typename T>
ConcreteArgument<T>::ConcreteArgument(const ArgumentInformation& info,
                                      T default_value,
                                      T* stored_value,
                                      std::vector<T>* stored_values) {
  info_ = info;
  value_ = default_value;
  value_counter_ = 0;
  value_status_ = ArgumentParsingStatus::kNoArgument;
  default_value_ = default_value;
  stored_value_ = stored_value;
  stored_values_ = stored_values;
}

template<typename T>
T ConcreteArgument<T>::GetValue(size_t index) const {
  if (!info_.has_store_values) {
    return value_;
  }

  return stored_values_->at(index);
}

template<typename T>
ArgumentParsingStatus ConcreteArgument<T>::GetValueStatus() const {
  return value_status_;
}

template<typename T>
const std::string& ConcreteArgument<T>::GetType() const {
  return info_.type;
}

template<typename T>
const ArgumentInformation& ConcreteArgument<T>::GetInfo() const {
  return info_;
}

template<typename T>
size_t ConcreteArgument<T>::GetUsedValues() const {
  return value_counter_;
}

template<typename T>
bool ConcreteArgument<T>::CheckLimit() {
  if (value_counter_ < info_.minimum_values) {
    value_status_ = ArgumentParsingStatus::kInsufficientArguments;
    return false;
  }

  if (value_status_ == ArgumentParsingStatus::kInsufficientArguments) {
    value_status_ = ArgumentParsingStatus::kSuccess;
  }

  return true;
}

template<typename T>
void ConcreteArgument<T>::ClearStored() {
  stored_values_->clear();
  value_counter_ = 0;

  if (stored_value_ != nullptr) {
    *stored_value_ = default_value_;
  }
}

template<typename T>
std::vector<size_t> ConcreteArgument<T>::ValidateArgument(const std::vector<std::string>& argv,
                                                          size_t position) {
  std::vector<size_t> used_positions;
  if (stored_values_->empty()) {
    value_status_ = ArgumentParsingStatus::kSuccess;
  }

  while (used_positions.empty() || info_.is_multi_value) {
    std::string value_string = argv[position];
    used_positions.push_back(position);

    if (argv[position][0] == '-') {
      size_t equals_index = argv[position].find('=');

      if (equals_index != std::string::npos) {
        value_string = argv[position].substr(equals_index + 1);
      } else if (std::is_same<T, bool>::value) {
        value_string = "true";
      } else {
        ++position;
        used_positions.push_back(position);
        value_string = argv[position];
      }
    }

    position = ObtainValue(argv, value_string, used_positions, position);

    ++position;
    ++value_counter_;

    stored_values_->push_back(value_);

    if (position >= argv.size() || argv[position][0] == '-') {
      break;
    }
  }

  CheckLimit();

  if (info_.has_store_value) {
    *stored_value_ = value_;
  }

  return used_positions;
}

#include "ConcreteArgument.inl"

}

#endif //CONCRETEARGUMENT_HPP_
