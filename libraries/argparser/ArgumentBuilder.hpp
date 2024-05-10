#ifndef ARGUMENTBUILDER_HPP_
#define ARGUMENTBUILDER_HPP_

#include "Argument.hpp"

namespace ArgumentParser {

class ArgumentBuilder {
 public:
  virtual ~ArgumentBuilder() = default;
  [[nodiscard]] virtual const ArgumentInformation& GetInfo() const = 0;
  [[nodiscard]] virtual std::string GetDefaultValue() const = 0;
  virtual Argument* build() = 0;
};

} // namespace ArgumentParser

#endif //ARGUMENTBUILDER_HPP_
