#include "RISCVAssemblerToBinary.hpp"

uint32_t RISCVAssemblerToBinary::ToBinary(const RISCVAssemblerCommand& command) {
  uint32_t result = 0;
  const std::string& command_name = command.GetCommandName();
  uint8_t reg1 = command.GetReg1() & 0x1f;
  uint8_t reg2 = command.GetReg2() & 0x1f;
  uint8_t reg3 = static_cast<uint8_t>(command.GetValue()) & 0x1f;
  auto value = static_cast<uint32_t>(command.GetValue());
  uint32_t imm_u = value & 0xfffff;
  uint32_t imm_j = value & 0xfff;
  uint8_t shamt = value & 0x1f;

  if (command_name == "lui") {
    result = (imm_u << 12) | (reg1 << 7) | 0b0110111;
  } else if (command_name == "aiupc") {
    result = (imm_u << 12) | (reg1 << 7) | 0b0010111;
  } else if (command_name == "addi") {
    result = (imm_j << 20) | (reg2 << 15) | (reg1 << 7) | 0b0010011;
  } else if (command_name == "slti") {
    result = (imm_j << 20) | (reg2 << 15) | (0b010 << 12) | (reg1 << 7) | 0b0010011;
  } else if (command_name == "sltiu") {
    result = (imm_j << 20) | (reg2 << 15) | (0b011 << 12) | (reg1 << 7) | 0b0010011;
  } else if (command_name == "xori") {
    result = (imm_j << 20) | (reg2 << 15) | (0b100 << 12) | (reg1 << 7) | 0b0010011;
  } else if (command_name == "ori") {
    result = (imm_j << 20) | (reg2 << 15) | (0b110 << 12) | (reg1 << 7) | 0b0010011;
  } else if (command_name == "andi") {
    result = (imm_j << 20) | (reg2 << 15) | (0b111 << 12) | (reg1 << 7) | 0b0010011;
  } else if (command_name == "slli") {
    result = (shamt << 20) | (reg2 << 15) | (0b001 << 12) | (reg1 << 7) | 0b0010011;
  } else if (command_name == "srli") {
    result = (shamt << 20) | (reg2 << 15) | (0b101 << 12) | (reg1 << 7) | 0b0010011;
  } else if (command_name == "srli") {
    result = (0b01000 << 27) | ((imm_j & 0x1f) << 20) | (reg1 << 15) | (0b101 << 12) | (reg1 << 7) | 0b0010011;
  } else if (command_name == "add") {
    result = (reg3 << 20) | (reg2 << 15) | (reg1 << 7) | 0b0110011;
  } else if (command_name == "sub") {
    result = (0b01000 << 27) | (reg3 << 20) | (reg2 << 15) | (reg1 << 7) | 0b0110011;
  } else if (command_name == "sll") {
    result = (reg3 << 20) | (reg2 << 15) | (0b001 << 12) | (reg1 << 7) | 0b0110011;
  } else if (command_name == "slt") {
    result = (reg3 << 20) | (reg2 << 15) | (0b010 << 12) | (reg1 << 7) | 0b0110011;
  } else if (command_name == "sltu") {
    result = (reg3 << 20) | (reg2 << 15) | (0b011 << 12) | (reg1 << 7) | 0b0110011;
  } else if (command_name == "xor") {
    result = (reg3 << 20) | (reg2 << 15) | (0b100 << 12) | (reg1 << 7) | 0b0110011;
  } else if (command_name == "or") {
    result = (reg3 << 20) | (reg2 << 15) | (0b110 << 12) | (reg1 << 7) | 0b0110011;
  } else if (command_name == "and") {
    result = (reg3 << 20) | (reg2 << 15) | (0b111 << 12) | (reg1 << 7) | 0b0110011;
  } else if (command_name == "srl") {
    result = (reg3 << 20) | (reg2 << 15) | (0b101 << 12) | (reg1 << 7) | 0b0110011;
  } else if (command_name == "sra") {
    result = (0b01000 << 27) | (reg3 << 20) | (reg2 << 15) | (0b101 << 12) | (reg1 << 7) | 0b0110011;
  } else if (command_name == "ecall") {
    result = 0b00000000000000000000000001110011;
  } else if (command_name == "ebreak") {
    result = 0b00000000000100000000000001110111;
  } else if (command_name == "lb") {
    result = (imm_j << 20) | (reg2 << 15) | (reg1 << 7) | 0b0000011;
  } else if (command_name == "lh") {
    result = (imm_j << 20) | (reg2 << 15) | (0b001 << 12) | (reg1 << 7) | 0b0000011;
  } else if (command_name == "lw") {
    result = (imm_j << 20) | (reg2 << 15) | (0b010 << 12) | (reg1 << 7) | 0b0000011;
  } else if (command_name == "lbu") {
    result = (imm_j << 20) | (reg2 << 15) | (0b100 << 12) | (reg1 << 7) | 0b0000011;
  } else if (command_name == "lhu") {
    result = (imm_j << 20) | (reg2 << 15) | (0b101 << 12) | (reg1 << 7) | 0b0000011;
  } else if (command_name == "sb") {
    result = ((imm_j & 0xfe0) << 20) | (reg1 << 20) | (reg2 << 15) | ((imm_j & 0x1f) << 7) | 0b0100011;
  } else if (command_name == "sh") {
    result = ((imm_j & 0xfe0) << 20) | (reg1 << 20) | (reg2 << 15) | (0b001 << 12) | ((imm_j & 0x1f) << 7) | 0b0100011;
  } else if (command_name == "sw") {
    result = ((imm_j & 0xfe0) << 20) | (reg1 << 20) | (reg2 << 15) | (0b010 << 12) | ((imm_j & 0x1f) << 7) | 0b0100011;
  } else if (command_name == "jal") {
    uint32_t imm_v = value & 0x1ffffe;
    uint32_t hi = ((imm_v >> 20) << 11) | (imm_v & 0b11111111110) | ((imm_v & 0b100000000000) >> 11);
    uint32_t lo = (imm_v >> 12) & 0x1f;
    result = (hi << 20) | (lo << 12) | (reg1 << 7) | 0b1101111;
  } else if (command_name == "jalr") {
    result = (imm_j << 20) | (reg2 << 15) | (reg1 << 7) | 0b1100111;
  } else if (command_name == "beq") {
    uint32_t imm_i = imm_u & 0x1ffe;
    uint32_t hi = ((imm_i & 0x1000) >> 1) | (imm_i & 0b11111100000);
    uint32_t lo = (imm_i & 0b11110) | ((imm_i & 0b100000000000) >> 11);
    result = (hi << 20) | (reg2 << 20) | (reg1 << 15) | (lo << 7) | 0b1100011;
  } else if (command_name == "bne") {
    uint32_t imm_i = imm_u & 0x1ffe;
    uint32_t hi = ((imm_i & 0x1000) >> 1) | (imm_i & 0b11111100000);
    uint32_t lo = (imm_i & 0b11110) | ((imm_i & 0b100000000000) >> 11);
    result = (hi << 20) | (reg2 << 20) | (reg1 << 15) | (0b001 << 12) | (lo << 7) | 0b1100011;
  } else if (command_name == "blt") {
    uint32_t imm_i = imm_u & 0x1ffe;
    uint32_t hi = ((imm_i & 0x1000) >> 1) | (imm_i & 0b11111100000);
    uint32_t lo = (imm_i & 0b11110) | ((imm_i & 0b100000000000) >> 11);
    result = (hi << 20) | (reg2 << 20) | (reg1 << 15) | (0b100 << 12) | (lo << 7) | 0b1100011;
  } else if (command_name == "bge") {
    uint32_t imm_i = imm_u & 0x1ffe;
    uint32_t hi = ((imm_i & 0x1000) >> 1) | (imm_i & 0b11111100000);
    uint32_t lo = (imm_i & 0b11110) | ((imm_i & 0b100000000000) >> 11);
    result = (hi << 20) | (reg2 << 20) | (reg1 << 15) | (0b101 << 12) | (lo << 7) | 0b1100011;
  } else if (command_name == "bltu") {
    uint32_t imm_i = imm_u & 0x1ffe;
    uint32_t hi = ((imm_i & 0x1000) >> 1) | (imm_i & 0b11111100000);
    uint32_t lo = (imm_i & 0b11110) | ((imm_i & 0b100000000000) >> 11);
    result = (hi << 20) | (reg2 << 20) | (reg1 << 15) | (0b110 << 12) | (lo << 7) | 0b1100011;
  } else if (command_name == "bgeu") {
    uint32_t imm_i = imm_u & 0x1ffe;
    uint32_t hi = ((imm_i & 0x1000) >> 1) | (imm_i & 0b11111100000);
    uint32_t lo = (imm_i & 0b11110) | ((imm_i & 0b100000000000) >> 11);
    result = (hi << 20) | (reg2 << 20) | (reg1 << 15) | (0b111 << 12) | (lo << 7) | 0b1100011;
  } else if (command_name == "mul") {
    result = (0b1 << 25) | (reg3 << 20) | (reg2 << 15) | (reg1 << 7) | 0b0110011;
  } else if (command_name == "mulh") {
    result = (0b1 << 25) | (reg3 << 20) | (reg2 << 15) | (0b001 << 12) | (reg1 << 7) | 0b0110011;
  } else if (command_name == "mulhsu") {
    result = (0b1 << 25) | (reg3 << 20) | (reg2 << 15) | (0b010 << 12) | (reg1 << 7) | 0b0110011;
  } else if (command_name == "mulhu") {
    result = (0b1 << 25) | (reg3 << 20) | (reg2 << 15) | (0b011 << 12) | (reg1 << 7) | 0b0110011;
  } else if (command_name == "div") {
    result = (0b1 << 25) | (reg3 << 20) | (reg2 << 15) | (0b100 << 12) | (reg1 << 7) | 0b0110011;
  } else if (command_name == "divu") {
    result = (0b1 << 25) | (reg3 << 20) | (reg2 << 15) | (0b101 << 12) | (reg1 << 7) | 0b0110011;
  } else if (command_name == "rem") {
    result = (0b1 << 25) | (reg3 << 20) | (reg2 << 15) | (0b110 << 12) | (reg1 << 7) | 0b0110011;
  } else if (command_name == "remu") {
    result = (0b1 << 25) | (reg3 << 20) | (reg2 << 15) | (0b111 << 12) | (reg1 << 7) | 0b0110011;
  }

  return result;
}

std::vector<uint32_t> RISCVAssemblerToBinary::Assemble(const std::vector<RISCVAssemblerCommand>& commands) {
  std::vector<uint32_t> result(commands.size());

  for (size_t i = 0; i < commands.size(); i++) {
    result[i] = ToBinary(commands[i]);
  }

  return result;
}
