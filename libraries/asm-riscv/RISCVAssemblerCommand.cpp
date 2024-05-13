#include <stdexcept>
#include <limits>

#include "RISCVAssemblerCommand.hpp"

RISCVAssemblerCommand::RISCVAssemblerCommand(const std::string& command_name,
                                             uint8_t reg1,
                                             uint8_t reg2,
                                             int32_t value)
    : command_name_(command_name), reg1_(reg1), reg2_(reg2), value_(value) {
  if (command_name == "lui") {
    command_ = [](RISCVRegisters& registers, ProcessMemory& memory, uint8_t reg1, uint8_t reg2, int32_t value) {
      registers.SetRegister(reg1, value << 12);
      return 0;
    };
  } else if (command_name == "aiupc") {
    command_ = [](RISCVRegisters& registers, ProcessMemory& memory, uint8_t reg1, uint8_t reg2, int32_t value) {
      registers.SetRegister(reg1, static_cast<int32_t>(registers.GetPC()) + SignExtended20Bits(value));
      return 0;
    };
  } else if (command_name == "addi") {
    command_ = [](RISCVRegisters& registers, ProcessMemory& memory, uint8_t reg1, uint8_t reg2, int32_t value) {
      registers.SetRegister(reg1, registers.GetRegister(reg2) + SignExtended12Bits(value));
      return 0;
    };
  } else if (command_name == "slti") {
    command_ = [](RISCVRegisters& registers, ProcessMemory& memory, uint8_t reg1, uint8_t reg2, int32_t value) {
      registers.SetRegister(reg1, registers.GetRegister(reg2) < SignExtended12Bits(value));
      return 0;
    };
  } else if (command_name == "sltiu") {
    command_ = [](RISCVRegisters& registers, ProcessMemory& memory, uint8_t reg1, uint8_t reg2, int32_t value) {
      registers.SetRegister(reg1, static_cast<uint32_t>(registers.GetRegister(reg2)) < Unsigned12Bits(value));
      return 0;
    };
  } else if (command_name == "xori") {
    command_ = [](RISCVRegisters& registers, ProcessMemory& memory, uint8_t reg1, uint8_t reg2, int32_t value) {
      registers.SetRegister(reg1, registers.GetRegister(reg2) ^ SignExtended12Bits(value));
      return 0;
    };
  } else if (command_name == "ori") {
    command_ = [](RISCVRegisters& registers, ProcessMemory& memory, uint8_t reg1, uint8_t reg2, int32_t value) {
      registers.SetRegister(reg1, registers.GetRegister(reg2) | SignExtended12Bits(value));
      return 0;
    };
  } else if (command_name == "andi") {
    command_ = [](RISCVRegisters& registers, ProcessMemory& memory, uint8_t reg1, uint8_t reg2, int32_t value) {
      registers.SetRegister(reg1, registers.GetRegister(reg2) & SignExtended12Bits(value));
      return 0;
    };
  } else if (command_name == "slli") {
    command_ = [](RISCVRegisters& registers, ProcessMemory& memory, uint8_t reg1, uint8_t reg2, int32_t value) {
      registers.SetRegister(reg1,
                            static_cast<int32_t>(static_cast<uint32_t>(registers.GetRegister(reg2)) << (value & 0x1f)));
      return 0;
    };
  } else if (command_name == "srli") {
    command_ = [](RISCVRegisters& registers, ProcessMemory& memory, uint8_t reg1, uint8_t reg2, int32_t value) {
      registers.SetRegister(reg1,
                            static_cast<int32_t>(static_cast<uint32_t>(registers.GetRegister(reg2)) >> (value & 0x1f)));
      return 0;
    };
  } else if (command_name == "srli") {
    command_ = [](RISCVRegisters& registers, ProcessMemory& memory, uint8_t reg1, uint8_t reg2, int32_t value) {
      registers.SetRegister(reg1, registers.GetRegister(reg2) >> (value & 0x1f));
      return 0;
    };
  } else if (command_name == "add") {
    command_ = [](RISCVRegisters& registers, ProcessMemory& memory, uint8_t reg1, uint8_t reg2, int32_t value) {
      registers.SetRegister(reg1, registers.GetRegister(reg2) + registers.GetRegister(value));
      return 0;
    };
  } else if (command_name == "sub") {
    command_ = [](RISCVRegisters& registers, ProcessMemory& memory, uint8_t reg1, uint8_t reg2, int32_t value) {
      registers.SetRegister(reg1, registers.GetRegister(reg2) - registers.GetRegister(value));
      return 0;
    };
  } else if (command_name == "slt") {
    command_ = [](RISCVRegisters& registers, ProcessMemory& memory, uint8_t reg1, uint8_t reg2, int32_t value) {
      registers.SetRegister(reg1, registers.GetRegister(reg2) < registers.GetRegister(value));
      return 0;
    };
  } else if (command_name == "sltu") {
    command_ = [](RISCVRegisters& registers, ProcessMemory& memory, uint8_t reg1, uint8_t reg2, int32_t value) {
      registers.SetRegister(reg1,
                            static_cast<uint32_t>(registers.GetRegister(reg2))
                                < static_cast<uint32_t>(registers.GetRegister(value)));
      return 0;
    };
  } else if (command_name == "xor") {
    command_ = [](RISCVRegisters& registers, ProcessMemory& memory, uint8_t reg1, uint8_t reg2, int32_t value) {
      registers.SetRegister(reg1, registers.GetRegister(reg2) ^ registers.GetRegister(value));
      return 0;
    };
  } else if (command_name == "or") {
    command_ = [](RISCVRegisters& registers, ProcessMemory& memory, uint8_t reg1, uint8_t reg2, int32_t value) {
      registers.SetRegister(reg1, registers.GetRegister(reg2) | registers.GetRegister(value));
      return 0;
    };
  } else if (command_name == "and") {
    command_ = [](RISCVRegisters& registers, ProcessMemory& memory, uint8_t reg1, uint8_t reg2, int32_t value) {
      registers.SetRegister(reg1, registers.GetRegister(reg2) & registers.GetRegister(value));
      return 0;
    };
  } else if (command_name == "srl") {
    command_ = [](RISCVRegisters& registers, ProcessMemory& memory, uint8_t reg1, uint8_t reg2, int32_t value) {
      registers.SetRegister(reg1,
                            static_cast<int32_t>(static_cast<uint32_t>(registers.GetRegister(reg2))
                                >> (registers.GetRegister(value) & 0x1f)));
      return 0;
    };
  } else if (command_name == "srl") {
    command_ = [](RISCVRegisters& registers, ProcessMemory& memory, uint8_t reg1, uint8_t reg2, int32_t value) {
      registers.SetRegister(reg1, registers.GetRegister(reg2) >> (registers.GetRegister(value) & 0x1f));
      return 0;
    };
  } else if (command_name == "ecall") {
    command_ = [](RISCVRegisters& registers, ProcessMemory& memory, uint8_t reg1, uint8_t reg2, int32_t value) {
      return 1;
    };
  } else if (command_name == "ebreak") {
    command_ = [](RISCVRegisters& registers, ProcessMemory& memory, uint8_t reg1, uint8_t reg2, int32_t value) {
      return 2;
    };
  } else if (command_name == "lb") {
    command_ = [](RISCVRegisters& registers, ProcessMemory& memory, uint8_t reg1, uint8_t reg2, int32_t value) {
      uint8_t result = memory.ReadByte(registers.GetRegister(reg2) + SignExtended12Bits(value));
      registers.SetRegister(reg1, SignExtended32Bits(result));
      return 0;
    };
  } else if (command_name == "lh") {
    command_ = [](RISCVRegisters& registers, ProcessMemory& memory, uint8_t reg1, uint8_t reg2, int32_t value) {
      uint16_t result = memory.ReadHalfWord(registers.GetRegister(reg2) + SignExtended12Bits(value));
      registers.SetRegister(reg1, SignExtended32Bits(result));
      return 0;
    };
  } else if (command_name == "lw") {
    command_ = [](RISCVRegisters& registers, ProcessMemory& memory, uint8_t reg1, uint8_t reg2, int32_t value) {
      uint32_t result = memory.ReadWord(registers.GetRegister(reg2) + SignExtended12Bits(value));
      registers.SetRegister(reg1, static_cast<int32_t>(result));
      return 0;
    };
  } else if (command_name == "lbu") {
    command_ = [](RISCVRegisters& registers, ProcessMemory& memory, uint8_t reg1, uint8_t reg2, int32_t value) {
      uint8_t result = memory.ReadByte(registers.GetRegister(reg2) + SignExtended12Bits(value));
      registers.SetRegister(reg1, static_cast<int32_t>(result));
      return 0;
    };
  } else if (command_name == "lhu") {
    command_ = [](RISCVRegisters& registers, ProcessMemory& memory, uint8_t reg1, uint8_t reg2, int32_t value) {
      uint16_t result = memory.ReadHalfWord(registers.GetRegister(reg2) + SignExtended12Bits(value));
      registers.SetRegister(reg1, static_cast<int32_t>(result));
      return 0;
    };
  } else if (command_name == "sb") {
    command_ = [](RISCVRegisters& registers, ProcessMemory& memory, uint8_t reg1, uint8_t reg2, int32_t value) {
      uint8_t result = static_cast<uint8_t>(registers.GetRegister(reg1) & 0xff);
      memory.WriteByte(registers.GetRegister(reg2) + SignExtended12Bits(value), result);
      return 0;
    };
  } else if (command_name == "sh") {
    command_ = [](RISCVRegisters& registers, ProcessMemory& memory, uint8_t reg1, uint8_t reg2, int32_t value) {
      uint16_t result = static_cast<uint16_t>(registers.GetRegister(reg1) & 0xffff);
      memory.WriteHalfWord(registers.GetRegister(reg2) + SignExtended12Bits(value), result);
      return 0;
    };
  } else if (command_name == "sw") {
    command_ = [](RISCVRegisters& registers, ProcessMemory& memory, uint8_t reg1, uint8_t reg2, int32_t value) {
      uint32_t result = static_cast<uint32_t>(registers.GetRegister(reg1));
      memory.WriteWord(registers.GetRegister(reg2) + SignExtended12Bits(value), result);
      return 0;
    };
  } else if (command_name == "jal") {
    command_ = [](RISCVRegisters& registers, ProcessMemory& memory, uint8_t reg1, uint8_t reg2, int32_t value) {
      registers.SetRegister(reg1, static_cast<int32_t>(registers.GetPC() + 4));
      registers.SetPC(registers.GetPC() + SignExtended12Bits(value));
      return 0;
    };
  } else if (command_name == "jalr") {
    command_ = [](RISCVRegisters& registers, ProcessMemory& memory, uint8_t reg1, uint8_t reg2, int32_t value) {
      int32_t t = static_cast<int32_t>(registers.GetPC() + 4);
      int32_t address = registers.GetRegister(reg2) + SignExtended12Bits(value);

      if (address == 0) {
        return -1;
      }

      if (address < 0) {
        return -2;
      }

      registers.SetPC(address);
      registers.SetRegister(reg1, t);
      return 0;
    };
  } else if (command_name == "beq") {
    command_ = [](RISCVRegisters& registers, ProcessMemory& memory, uint8_t reg1, uint8_t reg2, int32_t value) {
      if (registers.GetRegister(reg1) == registers.GetRegister(reg2)) {
        registers.SetPC(registers.GetPC() + SignExtended12Bits(value));
      }

      return 0;
    };
  } else if (command_name == "bne") {
    command_ = [](RISCVRegisters& registers, ProcessMemory& memory, uint8_t reg1, uint8_t reg2, int32_t value) {
      if (registers.GetRegister(reg1) != registers.GetRegister(reg2)) {
        registers.SetPC(registers.GetPC() + SignExtended12Bits(value));
      }

      return 0;
    };
  } else if (command_name == "blt") {
    command_ = [](RISCVRegisters& registers, ProcessMemory& memory, uint8_t reg1, uint8_t reg2, int32_t value) {
      if (registers.GetRegister(reg1) < registers.GetRegister(reg2)) {
        registers.SetPC(registers.GetPC() + SignExtended12Bits(value));
      }

      return 0;
    };
  } else if (command_name == "bge") {
    command_ = [](RISCVRegisters& registers, ProcessMemory& memory, uint8_t reg1, uint8_t reg2, int32_t value) {
      if (registers.GetRegister(reg1) >= registers.GetRegister(reg2)) {
        registers.SetPC(registers.GetPC() + SignExtended12Bits(value));
      }

      return 0;
    };
  } else if (command_name == "bltu") {
    command_ = [](RISCVRegisters& registers, ProcessMemory& memory, uint8_t reg1, uint8_t reg2, int32_t value) {
      if (static_cast<uint32_t>(registers.GetRegister(reg1)) < static_cast<uint32_t>(registers.GetRegister(reg2))) {
        registers.SetPC(registers.GetPC() + SignExtended12Bits(value));
      }

      return 0;
    };
  } else if (command_name == "bgeu") {
    command_ = [](RISCVRegisters& registers, ProcessMemory& memory, uint8_t reg1, uint8_t reg2, int32_t value) {
      if (static_cast<uint32_t>(registers.GetRegister(reg1)) >= static_cast<uint32_t>(registers.GetRegister(reg2))) {
        registers.SetPC(registers.GetPC() + SignExtended12Bits(value));
      }

      return 0;
    };
  } else if (command_name == "mul") {
    command_ = [](RISCVRegisters& registers, ProcessMemory& memory, uint8_t reg1, uint8_t reg2, int32_t value) {
      int64_t result = static_cast<int64_t>(registers.GetRegister(reg2)) * static_cast<int64_t>(registers.GetRegister(value));
      registers.SetRegister(reg1, static_cast<int32_t>(result));
      return 0;
    };
  } else if (command_name == "mulh") {
    command_ = [](RISCVRegisters& registers, ProcessMemory& memory, uint8_t reg1, uint8_t reg2, int32_t value) {
      int64_t result = static_cast<int64_t>(registers.GetRegister(reg2)) * static_cast<int64_t>(registers.GetRegister(value));
      registers.SetRegister(reg1, static_cast<int32_t>(result >> 32));
      return 0;
    };
  } else if (command_name == "mulhsu") {
    command_ = [](RISCVRegisters& registers, ProcessMemory& memory, uint8_t reg1, uint8_t reg2, int32_t value) {
      int64_t result = static_cast<int64_t>(registers.GetRegister(reg2)) * static_cast<uint64_t>(registers.GetRegister(value));
      registers.SetRegister(reg1, static_cast<int32_t>(result >> 32));
      return 0;
    };
  } else if (command_name == "mulhu") {
    command_ = [](RISCVRegisters& registers, ProcessMemory& memory, uint8_t reg1, uint8_t reg2, int32_t value) {
      uint64_t result = static_cast<uint64_t>(registers.GetRegister(reg2)) * static_cast<uint64_t>(registers.GetRegister(value));
      registers.SetRegister(reg1, static_cast<int32_t>(result >> 32));
      return 0;
    };
  } else if (command_name == "div") {
    command_ = [](RISCVRegisters& registers, ProcessMemory& memory, uint8_t reg1, uint8_t reg2, int32_t value) {
      if (registers.GetRegister(value) == 0) { // division by zero
        registers.SetRegister(reg1, -1);
      } else if (registers.GetRegister(reg2) == std::numeric_limits<int32_t>::min() && registers.GetRegister(value) == -1) { // arithmetic overflow
        registers.SetRegister(reg1, registers.GetRegister(reg2));
      }

      registers.SetRegister(reg1, registers.GetRegister(reg2) / registers.GetRegister(value));
      return 0;
    };
  } else if (command_name == "divu") {
    command_ = [](RISCVRegisters& registers, ProcessMemory& memory, uint8_t reg1, uint8_t reg2, int32_t value) {
      if (registers.GetRegister(value) == 0) { // division by zero
        registers.SetRegister(reg1, static_cast<int32_t>(std::numeric_limits<uint32_t>::max()));
      } else if (registers.GetRegister(reg2) == std::numeric_limits<int32_t>::min() && registers.GetRegister(value) == -1) { // arithmetic overflow
        registers.SetRegister(reg1, 0);
      }

      uint32_t result = static_cast<uint32_t>(registers.GetRegister(reg2)) / static_cast<uint32_t>(registers.GetRegister(value));
      registers.SetRegister(reg1, static_cast<int32_t>(result));
      return 0;
    };
  } else if (command_name == "rem") {
    command_ = [](RISCVRegisters& registers, ProcessMemory& memory, uint8_t reg1, uint8_t reg2, int32_t value) {
      if (registers.GetRegister(value) == 0) { // division by zero
        registers.SetRegister(reg1, registers.GetRegister(reg2));
      } else if (registers.GetRegister(reg2) == std::numeric_limits<int32_t>::min() && registers.GetRegister(value) == -1) { // arithmetic overflow
        registers.SetRegister(reg1, 0);
      }

      registers.SetRegister(reg1, registers.GetRegister(reg2) % registers.GetRegister(value));
      return 0;
    };
  } else if (command_name == "remu") {
    command_ = [](RISCVRegisters& registers, ProcessMemory& memory, uint8_t reg1, uint8_t reg2, int32_t value) {
      if (registers.GetRegister(value) == 0) { // division by zero
        registers.SetRegister(reg1, registers.GetRegister(reg2));
      } else if (registers.GetRegister(reg2) == std::numeric_limits<int32_t>::min() && registers.GetRegister(value) == -1) { // arithmetic overflow
        registers.SetRegister(reg1, 0);
      }

      uint32_t result = static_cast<uint32_t>(registers.GetRegister(reg2)) % static_cast<uint32_t>(registers.GetRegister(value));
      registers.SetRegister(reg1, static_cast<int32_t>(result));
      return 0;
    };
  } else {
    throw std::logic_error("Unknown command: " + command_name);
  }
}

