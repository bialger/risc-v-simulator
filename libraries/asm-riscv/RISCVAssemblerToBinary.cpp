#include "RISCVAssemblerToBinary.hpp"

uint32_t RISCVAssemblerToBinary::ToBinary(const RISCVAssemblerCommand& command) {
  return 0;
}

std::vector<uint32_t> RISCVAssemblerToBinary::Assemble(const std::vector<RISCVAssemblerCommand>& commands) {
  std::vector<uint32_t> result(commands.size());

  for (size_t i = 0; i < commands.size(); i++) {
    result[i] = ToBinary(commands[i]);
  }

  return result;
}
