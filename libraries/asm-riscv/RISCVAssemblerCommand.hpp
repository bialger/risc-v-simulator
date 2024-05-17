#ifndef LIBRARIES_ASM_RISCV_RISCVASSEMBLERCOMMAND_HPP_
#define LIBRARIES_ASM_RISCV_RISCVASSEMBLERCOMMAND_HPP_

#include <functional>
#include <string>

#include "RISCVRegisters.hpp"
#include "ProcessMemory.hpp"

class RISCVAssemblerCommand {
 public:
  RISCVAssemblerCommand(const std::string& command_name, uint8_t reg1, uint8_t reg2, int32_t value);

  int32_t Execute(RISCVRegisters& registers, ProcessMemory& memory);

  [[nodiscard]] const std::string& GetCommandName() const;
  [[nodiscard]] uint8_t GetReg1() const;
  [[nodiscard]] uint8_t GetReg2() const;
  [[nodiscard]] int32_t GetValue() const;
 private:
  std::function<int32_t(RISCVRegisters&, ProcessMemory&, uint8_t reg1, uint8_t reg2, int32_t value)> command_;
  const std::string& command_name_;
  uint8_t reg1_;
  uint8_t reg2_;
  int32_t value_;

  static int32_t SignExtended12Bits(int32_t value);
  static int32_t SignExtended13Bits(int32_t value);
  static int32_t SignExtended20Bits(int32_t value);
  static int32_t SignExtended32Bits(uint8_t value);
  static int32_t SignExtended32Bits(uint16_t value);
  static uint32_t Unsigned12Bits(uint32_t value);
  static uint32_t Unsigned20Bits(uint32_t value);
};

#endif //LIBRARIES_ASM_RISCV_RISCVASSEMBLERCOMMAND_HPP_
