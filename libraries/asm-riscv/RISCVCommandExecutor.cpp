#include <stdexcept>

#include "RISCVCommandExecutor.hpp"

RISCVCommandExecutor::RISCVCommandExecutor(const std::vector<RISCVAssemblerCommand>& commands)
    : commands_(commands), memory_(), registers_() {}

void RISCVCommandExecutor::Execute() {
  for (size_t command_index = 0; command_index < commands_.size(); command_index = registers_.GetPC() / 4) {
    uint32_t pc = registers_.GetPC();
    int32_t result = commands_[command_index].Execute(registers_, memory_);

    if (result == -1) {
      registers_.SetPC(commands_.size() * 4);
      break;
    }

    if (result != 0) {
      std::string error_message = "Unknown error";

      switch (result) {
        case 1:
          error_message = "ecall raised";
          break;
        case 2:
          error_message = "ebreak raised";
          break;
        default:
          break;
      }

      throw std::runtime_error(error_message);
    }

    if (pc == registers_.GetPC()) {
      registers_.SetPC(pc + 4);
    }
  }

  if (registers_.GetPC() / 4 != commands_.size()) {
    throw std::runtime_error("PC out of bounds");
  }
}

float RISCVCommandExecutor::GetLRUHitRate() const {
  return memory_.GetLRUHitRate();
}

float RISCVCommandExecutor::GetBitpLRUHitRate() const {
  return memory_.GetBitpLRUHitRate();
}
