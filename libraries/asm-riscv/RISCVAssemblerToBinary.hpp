#ifndef LIBRARIES_ASM_RISCV_RISCVASSEMBLERTOBINARY_HPP_
#define LIBRARIES_ASM_RISCV_RISCVASSEMBLERTOBINARY_HPP_

#include <vector>
#include <cstdint>

#include "RISCVAssemblerCommand.hpp"

class RISCVAssemblerToBinary {
 public:
  RISCVAssemblerToBinary() = default;

  static std::vector<uint32_t> Assemble(const std::vector<RISCVAssemblerCommand>& commands);

 private:
  static uint32_t ToBinary(const RISCVAssemblerCommand& command);
};

#endif //LIBRARIES_ASM_RISCV_RISCVASSEMBLERTOBINARY_HPP_