int32_t RISCVAssemblerCommand::Execute(RISCVRegisters& registers, ProcessMemory& memory) {
  return command_(registers, memory, reg1_, reg2_, value_);
}

const std::string& RISCVAssemblerCommand::GetCommandName() const {
  return command_name_;
}

uint8_t RISCVAssemblerCommand::GetReg1() const {
  return reg1_;
}

uint8_t RISCVAssemblerCommand::GetReg2() const {
  return reg2_;
}

int32_t RISCVAssemblerCommand::GetValue() const {
  return value_;
}

int32_t RISCVAssemblerCommand::SignExtended12Bits(int32_t value) {
  uint32_t pre_result = std::abs(value) & 0xfff;
  return static_cast<int32_t>(pre_result) * (value < 0 ? -1 : 1);
}

int32_t RISCVAssemblerCommand::SignExtended20Bits(int32_t value) {
  return value << 12;
}

int32_t RISCVAssemblerCommand::SignExtended32Bits(uint8_t value) {
  return static_cast<int8_t>(value);
}

int32_t RISCVAssemblerCommand::SignExtended32Bits(uint16_t value) {
  return static_cast<int16_t>(value);
}

uint32_t RISCVAssemblerCommand::Unsigned12Bits(uint32_t value) {
  return value & 0xfff;
}

uint32_t RISCVAssemblerCommand::Unsigned20Bits(uint32_t value) {
  return value << 12;
}
